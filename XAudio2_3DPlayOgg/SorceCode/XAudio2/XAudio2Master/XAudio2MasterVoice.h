#ifndef XAUDIO2_MASTERVOICE_H
#define XAUDIO2_MASTERVOICE_H

#include "XAudio2.h"
#include <x3daudio.h>
#define COM_SAFE_RELEASE( p ) { if(p) { (p)->Release(); (p) = nullptr; } }

/**
 * @file XAudio2MasterVoice.h
 * @brief XAduio2�̍Đ��ɕK�v�ȃ}�X�^�[�{�C�X�N���X.
 * @author OTakotiri.
 */

class CXAudio2MasterVoice
{
public:
	CXAudio2MasterVoice();
	~CXAudio2MasterVoice();

	// �C���X�^���X�̎擾.
	static CXAudio2MasterVoice* GetInstance();

	IXAudio2* GetInterface()const;
	// �}�X�^�[�{�C�X�擾.
	IXAudio2MasteringVoice* GetMasteringVoice()const;
	static CXAudio2MasterVoice& GetGlobalSystem();
	// ����֐�.
	static void Release();
public:
	UINT					m_SampleRate;
	UINT					m_InputChannels;
	// 3D�T�E���h�n���h��.
	X3DAUDIO_HANDLE         m_x3DHandle;

protected:
	// �������֐�.
	bool Initialize();


	IXAudio2*				m_pXAudioInterface;                //XAudio2�C���^�[�t�F�[�X
	IXAudio2MasteringVoice* m_pMasterVoice;                    //�}�X�^�[�{�C�X.
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CXAudio2MasterVoice(const CXAudio2MasterVoice &) = delete;
	CXAudio2MasterVoice& operator = (const CXAudio2MasterVoice &) = delete;
	CXAudio2MasterVoice(CXAudio2MasterVoice &&) = delete;
	CXAudio2MasterVoice& operator = (CXAudio2MasterVoice &&) = delete;
};

#endif //#ifndef XAUDIO2_MASTERVOICE_H