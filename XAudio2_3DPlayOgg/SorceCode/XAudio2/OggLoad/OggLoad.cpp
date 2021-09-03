// ogg_int64_t を DWORD型に入れる時の警告を消す.
#pragma warning( disable: 4244 )

#include "OggLoad.h"
#include <memory.h>
#include <crtdbg.h>
#include <tchar.h>
//=============================
// 定数宣言.
//=============================
namespace
{
	constexpr unsigned int REQUEST_SIZE = 4096;	// 読み込み単位

	constexpr int FILE_PATH_SIZE = 4;
	const std::string FILE_PATH[] = { 
		"Data\\Sound\\BGM\\",
		"Data\\Sound\\3DBGM\\",
		"Data\\Sound\\SE\\",
		"Data\\Sound\\3DSE\\"
	};
}

COggLoad::COggLoad()
	: m_WaveFormat		( nullptr )
	, m_Size			( 0 )
	, m_isReady			( false )
	, m_SeWaveBuffer	( nullptr )
	, m_SeFileSize		( 0 )
{
}

COggLoad::~COggLoad()
{
	//Close();
}
//=============================
// oggデータ作成.
//=============================
HRESULT COggLoad::CreateOggData(std::string sFileName, const bool& isSE, bool isBuffered)
{
	HRESULT ret;
	if (FAILED(ret = Open(sFileName,isBuffered)))
	{
		_ASSERT_EXPR("error ogg file open ret=%d\n", ret);
		return E_FAIL;
	}
	// フラグが立っていればSEの作成.
	if (isSE == true) {
		// SEファイルのサイズ
		m_SeFileSize = GetSize();
		// SEファイルの読み込み用バッファー
		m_SeWaveBuffer = new BYTE[m_SeFileSize];

		// SEファイルのデータをバッファーに読み込み
		if (FAILED(ret = Read(m_SeWaveBuffer, m_SeFileSize, &m_SeFileSize))) {
			_ASSERT_EXPR("error read SE File %#X\n", ret);
			return E_FAIL;
		}
	}

	return S_OK;
}
//=============================
// ファイルを開く.
//=============================
HRESULT COggLoad::Open(std::string sFileName, bool isBuffered)
{
	HRESULT hr;
	errno_t error;
	// ファイル名チェック
	if (sFileName == "") return E_INVALIDARG;
	// BGMデータがあるディレクトリパスを入れる.
/*	std::string BGMPass = FILE_PATH[0] + sFileName;
	BGMPass += ".ogg"*/;
	//LPSTR CastLPSTR = const_cast<char*>(BGMPass.c_str());

	bool Success = false;
	std::string Path;
	for (int i = 0; i < FILE_PATH_SIZE; i++) {
		Path = FILE_PATH[i] + sFileName += ".ogg";
		LPSTR CastLPSTR = const_cast<char*>(Path.c_str());
		if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
			continue;
		}
		else {
			Success = true;
			break;
		}
	}
	if(Success == false) return E_FAIL;


	//// CastLPSTRがエラー値ならSEのファイルの名前の可能性があるので、SEのディレクトリパスで検索.
	//if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
	//	std::string SEPass = "Data\\Sound\\SE\\" + sFileName;
	//	SEPass += ".ogg";
	//	LPSTR CastLPSTR = const_cast<char*>(SEPass.c_str());
	//	if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
	//		// データが存在しない.
	//		return E_FAIL;
	//	}
	//}
	// Ogg構造体に情報を取り込む.
	error = ov_open(m_pFile, &m_OggVF, nullptr, 0);
	if (error != 0) return E_FAIL;

	// oggファイル情報を取得してセット
	vorbis_info *info = ov_info(&m_OggVF, -1);
	// ファイル名セット.
	SetFileName(sFileName);
	// チャンネルナンバーセット.
	SetChannelNumber(info->channels);
	// bitrateは16にしておく
	SetBitRate(16);
	// サンプリングレートセット.
	SetSamplingRate(info->rate);
	// セットした情報使ってフォーマットファイル作成
	if (!CreateWaveFormatEx()) return E_FAIL;
	// サイズを計算して入れておく
	m_Size = info->channels * 2 * ov_pcm_total(&m_OggVF, -1);

	// ファイルの準備が出来た
	m_isReady = true;
	hr = S_OK;
	return hr;
}
//=============================
// ファイルの読み取りポインターをリセットする.
// これを呼んだらOggが初めから再生される.
//=============================
HRESULT COggLoad::ResetFile()
{
	if (!GetReady())
		return E_FAIL;

	ov_time_seek(&m_OggVF, 0.0);

	return S_OK;
}
//=============================
// ファイル内容をすべてバッファに読み込み.
//=============================
HRESULT COggLoad::Read(BYTE* buffer, DWORD size, DWORD* readSize)
{
	// 読み込むバッファーをメモリに割り当て
	memset(buffer, 0, this->m_Size);
	char *tmpBuffer = new char[this->m_Size];
	int bitStream = 0;
	int tmpReadSize = 0;
	int comSize = 0;

	*readSize = 0;
	while (1)
	{
		tmpReadSize = ov_read(&m_OggVF, (char*)tmpBuffer, 4096, 0, 2, 1, &bitStream);
		// 読み込みサイズ超えてたり、ファイル末尾まで読み込みしてたらbreak
		if (comSize + tmpReadSize >= static_cast<int>(this->m_Size) || tmpReadSize == 0 || tmpReadSize == EOF)
			break;
		// メモリーのバッファーにポインター位置ずらしながら書き込み
		memcpy(buffer + comSize, tmpBuffer, tmpReadSize);
		comSize += tmpReadSize;
	}


	// 読み込んだサイズをセット
	*readSize = comSize;
	// 後始末
	delete[] tmpBuffer;

	return S_OK;

}
//=============================
// 指定のサイズまで埋めたセグメントデータを取得する.
//=============================
bool COggLoad::GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd)
{
	// ファイルの準備が終わってなければ終了
	if (GetReady() == false)
		return false;

	// バッファーの指定チェック
	if (buffer == 0)
	{
		if (isEnd) *isEnd = true;
		if (writeSize) *writeSize = 0;
		return false;
	}

	if (isEnd) *isEnd = false;

	// メモリ上にバッファー領域を確保
	memset(buffer, 0, size);
	unsigned int requestSize = REQUEST_SIZE;
	int bitStream = 0;
	int readSize = 0;
	unsigned int comSize = 0;
	bool isAdjust = false; // 端数のデータの調整中フラグ

	// 指定サイズが予定サイズより小さい場合は調整中とみなす
	if (size < requestSize)
	{
		requestSize = size;
		isAdjust = true;
	}

	// バッファーを指定サイズで埋めるまで繰り返す
	while (1)
	{
		readSize = ov_read(&m_OggVF, (char*)(buffer + comSize), requestSize, 0, 2, 1, &bitStream);
		// ファイルエンドに達した
		if (readSize == 0)
		{
			// 終了
			if (isEnd) *isEnd = true;
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// 読み取りサイズを加算
		comSize += readSize;

		_ASSERT(comSize <= size);	// バッファオーバー

		// バッファを埋め尽くした
		if (comSize >= size)
		{
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// 端数データの調整
		if (size - comSize < REQUEST_SIZE)
		{
			isAdjust = true;
			requestSize = size - comSize;
		}

	}

	// エラー
	if (writeSize) *writeSize = 0;
	return false;
}
//=============================
// ファイルからチャンクデータを読み込んでバッファを入れる.
//=============================
HRESULT COggLoad::ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize)
{
	if (readSize != NULL)
		*readSize = 0;

	// ファイルから指定されたサイズだけデータを読み取る
	bool result = GetSegment((char *)buffer[bufferCount], len, readSize, 0);
	if (result == false)
		return E_FAIL;

	return S_OK;
}
//=============================
// データ解放.
//=============================
HRESULT COggLoad::Close()
{
	ov_clear(&m_OggVF);	// OggVorbis_File構造体をクリア.
	if (m_pFile)		// ファイルが開かれている.
	{
		fclose(m_pFile);// ファイルを閉じる.
		m_pFile = nullptr;	// 中身にnullptr.
	}
	// 解放.
	SAFE_DELETE(m_SeWaveBuffer);
	SAFE_DELETE(m_WaveFormat);	// WAVEFORMATEXの解放.

	return S_OK;
}
//=============================
// Wavフォーマットデータ作成.
//=============================
const bool COggLoad::CreateWaveFormatEx()
{
	m_WaveFormat = nullptr;
	m_WaveFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	if (nullptr == m_WaveFormat)
		return false;

	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;		// PCM形式を設定.
	m_WaveFormat->nChannels = m_ChannelNumber;		// チャンネル数を設定.
	m_WaveFormat->nSamplesPerSec = m_SamplingRate;	// サンプリング周波数を設定.
	m_WaveFormat->wBitsPerSample = m_BitRate;		// 量子化ビット数(ビットレート)を設定.
	m_WaveFormat->nBlockAlign = m_ChannelNumber * m_BitRate / 8;	// データの最小単位を設定.
	m_WaveFormat->nAvgBytesPerSec = m_WaveFormat->nSamplesPerSec * m_WaveFormat->nBlockAlign;	// 1秒間のバイト数を設定.
	m_WaveFormat->cbSize = 0;

	return true;
}
