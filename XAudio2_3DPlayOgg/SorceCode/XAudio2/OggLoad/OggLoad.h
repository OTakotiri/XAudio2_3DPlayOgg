#ifndef OGGLOAD_H
#define OGGLOAD_H

#include "..\XAudio2.h"
#include <string>
#include <vorbis\vorbisfile.h>
/**
 * @file OggLoad.h
 * @brief Ogg�̃f�[�^�̓ǂݍ��݁A�쐬�N���X.
 * @author OTakotiri.
 */

class COggLoad
{
public: // --------- �֐� ---------
	COggLoad();
	virtual ~COggLoad();
	// ogg�f�[�^�쐬.
	HRESULT CreateOggData(std::string sFileName,const bool& isSE = false, bool isBuffered = false);
	// �t�@�C�����J��
	HRESULT Open(std::string sFileName,bool isBuffered = false);
	// �t�@�C�������
	HRESULT Close();
	// �t�@�C����ǂݍ���
	HRESULT Read(BYTE* buffer, DWORD size, DWORD* readSize);
	// �`�����N�f�[�^��ǂݍ���(�א؂�ɂŁ[����ǂ݂���)
	HRESULT ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize);
	// �t�@�C���̈ʒu�����Z�b�g
	HRESULT ResetFile();
	// �t�@�C�����I�[�v���ς݂��ǂ������擾.
	inline const bool GetReady() { return m_isReady; }
	// �t�@�C���̖��O���擾.
	inline const char* GetFileName() { return m_sFileName.c_str(); } 
	// �T�C�Y���擾
	inline const DWORD GetSize() { return m_Size; }
	// Wav�t�H�[�}�b�g�擾.
	inline const WAVEFORMATEX* GetFormat() { return m_WaveFormat; }
	// SE��Wav�o�b�t�@�擾.
	inline const BYTE* GetSEWaveBuffer() { return m_SeWaveBuffer; }
	// SE�̃t�@�C���T�C�Y�擾.
	inline const DWORD GetSEFileSize() { return m_SeFileSize; }
private:
	// �Z�O�����g�f�[�^�擾.
	virtual bool GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd);
	// Wav�t�H�[�}�b�g�f�[�^�쐬.
	const bool CreateWaveFormatEx();
	// �t�@�C���l�[���Z�b�g.
	inline void SetFileName(std::string sName) { m_sFileName = sName; }
	// �`�����l���Z�b�g�֐�.
	inline void SetChannelNumber(int value) { m_ChannelNumber = value; }
	// �T���v�����O���[�g�Z�b�g�֐�.
	inline void SetSamplingRate(int value) { m_SamplingRate = value; }
	// W�r�b�g���[�g�Z�b�g�֐�.
	inline void SetBitRate(int value) { m_BitRate = value; }
private: // --------- �ϐ� ---------
	OggVorbis_File m_OggVF;			// OggVorbis�\����.
	FILE *m_pFile;					// �t�@�C��������ϐ�.
	bool m_isReady;					// �t�@�C�����I�[�v���ς݂Ȃ�true.
	std::string m_sFileName;		// �t�@�C����.
	unsigned int m_ChannelNumber;	// �`�����l��.
	unsigned int m_SamplingRate;	// �T���v�����O���[�g.
	unsigned int m_BitRate;			// �r�b�g���[�g.
	DWORD m_SeFileSize;				// SE�t�@�C���̃T�C�Y.
	BYTE* m_SeWaveBuffer;				// SE�t�@�C���̓ǂݍ��ݗp�o�b�t�@�[.
	WAVEFORMATEX* m_WaveFormat;		// WAVEFORMATEX�\����.		
	DWORD m_Size;					// Wav�t�@�C���̃T�C�Y������ϐ�.	
};




#endif // #ifndef OGGLOAD_H.