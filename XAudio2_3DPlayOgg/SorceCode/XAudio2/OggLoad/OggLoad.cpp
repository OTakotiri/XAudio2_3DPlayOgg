// ogg_int64_t �� DWORD�^�ɓ���鎞�̌x��������.
#pragma warning( disable: 4244 )

#include "OggLoad.h"
#include <memory.h>
#include <crtdbg.h>
#include <tchar.h>
//=============================
// �萔�錾.
//=============================
namespace
{
	constexpr unsigned int REQUEST_SIZE = 4096;	// �ǂݍ��ݒP��

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
// ogg�f�[�^�쐬.
//=============================
HRESULT COggLoad::CreateOggData(std::string sFileName, const bool& isSE, bool isBuffered)
{
	HRESULT ret;
	if (FAILED(ret = Open(sFileName,isBuffered)))
	{
		_ASSERT_EXPR("error ogg file open ret=%d\n", ret);
		return E_FAIL;
	}
	// �t���O�������Ă����SE�̍쐬.
	if (isSE == true) {
		// SE�t�@�C���̃T�C�Y
		m_SeFileSize = GetSize();
		// SE�t�@�C���̓ǂݍ��ݗp�o�b�t�@�[
		m_SeWaveBuffer = new BYTE[m_SeFileSize];

		// SE�t�@�C���̃f�[�^���o�b�t�@�[�ɓǂݍ���
		if (FAILED(ret = Read(m_SeWaveBuffer, m_SeFileSize, &m_SeFileSize))) {
			_ASSERT_EXPR("error read SE File %#X\n", ret);
			return E_FAIL;
		}
	}

	return S_OK;
}
//=============================
// �t�@�C�����J��.
//=============================
HRESULT COggLoad::Open(std::string sFileName, bool isBuffered)
{
	HRESULT hr;
	errno_t error;
	// �t�@�C�����`�F�b�N
	if (sFileName == "") return E_INVALIDARG;
	// BGM�f�[�^������f�B���N�g���p�X������.
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


	//// CastLPSTR���G���[�l�Ȃ�SE�̃t�@�C���̖��O�̉\��������̂ŁASE�̃f�B���N�g���p�X�Ō���.
	//if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
	//	std::string SEPass = "Data\\Sound\\SE\\" + sFileName;
	//	SEPass += ".ogg";
	//	LPSTR CastLPSTR = const_cast<char*>(SEPass.c_str());
	//	if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
	//		// �f�[�^�����݂��Ȃ�.
	//		return E_FAIL;
	//	}
	//}
	// Ogg�\���̂ɏ�����荞��.
	error = ov_open(m_pFile, &m_OggVF, nullptr, 0);
	if (error != 0) return E_FAIL;

	// ogg�t�@�C�������擾���ăZ�b�g
	vorbis_info *info = ov_info(&m_OggVF, -1);
	// �t�@�C�����Z�b�g.
	SetFileName(sFileName);
	// �`�����l���i���o�[�Z�b�g.
	SetChannelNumber(info->channels);
	// bitrate��16�ɂ��Ă���
	SetBitRate(16);
	// �T���v�����O���[�g�Z�b�g.
	SetSamplingRate(info->rate);
	// �Z�b�g�������g���ăt�H�[�}�b�g�t�@�C���쐬
	if (!CreateWaveFormatEx()) return E_FAIL;
	// �T�C�Y���v�Z���ē���Ă���
	m_Size = info->channels * 2 * ov_pcm_total(&m_OggVF, -1);

	// �t�@�C���̏������o����
	m_isReady = true;
	hr = S_OK;
	return hr;
}
//=============================
// �t�@�C���̓ǂݎ��|�C���^�[�����Z�b�g����.
// ������Ă񂾂�Ogg�����߂���Đ������.
//=============================
HRESULT COggLoad::ResetFile()
{
	if (!GetReady())
		return E_FAIL;

	ov_time_seek(&m_OggVF, 0.0);

	return S_OK;
}
//=============================
// �t�@�C�����e�����ׂăo�b�t�@�ɓǂݍ���.
//=============================
HRESULT COggLoad::Read(BYTE* buffer, DWORD size, DWORD* readSize)
{
	// �ǂݍ��ރo�b�t�@�[���������Ɋ��蓖��
	memset(buffer, 0, this->m_Size);
	char *tmpBuffer = new char[this->m_Size];
	int bitStream = 0;
	int tmpReadSize = 0;
	int comSize = 0;

	*readSize = 0;
	while (1)
	{
		tmpReadSize = ov_read(&m_OggVF, (char*)tmpBuffer, 4096, 0, 2, 1, &bitStream);
		// �ǂݍ��݃T�C�Y�����Ă���A�t�@�C�������܂œǂݍ��݂��Ă���break
		if (comSize + tmpReadSize >= static_cast<int>(this->m_Size) || tmpReadSize == 0 || tmpReadSize == EOF)
			break;
		// �������[�̃o�b�t�@�[�Ƀ|�C���^�[�ʒu���炵�Ȃ��珑������
		memcpy(buffer + comSize, tmpBuffer, tmpReadSize);
		comSize += tmpReadSize;
	}


	// �ǂݍ��񂾃T�C�Y���Z�b�g
	*readSize = comSize;
	// ��n��
	delete[] tmpBuffer;

	return S_OK;

}
//=============================
// �w��̃T�C�Y�܂Ŗ��߂��Z�O�����g�f�[�^���擾����.
//=============================
bool COggLoad::GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd)
{
	// �t�@�C���̏������I����ĂȂ���ΏI��
	if (GetReady() == false)
		return false;

	// �o�b�t�@�[�̎w��`�F�b�N
	if (buffer == 0)
	{
		if (isEnd) *isEnd = true;
		if (writeSize) *writeSize = 0;
		return false;
	}

	if (isEnd) *isEnd = false;

	// ��������Ƀo�b�t�@�[�̈���m��
	memset(buffer, 0, size);
	unsigned int requestSize = REQUEST_SIZE;
	int bitStream = 0;
	int readSize = 0;
	unsigned int comSize = 0;
	bool isAdjust = false; // �[���̃f�[�^�̒������t���O

	// �w��T�C�Y���\��T�C�Y��菬�����ꍇ�͒������Ƃ݂Ȃ�
	if (size < requestSize)
	{
		requestSize = size;
		isAdjust = true;
	}

	// �o�b�t�@�[���w��T�C�Y�Ŗ��߂�܂ŌJ��Ԃ�
	while (1)
	{
		readSize = ov_read(&m_OggVF, (char*)(buffer + comSize), requestSize, 0, 2, 1, &bitStream);
		// �t�@�C���G���h�ɒB����
		if (readSize == 0)
		{
			// �I��
			if (isEnd) *isEnd = true;
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// �ǂݎ��T�C�Y�����Z
		comSize += readSize;

		_ASSERT(comSize <= size);	// �o�b�t�@�I�[�o�[

		// �o�b�t�@�𖄂ߐs������
		if (comSize >= size)
		{
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// �[���f�[�^�̒���
		if (size - comSize < REQUEST_SIZE)
		{
			isAdjust = true;
			requestSize = size - comSize;
		}

	}

	// �G���[
	if (writeSize) *writeSize = 0;
	return false;
}
//=============================
// �t�@�C������`�����N�f�[�^��ǂݍ���Ńo�b�t�@������.
//=============================
HRESULT COggLoad::ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize)
{
	if (readSize != NULL)
		*readSize = 0;

	// �t�@�C������w�肳�ꂽ�T�C�Y�����f�[�^��ǂݎ��
	bool result = GetSegment((char *)buffer[bufferCount], len, readSize, 0);
	if (result == false)
		return E_FAIL;

	return S_OK;
}
//=============================
// �f�[�^���.
//=============================
HRESULT COggLoad::Close()
{
	ov_clear(&m_OggVF);	// OggVorbis_File�\���̂��N���A.
	if (m_pFile)		// �t�@�C�����J����Ă���.
	{
		fclose(m_pFile);// �t�@�C�������.
		m_pFile = nullptr;	// ���g��nullptr.
	}
	// ���.
	SAFE_DELETE(m_SeWaveBuffer);
	SAFE_DELETE(m_WaveFormat);	// WAVEFORMATEX�̉��.

	return S_OK;
}
//=============================
// Wav�t�H�[�}�b�g�f�[�^�쐬.
//=============================
const bool COggLoad::CreateWaveFormatEx()
{
	m_WaveFormat = nullptr;
	m_WaveFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	if (nullptr == m_WaveFormat)
		return false;

	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;		// PCM�`����ݒ�.
	m_WaveFormat->nChannels = m_ChannelNumber;		// �`�����l������ݒ�.
	m_WaveFormat->nSamplesPerSec = m_SamplingRate;	// �T���v�����O���g����ݒ�.
	m_WaveFormat->wBitsPerSample = m_BitRate;		// �ʎq���r�b�g��(�r�b�g���[�g)��ݒ�.
	m_WaveFormat->nBlockAlign = m_ChannelNumber * m_BitRate / 8;	// �f�[�^�̍ŏ��P�ʂ�ݒ�.
	m_WaveFormat->nAvgBytesPerSec = m_WaveFormat->nSamplesPerSec * m_WaveFormat->nBlockAlign;	// 1�b�Ԃ̃o�C�g����ݒ�.
	m_WaveFormat->cbSize = 0;

	return true;
}
