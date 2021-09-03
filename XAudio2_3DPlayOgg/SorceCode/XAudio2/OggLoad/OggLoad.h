#ifndef OGGLOAD_H
#define OGGLOAD_H

#include "..\XAudio2.h"
#include <string>
#include <vorbis\vorbisfile.h>
/**
 * @file OggLoad.h
 * @brief Oggのデータの読み込み、作成クラス.
 * @author OTakotiri.
 */

class COggLoad
{
public: // --------- 関数 ---------
	COggLoad();
	virtual ~COggLoad();
	// oggデータ作成.
	HRESULT CreateOggData(std::string sFileName,const bool& isSE = false, bool isBuffered = false);
	// ファイルを開く
	HRESULT Open(std::string sFileName,bool isBuffered = false);
	// ファイルを閉じる
	HRESULT Close();
	// ファイルを読み込む
	HRESULT Read(BYTE* buffer, DWORD size, DWORD* readSize);
	// チャンクデータを読み込み(細切れにでーたを読みこみ)
	HRESULT ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize);
	// ファイルの位置をリセット
	HRESULT ResetFile();
	// ファイルがオープン済みかどうかを取得.
	inline const bool GetReady() { return m_isReady; }
	// ファイルの名前を取得.
	inline const char* GetFileName() { return m_sFileName.c_str(); } 
	// サイズを取得
	inline const DWORD GetSize() { return m_Size; }
	// Wavフォーマット取得.
	inline const WAVEFORMATEX* GetFormat() { return m_WaveFormat; }
	// SEのWavバッファ取得.
	inline const BYTE* GetSEWaveBuffer() { return m_SeWaveBuffer; }
	// SEのファイルサイズ取得.
	inline const DWORD GetSEFileSize() { return m_SeFileSize; }
private:
	// セグメントデータ取得.
	virtual bool GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd);
	// Wavフォーマットデータ作成.
	const bool CreateWaveFormatEx();
	// ファイルネームセット.
	inline void SetFileName(std::string sName) { m_sFileName = sName; }
	// チャンネルセット関数.
	inline void SetChannelNumber(int value) { m_ChannelNumber = value; }
	// サンプリングレートセット関数.
	inline void SetSamplingRate(int value) { m_SamplingRate = value; }
	// Wビットレートセット関数.
	inline void SetBitRate(int value) { m_BitRate = value; }
private: // --------- 変数 ---------
	OggVorbis_File m_OggVF;			// OggVorbis構造体.
	FILE *m_pFile;					// ファイルを入れる変数.
	bool m_isReady;					// ファイルがオープン済みならtrue.
	std::string m_sFileName;		// ファイル名.
	unsigned int m_ChannelNumber;	// チャンネル.
	unsigned int m_SamplingRate;	// サンプリングレート.
	unsigned int m_BitRate;			// ビットレート.
	DWORD m_SeFileSize;				// SEファイルのサイズ.
	BYTE* m_SeWaveBuffer;				// SEファイルの読み込み用バッファー.
	WAVEFORMATEX* m_WaveFormat;		// WAVEFORMATEX構造体.		
	DWORD m_Size;					// Wavファイルのサイズを入れる変数.	
};




#endif // #ifndef OGGLOAD_H.