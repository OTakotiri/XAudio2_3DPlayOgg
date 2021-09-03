#include "XAudio2PlaySE.h"
#include "XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include <vector>
#include <process.h>
#include <thread>

#include "..\SoundManager.h"

CXAudio2PlaySE::CXAudio2PlaySE()
	: m_pOggWavData				( nullptr )
	, m_CheckNonePlaySeVoice	( 0 )
	, m_RepeatPlaySe			( 0 )
	, m_bisAnotherVolume		( false )
	, m_fSEVolume				( 1.0f )
	, m_fAnotherVolume			( 0.2f )
	, m_SourceFileName			( "" )
{
	m_pSourceVoice[0] = nullptr;
}

CXAudio2PlaySE::~CXAudio2PlaySE()
{
}
HRESULT CXAudio2PlaySE::InitSourceVoice(const char * filename)
{
	m_SourceFileName = filename;
	// WavLoad�N���X�쐬.
	m_pOggWavData = std::make_shared<COggLoad>();
	// Wav�t�H�[�}�b�g�擾.
	if (FAILED(m_pOggWavData->CreateOggData(m_SourceFileName, true))) return E_FAIL;
	if (FAILED(CreateOggSound())) return E_FAIL;
	return S_OK;
}
//=============================
// �������.
//=============================
void CXAudio2PlaySE::Release()
{
	m_pOggWavData->Close();
	DestoroySource();
}
//=============================
// �Đ�����Ă��Ȃ��\�[�X�{�C�X��T��.
//=============================
int CXAudio2PlaySE::CheckPlaySeBuffer()
{
	//XAUDIO2_VOICE_STATE state;
	// �Đ����Ă��Ȃ�SEVoice��T��.
	for (int Array = 0; Array < SE_VOICE_SIZE; Array++) {
		//m_pSourceVoice[Array]->GetState(&state);
		if (!IsPlayingSE(Array)) return Array;	// �Đ�����Ă��Ȃ��z��̒l��Ԃ�.
	}
	// ���ׂăT�E���h�Đ��r���Ȃ̂ŁA���s�[�g�Đ�����.
	if (m_RepeatPlaySe < SE_VOICE_SIZE) m_RepeatPlaySe++;
	// �ő吔�Ɠ����ɂȂ�����0�ɂ���.
	if (m_RepeatPlaySe == SE_VOICE_SIZE) m_RepeatPlaySe = 0;


	return m_RepeatPlaySe;
}
//=============================
// �w�肳�ꂽ�z��ԍ���SE���Đ�����Ă��邩.
//=============================
const bool CXAudio2PlaySE::IsPlayingSE(size_t ArrayNum)
{
	// SoundSource���Ȃ��ꍇ���^�[�� 0(�Đ����Ă��Ȃ��Ƃ��Ɠ���).
	if (m_pSourceVoice[ArrayNum] == nullptr) return false;
	XAUDIO2_VOICE_STATE xState;
	// SoundSource�̃X�e�[�g�擾.
	m_pSourceVoice[ArrayNum]->GetState(&xState);

	return xState.BuffersQueued != 0;
}
//=============================
// �T�E���h�Đ�.
//=============================
bool CXAudio2PlaySE::Play()
{
	HRESULT hr;
	int Array = 0;
	if (m_pSourceVoice[0] == nullptr) return false;
	if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;

	Array = CheckPlaySeBuffer(); // �Đ�����Ă��Ȃ��\�[�X�{�C�X�̔z��ԍ����擾.
	// �ʂ�SE�̉��ʂ��Z�b�g����t���O�������Ă��鎞.
	if (m_bisAnotherVolume == true) SetSEVolume(m_fAnotherVolume, Array);	// SE�ɑS�̂Ƃ͕ʂ̉��ʃZ�b�g.
	else SetSEVolume(m_fSEVolume, Array);	// SE�̉��ʃZ�b�g.

	m_pSourceVoice[Array]->Stop();			// �\�[�X�{�C�X��~.
	m_pSourceVoice[Array]->FlushSourceBuffers();// �\�[�X�o�b�t�@���N���A.
	m_pSourceVoice[Array]->Start();			// �\�[�X�{�C�X�Đ�.

	// �o�b�t�@�[�𐶐�����
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = m_pOggWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = m_pOggWavData->GetSEFileSize();

	// �L���[�Ƀo�b�t�@�[�𓊓�����
	if (FAILED(hr = m_pSourceVoice[Array]->SubmitSourceBuffer(&seBuffer)))
	{
		return false;
	}

	return true;
}
//=============================
// SE�𑽏d�Đ����Ȃ��ōĐ�.
// #���Ƃ���Ȃ��Ă���SE���~�߂Ĉ�ɂ�����̂ł͂Ȃ�.
//=============================
bool CXAudio2PlaySE::NoMultiplePlay()
{
	HRESULT hr;

	if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;

	// ����SE���Đ�����Ă��Ȃ���Β��ɓ���Đ�����.
	if (IsPlayingSE(0) == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_pSourceVoice[0]->Start();	// �\�[�X�{�C�X�Đ�.
	}
	if (m_bisAnotherVolume == true) SetSEVolume(m_fAnotherVolume, 0);	// SE�ɑS�̂Ƃ͕ʂ̉��ʃZ�b�g.
	else SetSEVolume(m_fSEVolume, 0);		// SE�̉��ʃZ�b�g.

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// �o�b�t�@��0���傫��.
	{
		m_pSourceVoice[0]->Stop();		// �\�[�X�{�C�X��~.
		m_pSourceVoice[0]->FlushSourceBuffers();// �\�[�X�o�b�t�@���N���A.
		m_pSourceVoice[0]->Start();		// �\�[�X�{�C�X�Đ�.
	}

	// �o�b�t�@�[�𐶐�����
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = m_pOggWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = m_pOggWavData->GetSEFileSize();

	// �L���[�Ƀo�b�t�@�[�𓊓�����
	if (FAILED(hr = m_pSourceVoice[0]->SubmitSourceBuffer(&seBuffer)))
	{
		return false;
	}

	return true;
}
//=============================
// SE��~.
//=============================
bool CXAudio2PlaySE::SeStop(const size_t ArrayNum)
{
	if (m_pSourceVoice[ArrayNum] == nullptr) return true;

	m_pSourceVoice[ArrayNum]->Stop(0);
	m_pSourceVoice[ArrayNum]->FlushSourceBuffers();
	return true;
}
//=============================
// �쐬���Ă�\���܂ޑS�Ă�SE��~.
//=============================
bool CXAudio2PlaySE::AllSeStop()
{
	if (m_pSourceVoice[0] == nullptr) return true;

	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array]->Stop(0);
		m_pSourceVoice[Array]->FlushSourceBuffers();
	}

	return true;
}
//=============================
// Ogg�f�[�^�쐬.
//=============================
HRESULT CXAudio2PlaySE::CreateOggSound()
{
	//���f�[�^������Ă����烊�^�[��.
	if (m_pOggWavData == nullptr) return E_FAIL;
	HRESULT hr;
	// XAudio2�}�X�^�[�{�C�X�̃C���X�^���X�擾.
	CXAudio2MasterVoice& xAudioMaster = CXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();
	// ���o�[�u�p��IUnknown�p��.
	IUnknown* pXAPO;
	// ���o�[�u�쐬.
	XAudio2CreateReverb(&pXAPO);

	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.pEffect = pXAPO;
	desc.InitialState = TRUE;			// �G�t�F�N�g��L����Ԃŏ��������邩�ǂ���(EnableEffect(0)���Ă΂Ȃ��Ă�����).
	desc.OutputChannels = 1;			// �G�t�F�N�g����������o�̓`�����l���̐�.
	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;				// �{�C�X�̃G�t�F�N�g�`�F�[�����̃G�t�F�N�g�̐�����̓��o�[�u�̂�.
	chain.pEffectDescriptors = &desc;	// XAUDIO2_EFFECT_DESCRIPTOR.

	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {

		m_pSubmixVoice[Array] = nullptr;
		m_pSubmixVoice[Array] = { 0 };
		// �T�u�~�b�N�X�{�C�X�̍쐬.
		if (FAILED(hr = pHandle->CreateSubmixVoice(&m_pSubmixVoice[Array], 1,
			xAudioMaster.m_SampleRate, 0, 0,
			nullptr, &chain)))
		{
			_ASSERT_EXPR(false, L"Failed Create SubMixVoice");
			return hr;
		}
		// �f�t�H���g�G�t�F�N�g�p�����[�^���Z�b�g����.
		XAUDIO2FX_REVERB_PARAMETERS native;
		XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
		ReverbConvertI3DL2ToNative(&PRESET_PARAMS, &native);
		m_pSubmixVoice[Array]->SetEffectParameters(0, &native, sizeof(native));
		// IUnknown�̔j��.
		// �����Ŕj�����邱�ƂŁAIUnknown�̎Q�ƃJ�E���^��1�ɂȂ�A���L�����\�[�X�{�C�X�����H.

		// �ʂ̃{�C�X����̑��M�̃^�[�Q�b�g�ł��鈶��{�C�X���\�[�X�ɃZ�b�g����.
		XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
		// ���Ŏw�肵�����M��Ƀt�B���^�[���g�p���� 0 or XAUDIO2_SEND_USEFILTER.
		sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path.
		sendDescriptors[0].pOutputVoice = xAudioMaster.GetMasteringVoice();
		// ���Ŏw�肵�����M��Ƀt�B���^�[���g�p���� 0 or XAUDIO2_SEND_USEFILTER.
		sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path �ȗ����邱�ƂŁA��胊�A���ȃI�N���[�W�������]���ɂ��ăp�t�H�[�}���X�����コ���邱�Ƃ��ł��܂�.
		sendDescriptors[1].pOutputVoice = m_pSubmixVoice[Array];
		// ���X�g��p�ӁASEND_DESCRIPTOR�̐�,�ϐ�.
		const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };


		if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice[Array], m_pOggWavData->GetFormat())))
		{
			// ���s����.
			_ASSERT_EXPR("error %#X creating se source voice\n", hr);
			return E_FAIL;
		}
		m_pX3DAudio[Array] = std::make_unique<CX3DAudio>(m_pSourceVoice[Array], m_pSubmixVoice[Array], 2);
		if (FAILED(hr))
			return E_FAIL;
	}
	pXAPO->Release();

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}
//=============================
// ������ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
//=============================
void CXAudio2PlaySE::SetListenerInfo(CX3DAudio::AUDIO_LISTENER& stListenerData)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetListenerData(stListenerData);
	}
}
//=============================
// �����ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
//=============================
void CXAudio2PlaySE::SetEmitterInfo(CX3DAudio::AUDIO_EMITTER& stEmitterData)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetEmitterData(stEmitterData);
	}
}
//=============================
// ���o�[�u��ݒ�.
//=============================
HRESULT CXAudio2PlaySE::SetReverb(const XAudio2Reverb::enReverbPreset Preset)
{
	if (m_pX3DAudio[0] == nullptr) return E_FAIL;
	HRESULT hr;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		hr = m_pX3DAudio[Array]->SetReverb(Preset);
	}
	return hr;
}
//=============================
// ���X�i�[�̃R�[�����g�p���邩�̃t���O���Z�b�g����.
//=============================
void CXAudio2PlaySE::SetUseListenerConeFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetUseListenerConeFlag(isFlag);
	}
}
//=============================
// InnerRadius���g�p���邩�̃t���O���Z�b�g����.
//=============================
void CXAudio2PlaySE::SetUseInnerRadiusFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		if (m_pX3DAudio != nullptr) m_pX3DAudio[Array]->SetUseInnerRadiusFlag(isFlag);
	}
}
//=============================
// �h�b�v���[���ʂ��g�p���邩�̃t���O���Z�b�g����.
//=============================
void CXAudio2PlaySE::SetUseDopplerFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		if (m_pX3DAudio != nullptr) m_pX3DAudio[Array]->SetUseDopplerFlag(isFlag);
	}
}

//=============================
// ���ʎ擾.
//=============================
const float CXAudio2PlaySE::GetSEVolume(size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return 0.0f;
	float Volume = 0.0f;

	m_pSourceVoice[Array]->GetVolume(&Volume);

	return Volume;
}
//=============================
// SE���ʐݒ�.
//=============================
bool CXAudio2PlaySE::SetSEVolume(float value, size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return false;

	m_pSourceVoice[Array]->SetVolume(value * CSoundManager::GetInstance()->m_stSound.MasterVolume, 0);
	return true;
}
//=============================
// �\�[�X�{�C�X���.
//=============================
void CXAudio2PlaySE::DestoroySource()
{
	if (m_pSourceVoice[0] != nullptr) {
		for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
			m_pSourceVoice[Array]->DestroyVoice();
			m_pSourceVoice[Array] = nullptr;
		}
	}
}
