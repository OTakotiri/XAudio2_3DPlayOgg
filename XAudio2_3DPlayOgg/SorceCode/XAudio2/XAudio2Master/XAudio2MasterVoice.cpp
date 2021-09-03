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
// インスタンスの作成.
//=============================
CXAudio2MasterVoice* CXAudio2MasterVoice::GetInstance()
{
	static std::unique_ptr<CXAudio2MasterVoice> pInstance =
		std::make_unique<CXAudio2MasterVoice>();	// インスタンスの作成.
	return pInstance.get();
}
//=============================
// XAudioインターフェース取得.
//=============================
IXAudio2* CXAudio2MasterVoice::GetInterface()const
{
	return m_pXAudioInterface;
}
//=============================
// マスターボイス取得.
//=============================
IXAudio2MasteringVoice* CXAudio2MasterVoice::GetMasteringVoice()const
{
	return m_pMasterVoice;
}

//=============================
// 初期化 XAudio作成 マスターボイス作成.
//=============================
bool CXAudio2MasterVoice::Initialize()
{
	// COM の初期化
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) &&
		FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		_ASSERT_EXPR(false, "Failed CoInitialize");
		return false;
	}
	// XAudio2 インターフェースの作成
	if (FAILED(XAudio2Create(&m_pXAudioInterface, 0)))
	{
		_ASSERT_EXPR(false, "Create XAudio2Interface Is Failed");
		return false;
	}
	// マスターボイスの作成
	if (FAILED(m_pXAudioInterface->CreateMasteringVoice(&m_pMasterVoice)))
	{
		_ASSERT_EXPR(false, "Create XAudio2MasterVoice Is Failed");
		return FALSE;
	}
	// 以下3Dサウンド.
	XAUDIO2_VOICE_DETAILS details;
	m_pMasterVoice->GetVoiceDetails(&details);
	// サウンド出力チャンネルが指定値より多い.
	if (details.InputChannels > MAXOUTPUT_CHANNEL)
	{
		_ASSERT_EXPR(false, "OutPutChannnel is Over");
		return false;
	}
	m_SampleRate = details.InputSampleRate;
	m_InputChannels = details.InputChannels;

	// 3Dサウンドの初期化.
	X3DAudioInitialize(SPEAKER_STEREO,
		X3DAUDIO_SPEED_OF_SOUND, m_x3DHandle);
	

	return true;
}
//=============================
// 解放関数.
//=============================
void CXAudio2MasterVoice::Release()
{
	// マスターボイスが存在していなければリターン.
	if (GetInstance()->m_pMasterVoice == nullptr) return;
	// マスターボイス解放.
	GetInstance()->m_pMasterVoice->DestroyVoice();
	GetInstance()->m_pMasterVoice = nullptr;
	// XAudio2インターフェースのリリース.
	COM_SAFE_RELEASE(GetInstance()->m_pXAudioInterface);
	GetInstance()->m_pXAudioInterface = nullptr;
	// COMライブラリを閉じる
	CoUninitialize();
}