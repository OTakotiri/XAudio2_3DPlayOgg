#include "XAudio2PlayBGM.h"
#include "XAudio2.h"
#include <x3daudio.h>
#include <vector>
#include <process.h>
#include <thread>
#include "..\SoundManager.h"
#include "..\..\Common\FPSCounter\FPSCounter.h"
//=============================
// �萔�錾.
//=============================
namespace {
	constexpr float MAX_PITCH = 5.0f;			// �ő�s�b�`(�ō�5.0f).
	constexpr float INIT_FADE_VALUE = -100.0f;	// �t�F�[�h����l��1�񂾂��v�Z���邽�߂̏������l.
	constexpr int DIVISION_FADE_VALUE = 3000;	// �t�F�[�h����l���v�Z���邽�߂̊���Z�̒l.
}

CXAudio2PlayBGM::CXAudio2PlayBGM()
	: m_pSourceVoice			( nullptr )
	, m_pSubmixVoice			( nullptr )
	, m_pOggWavData				( nullptr )
	, m_fMaxPitch				( MAX_PITCH )
	, m_fPitch					( 1.0f )
	, m_fFadeInValue			( -100.0f )
	, m_fFadeOutValue			( -100.0f )
	, m_isMoveFadeInThread		( false )
	, m_isMoveFadeOutThread		( false )
	, m_bFadeInStart			( false )
	, m_bFadeOutStart			( false )
	, m_bFirstPlay				( true )
	, m_SourceFileName			( "" )
{
	m_Buffer	= { 0 };
	m_Buff_len	= 0;
	m_Buff		= nullptr;
	m_Len		= 0;
	m_Buff_cnt	= 0;
	m_Size		= 0;
}

CXAudio2PlayBGM::~CXAudio2PlayBGM()
{
	if (m_FadeInThread.joinable() == true) m_FadeInThread.join();
	if (m_FadeOutThread.joinable() == true) m_FadeOutThread.join();
}
HRESULT CXAudio2PlayBGM::InitSourceVoice(const char * filename)
{
	m_SourceFileName = filename;
	// WavLoad�N���X�쐬.
	m_pOggWavData = std::make_shared<COggLoad>();
	// Wav�t�H�[�}�b�g�擾.
	if (FAILED(m_pOggWavData->CreateOggData(m_SourceFileName))) return E_FAIL;
	if (FAILED(CreateOggSound())) return E_FAIL;

	return S_OK;
}
//=============================
// �����f�[�^���X�g���[���ɗ�������.
//=============================
HRESULT CXAudio2PlayBGM::OggSubmit()
{
	HRESULT ret;
	// �I�[�f�B�I�o�b�t�@�[��p��
	// �X�g���[���Đ�����̂Ńo�b�t�@��2����.
	m_Buff_len = 2;
	m_Buff = new BYTE*[m_Buff_len];
	m_Len = m_pOggWavData->GetFormat()->nAvgBytesPerSec;

	for (int i = 0; i < m_Buff_len; i++) {
		m_Buff[i] = new BYTE[m_Len];
	}

	// �ŏ��̃o�b�t�@�[�փf�[�^��ǂݍ���
	ret = m_pOggWavData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);
	if (FAILED(ret))
	{
		_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", ret);
		return E_FAIL;
	}

	m_Buffer.AudioBytes = m_Size;
	m_Buffer.pAudioData = m_Buff[m_Buff_cnt];
	if (0 < m_Size)
	{
		ret = m_pSourceVoice->SubmitSourceBuffer(&m_Buffer);
		if (FAILED(ret)) {
			_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", ret);
			return E_FAIL;
		}
	}

	if (m_Buff_len <= ++m_Buff_cnt) m_Buff_cnt = 0;

	return S_OK;
}
//=============================
// �t�F�[�h�A�E�g�p�̃X���b�h���N������.
//=============================
void CXAudio2PlayBGM::CallFadeOutBGM(bool & isEnd)
{
	if (m_bFadeOutStart == true) {
		if (m_isMoveFadeOutThread == true) return;
		m_isMoveFadeOutThread = true;
		auto BGM = [&]()
		{
			// 1�x�����t�F�[�h����l���v�Z.
			if (m_fFadeOutValue <= INIT_FADE_VALUE) {
				m_fFadeOutValue = GetVolume() / DIVISION_FADE_VALUE;
			}
			FadeOutBGM(-m_fFadeOutValue, isEnd);
			m_bFadeOutStart = false;
			m_fFadeOutValue = INIT_FADE_VALUE;
			m_isMoveFadeOutThread = false;
		};
		m_FadeOutThread = std::thread(BGM);
		m_FadeOutThread.detach();
	}
}
//=============================
// �t�F�[�h�C���p�̃X���b�h���N������.
//=============================
void CXAudio2PlayBGM::CallFadeInBGM(bool& isEnd)
{
	if (m_bFadeInStart == true) {
		if (m_isMoveFadeInThread == true) return;
		m_isMoveFadeInThread = true;
		auto BGM = [&]()	// �����_�֐�.
		{
			// 1�x�����t�F�[�h����l���v�Z.
			if (m_fFadeInValue <= INIT_FADE_VALUE) {
				m_fFadeInValue = CSoundManager::GetInstance()->m_stSound.BGMVolume * CSoundManager::GetInstance()->m_stSound.MasterVolume / DIVISION_FADE_VALUE;
			}
			FadeInBGM(m_fFadeInValue, isEnd); // �t�F�[�h�C���֐�.
			m_bFadeInStart = false;					// �����ɗ�����t�F�[�h�C�����I����Ă�̂Ńt���O������.
			m_fFadeInValue = INIT_FADE_VALUE;
			m_isMoveFadeInThread = false;		// �X���b�h���.
		};
		m_FadeInThread = std::thread(BGM);
		m_FadeInThread.detach();
	}

}

//=============================
// �Đ������ǂ������擾����֐�.
//=============================
const bool CXAudio2PlayBGM::IsPlaying()
{
	// SoundSource���Ȃ��ꍇ���^�[�� 0(�Đ����Ă��Ȃ��Ƃ��Ɠ���).
	if (m_pSourceVoice == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	// SoundSource�̃X�e�[�g�擾.
	m_pSourceVoice->GetState(&xState);
	// 0����Ȃ���΍Đ���(true)��Ԃ�.
	return xState.BuffersQueued != 0;
}
//=============================
// �X���b�h�������BGM���Đ�����.
//=============================
void CXAudio2PlayBGM::PlayBGM(const bool LoopFlag)
{
	if (m_pSourceVoice == nullptr) return;

	// �X���b�h�ɓ���郉���_�֐�.
	auto BGM = [&]()
	{
		Play(m_IsEndPlay, LoopFlag);	// �Đ��֐�.
		StopBGMThread();
	};

	DWORD ThreadExitCode = -1;
	// �w�肵��BGM�̖��O�̃X���b�h����n���h��ID�擾.
	GetExitCodeThread(m_pBGMThread.native_handle(), &ThreadExitCode);
	// 0xFFFFFFFF���A���Ă�����X���b�h�������Ă��Ȃ��̂œ�����.
	if (ThreadExitCode == 0xFFFFFFFF) {
		m_IsEndPlay = false;
		m_pBGMThread = std::thread(BGM);
	}
}
//=============================
// �Đ�.
//=============================
bool CXAudio2PlayBGM::Play(bool& isEnd, const bool IsLoop)
{
	// ���b�N.
	std::unique_lock<std::mutex> lock(m_mtx);
	HRESULT ret;
	if (m_pSourceVoice == nullptr) return false;
	// �s�b�`�ݒ�.
	SetPitch(m_fPitch);
	if (CSoundManager::GetInstance()->m_stSound.BGMVolume == GetVolume()) SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
	// SoundSource���Đ�.
	m_pSourceVoice->Start();
	// Submit�͏���Đ����̂ݎ��s����.
	if (m_bFirstPlay == true) {
		OggSubmit();
		m_bFirstPlay = false;
	}

	// �Đ����[�v
	do {
		if (isEnd) break;
		// �t�@�C������f�[�^��ǂݎ��
		m_pOggWavData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);
		// �Ō�܂œǂݐ؂���.
		if (m_Size <= 0) {
			// �I��(��~)�t���O�������Ă��Ȃ� �� ���[�v�t���O�������Ă��Ȃ�.
			if (!IsLoop) break;
			if (!isEnd) {
				Sleep(20);
				//�f�[�^�̓ǂݎ��|�C���^�����Z�b�g.
				m_pOggWavData->ResetFile();
				m_pOggWavData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);// �`�����N�ǂݍ���.
			}
			else {
				// �I���̂���while�𔲂���.
				break;
			}
		}
		// �o�b�t�@�[����
		m_Buffer.AudioBytes = m_Size;
		m_Buffer.pAudioData = m_Buff[m_Buff_cnt];
		// �o�b�t�@�[���L���[�ɒǉ�
		ret = m_pSourceVoice->SubmitSourceBuffer(&m_Buffer);
		if (FAILED(ret)) {
			_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", false);
			return true;
		}
		// 2�m�ۂ��Ă���o�b�t�@�[�����݂Ɏg���悤�ɃJ�E���^�`�F�b�N�ƃ��Z�b�g
		if (m_Buff_len <= ++m_Buff_cnt) m_Buff_cnt = 0;
		// �t�F�[�h�C���t���O�������Ă���΁A�t�F�[�h�C���֐����ĂԃX���b�h�𗧂Ă�.
		CallFadeInBGM(isEnd);
		// �t�F�[�h�A�E�g�t���O�������Ă���΁A�t�F�[�h�A�E�g�֐����ĂԃX���b�h�𗧂Ă�.
		CallFadeOutBGM(isEnd);
	} while (WaitForSingleObject(m_Callback.event, INFINITE) == WAIT_OBJECT_0);	// �o�b�t�@�I���܂�while���[�v.
	if (m_pSourceVoice == nullptr) return true;

	XAUDIO2_VOICE_STATE xState;
	m_pSourceVoice->GetState(&xState);
	// �Đ���Ԃ��~���܂Ń��[�v.
	while (IsPlaying() == true) {
		Stop();
		xState.BuffersQueued = 0;
	}
	m_pOggWavData->ResetFile(); //�f�[�^�̓ǂݎ��|�C���^�����Z�b�g.
	return true;
}
bool CXAudio2PlayBGM::StopBGMThread()
{
	// �Đ����ɐݒ肵���t���O��true�ɂ��āABGM���~�Ɍ����킹��.
	m_IsEndPlay = true;
	DWORD ThreadExitCode = -1;
	// �X���b�h����~�������ǂ�����ID�擾�Ŏ擾.
	GetExitCodeThread(m_pBGMThread.native_handle(), &ThreadExitCode);
	// �Ԃ��ė���l��4294967295�̏ꍇ�A���������X���b�h�������オ���Ă��Ȃ�(�Đ����Ă��Ȃ�)�̂Ń��^�[��.
	if (ThreadExitCode == 0xFFFFFFFF) return true;
	// ��~���Ă�����0���Ԃ��ė���.
	if (ThreadExitCode == 0) {
		return true;
	}
	return false;
}
//=============================
// �T�E���h���S��~.
//=============================
bool CXAudio2PlayBGM::Stop()
{
	if (m_pSourceVoice == nullptr) return true;
	m_pSourceVoice->Stop(0);
	// �o�b�t�@�t���b�V��.
	m_pSourceVoice->FlushSourceBuffers();
	return true;
}
//=============================
// �T�E���h�ꎞ��~.
//=============================
bool CXAudio2PlayBGM::Pause()
{
	if (m_pSourceVoice == nullptr) return true;
	m_pSourceVoice->Stop(0);
	return true;
}
//=============================
// ��~���̃T�E���h���ēx�Đ�.
//=============================
bool CXAudio2PlayBGM::PlayStart()
{
	if (m_pSourceVoice == nullptr) return true;
	if (IsPlaying() == false)  return true;

	m_pSourceVoice->Start();
	return true;
}
//=============================
//  �w�b�h�Z�b�g��X�s�[�J�[����o�͂���鉹�̃o�����X��ύX����.
//  [-1.0] ���̂� [ 0.0] ���� [+1.0] �E�̂�   
//  3D�T�E���h���g�p����ꍇ�͖���.
//=============================
void CXAudio2PlayBGM::SetAudioPan(float value)
{
	if (m_pSourceVoice == nullptr) return;
	if (value > 1.0f)value	= 1.0f;
	if (value < -1.0f)value = -1.0f;

	XAUDIO2_VOICE_DETAILS details;
	if (m_pSourceVoice)
		m_pSourceVoice->GetVoiceDetails(&details);

	int sz = details.InputChannels * 2;

	float *matrix = new float[sz];
	float halfPan = value / 2.0f;
	for (int i = 0; i < sz; ++i)
	{
		if (i % 2 == 0)
			matrix[i] = 0.5f - halfPan;
		else if (i % 2 == 1)
			matrix[i] = 0.5f + halfPan;
	}
	if (m_pSourceVoice)
		m_pSourceVoice->SetOutputMatrix(nullptr, details.InputChannels, 2, matrix);
	delete[] matrix;
	return;

}
//=============================
// Ogg����\�[�X�{�C�X���쐬����.
//=============================
HRESULT CXAudio2PlayBGM::CreateOggSound()
{
	//���f�[�^������Ă����烊�^�[��.
	if (m_pOggWavData == nullptr) return E_FAIL;
	HRESULT hr;
	// XAudio2�}�X�^�[�{�C�X�̃C���X�^���X�擾.
	CXAudio2MasterVoice& xAudioMaster = CXAudio2MasterVoice::GetGlobalSystem();
	// XAudio2�̃n���h���擾.
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
	// �T�u�~�b�N�X�{�C�X�̍쐬.
	if (FAILED(hr = pHandle->CreateSubmixVoice(&m_pSubmixVoice, 1,
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
	m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
	// IUnknown�̔j��.
	// �����Ŕj�����邱�ƂŁAIUnknown�̎Q�ƃJ�E���^��1�ɂȂ�A���L�����\�[�X�{�C�X�����H.
	pXAPO->Release();

	// �ʂ̃{�C�X����̑��M�̃^�[�Q�b�g�ł��鈶��{�C�X���\�[�X�ɃZ�b�g����.
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	// ���Ŏw�肵�����M��Ƀt�B���^�[���g�p���� 0 or XAUDIO2_SEND_USEFILTER.
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path.
	sendDescriptors[0].pOutputVoice = xAudioMaster.GetMasteringVoice();
	// ���Ŏw�肵�����M��Ƀt�B���^�[���g�p���� 0 or XAUDIO2_SEND_USEFILTER.
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path �ȗ����邱�ƂŁA��胊�A���ȃI�N���[�W�������]���ɂ��ăp�t�H�[�}���X�����コ���邱�Ƃ��ł��܂�.
	sendDescriptors[1].pOutputVoice = m_pSubmixVoice;
	// ���X�g��p�ӁASEND_DESCRIPTOR�̐�,�ϐ�.
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };
	// �\�[�X�{�C�X���쐬����.
	if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice,
		m_pOggWavData->GetFormat(), 0, m_fMaxPitch, &m_Callback, &sendList)))
	{
		// ���s����.
		_ASSERT_EXPR(L"error %#X creating bgm source voice\n", hr);
		return E_FAIL;
	}

	// ��ł������ꍇ�AChain�����Z�b�g.
	// m_pSourceVoice->SetEffectChain(&chain);
	return S_OK;
}
//=============================
// ���o�[�u��ݒ�.
//=============================
HRESULT CXAudio2PlayBGM::SetReverb(const XAudio2Reverb::enReverbPreset Preset)
{
	if (Preset < XAudio2Reverb::enReverbPreset::en_PRESET_FOREST || Preset > XAudio2Reverb::enReverbPreset::Max)
		return E_FAIL;

	if (m_pSubmixVoice)
	{
		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative(&XAudio2Reverb::PRESET_PARAMS[static_cast<int>(Preset)], &native);
		m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
	}

	return S_OK;
}
//=============================
// ���ʎ擾.
//=============================
const float CXAudio2PlayBGM::GetVolume()
{
	if (m_pSourceVoice == nullptr) return 0.0f;
	float Volume = 0.0f;

	m_pSourceVoice->GetVolume(&Volume);

	return Volume;
}
//=============================
// ���ʐݒ�.
//=============================
bool  CXAudio2PlayBGM::SetBGMVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	if (value >= CSoundManager::GetBGMVolume()) {
		value = CSoundManager::GetBGMVolume();
	}

	m_pSourceVoice->SetVolume(value * CSoundManager::GetInstance()->m_stSound.MasterVolume, 0);
	return true;
}
//=============================
// ���݂̉��ʂ�����Z�܂��͌��Z.
//=============================
bool CXAudio2PlayBGM::AdjustVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	float Calc = GetVolume() + value;

	m_pSourceVoice->SetVolume(Calc, 0);

	return true;
}
//=============================
// �s�b�`��ݒ�.
//=============================
bool CXAudio2PlayBGM::SetPitch(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	m_pSourceVoice->SetFrequencyRatio(max(min(value, m_fMaxPitch), XAUDIO2_MIN_FREQ_RATIO));

	return true;
}
//=============================
// �s�b�`���擾.
//=============================
const float CXAudio2PlayBGM::GetPitch()
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (m_pSourceVoice == nullptr) return pitch;

	m_pSourceVoice->GetFrequencyRatio(&pitch);

	return pitch;
}
//=============================
// �ő�s�b�`�ݒ�.
//=============================
void  CXAudio2PlayBGM::SetMaxPitch(float value)
{
	m_fMaxPitch = max(min(value, 2.0f), XAUDIO2_MIN_FREQ_RATIO);
}
//=============================
// �������.
//=============================
void CXAudio2PlayBGM::Release()
{
	// true�ɂȂ�܂ŌĂ�.
	while (StopBGMThread() == false);
	DestoroySource();
	m_pOggWavData->Close();
}
//=============================
// BGM���t�F�[�h�A�E�g����֐�.
// ���̊֐��͕ʃX���b�h����Ăяo��.
//=============================
bool CXAudio2PlayBGM::FadeOutBGM(float value, const bool& isGameEnd)
{
	// ���ʂ�0���傫���ԃ��[�v.
	while (GetVolume() > 0.0f)
	{
		// �I���t���O�������Ă���ΏI��.
		if (isGameEnd == true) return true;
		// �t�F�[�h�A�E�g�t���O�����Ă���ΏI��.
		if (m_bFadeOutStart == false) return true;
		// ���ʒ���.
		AdjustVolume(value);
		Sleep(1);
	}
	SetBGMVolume(0.0f);
	return true;
}
//=============================
// BGM���t�F�[�h�C������֐�.
// ���̊֐��͕ʃX���b�h����Ăяo��.
//=============================
bool CXAudio2PlayBGM::FadeInBGM(float value, const bool& isGameEnd)
{
	// ���ʂ��A�ő�BGM���� * �ő�Master���ʂ�菬�����ԃ��[�v.
	while (GetVolume() < CSoundManager::GetInstance()->m_stSound.BGMVolume * CSoundManager::GetInstance()->m_stSound.MasterVolume)
	{
		// �I���t���O�������Ă���ΏI��.
		if (isGameEnd == true) return true;
		// �t�F�[�h�C���t���O�����Ă���ΏI��.
		if (m_bFadeInStart == false) return true;
		// ���ʒ���.
		AdjustVolume(value);
		Sleep(1);
	}
	SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);

	return true;
}
//=============================
// �\�[�X�{�C�X�̉��.
//=============================
void CXAudio2PlayBGM::DestoroySource()
{
	if (m_pSourceVoice != nullptr) {
		m_pSourceVoice->DestroyVoice();	// �\�[�X�{�C�X�̉��.
		m_pSourceVoice = nullptr;
	}
	else {
		// �\�[�X�{�C�X�����݂��Ȃ��̂Ń��^�[��.
		return;
	}

	// �o�b�t�@�����[�X.
	for (int i = 0; i < m_Buff_len; i++) {
		SAFE_DELETE(m_Buff[i]);
	}
	SAFE_DELETE(m_Buff);
}
