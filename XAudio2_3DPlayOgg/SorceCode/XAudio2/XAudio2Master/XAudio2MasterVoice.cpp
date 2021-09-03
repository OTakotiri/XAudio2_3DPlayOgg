#include "XAudio2.h"
#include <iostream>
#include "XAudio2MasterVoice.h"

namespace {
	constexpr int MAXOUTPUT_CHANNEL = 8;
}

CXAudio2MasterVoice& CXAudio2MasterVoice::GetGlobalSystem()
{
	static CXAudio2MasterVoice xAudioMaster;
	return xAudioMaster;
}

CXAudio2MasterVoice::CXAudio2MasterVoice()
	: m_x3DHandle	()
{
	Initialize();
}

CXAudio2MasterVoice::~CXAudio2MasterVoice()
{
}
//=============================
// �C���X�^���X�̍쐬.
//=============================
CXAudio2MasterVoice* CXAudio2MasterVoice::GetInstance()
{
	static std::unique_ptr<CXAudio2MasterVoice> pInstance =
		std::make_unique<CXAudio2MasterVoice>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}
//=============================
// XAudio�C���^�[�t�F�[�X�擾.
//=============================
IXAudio2* CXAudio2MasterVoice::GetInterface()const
{
	return m_pXAudioInterface;
}
//=============================
// �}�X�^�[�{�C�X�擾.
//=============================
IXAudio2MasteringVoice* CXAudio2MasterVoice::GetMasteringVoice()const
{
	return m_pMasterVoice;
}

//=============================
// ������ XAudio�쐬 �}�X�^�[�{�C�X�쐬.
//=============================
bool CXAudio2MasterVoice::Initialize()
{
	// COM �̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) &&
		FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		_ASSERT_EXPR(false, "Failed CoInitialize");
		return false;
	}
	// XAudio2 �C���^�[�t�F�[�X�̍쐬
	if (FAILED(XAudio2Create(&m_pXAudioInterface, 0)))
	{
		_ASSERT_EXPR(false, "Create XAudio2Interface Is Failed");
		return false;
	}
	// �}�X�^�[�{�C�X�̍쐬
	if (FAILED(m_pXAudioInterface->CreateMasteringVoice(&m_pMasterVoice)))
	{
		_ASSERT_EXPR(false, "Create XAudio2MasterVoice Is Failed");
		return FALSE;
	}
	// �ȉ�3D�T�E���h.
	XAUDIO2_VOICE_DETAILS details;
	m_pMasterVoice->GetVoiceDetails(&details);
	// �T�E���h�o�̓`�����l�����w��l��葽��.
	if (details.InputChannels > MAXOUTPUT_CHANNEL)
	{
		_ASSERT_EXPR(false, "OutPutChannnel is Over");
		return false;
	}
	m_SampleRate = details.InputSampleRate;
	m_InputChannels = details.InputChannels;

	// 3D�T�E���h�̏�����.
	X3DAudioInitialize(SPEAKER_STEREO,
		X3DAUDIO_SPEED_OF_SOUND, m_x3DHandle);
	

	return true;
}
//=============================
// ����֐�.
//=============================
void CXAudio2MasterVoice::Release()
{
	// �}�X�^�[�{�C�X�����݂��Ă��Ȃ���΃��^�[��.
	if (GetInstance()->m_pMasterVoice == nullptr) return;
	// �}�X�^�[�{�C�X���.
	GetInstance()->m_pMasterVoice->DestroyVoice();
	GetInstance()->m_pMasterVoice = nullptr;
	// XAudio2�C���^�[�t�F�[�X�̃����[�X.
	COM_SAFE_RELEASE(GetInstance()->m_pXAudioInterface);
	GetInstance()->m_pXAudioInterface = nullptr;
	// COM���C�u���������
	CoUninitialize();
}