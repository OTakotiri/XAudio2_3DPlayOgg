#include "X3DAudio.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
//=================================
// �萔�錾.
namespace {
	// Lisner��Cone�ݒ�p.
	constexpr X3DAUDIO_CONE LISTENER_DIRECTIONALCONE		= { X3DAUDIO_PI*5.0f / 6.0f, X3DAUDIO_PI*11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };
	// Emitter��LFE�̃J�[�u�|�C���g�ݒ�p.
	constexpr X3DAUDIO_DISTANCE_CURVE_POINT EMITTER_LFE_CURVEP_POINTS[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	// Emitter��LFE�̃J�[�u�ݒ�p.
	constexpr X3DAUDIO_DISTANCE_CURVE EMITTER_LFE_CURVE		= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&EMITTER_LFE_CURVEP_POINTS[0], 3 };
	// Emitter��Reverb�J�[�u�|�C���g�ݒ�p.
	constexpr X3DAUDIO_DISTANCE_CURVE_POINT EMITTER_REVERB_CURVEPOINTS[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	// Emitter��Reverb�J�[�u�ݒ�p.
	constexpr X3DAUDIO_DISTANCE_CURVE EMITTER_REVERB_CURVE	= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&EMITTER_REVERB_CURVEPOINTS[0], 3 };
}

CX3DAudio::CX3DAudio(IXAudio2SourceVoice* pSource, IXAudio2SubmixVoice* pSubMix, UINT SrcChannelCount)
	: m_pSourceVoice	( pSource )
	, m_pSubmixVoice	( pSubMix )
	, m_SrcChannelCount	( SrcChannelCount )
	, m_IsPlayEnd		( false )
	, m_Emitter			( &m_InitEmitter )
	, m_Listener		( &m_InitListener )
{
	Init();
}

CX3DAudio::~CX3DAudio()
{
	Release();
}
//=============================
// Listener�\���̃|�C���^�[���Z�b�g����.
//=============================
void CX3DAudio::SetListenerData(AUDIO_LISTENER& stListenerData)
{
	m_Listener = &stListenerData;
}
//=============================
// ������̐��ʃx�N�g����ݒ�.
//=============================
void CX3DAudio::SetListenerFront(const XAudio2Vec vec)
{
	m_Listener->FrontVec.x = vec.x;
	m_Listener->FrontVec.y = vec.y;
	m_Listener->FrontVec.z = vec.z;
}
//=============================
// ������̏�����x�N�g����ݒ�.
//=============================
void CX3DAudio::SetListenerUp(const XAudio2Vec vec)
{
	m_Listener->Up.x = vec.x;
	m_Listener->Up.y = vec.y;
	m_Listener->Up.z = vec.z;
}
//=============================
// ������̃��[���h�ʒu��ݒ�.
//=============================
void CX3DAudio::SetListenerPosition(const XAudio2Vec vec)
{
	m_Listener->Pos.x = vec.x;
	m_Listener->Pos.y = vec.y;
	m_Listener->Pos.z = vec.z;
}
//=============================
// ������̉����x��ݒ�.
//=============================
void CX3DAudio::SetListenervelocity(const XAudio2Vec vec)
{
	m_Listener->Velocity.x = vec.x;
	m_Listener->Velocity.y = vec.y;
	m_Listener->Velocity.z = vec.z;
}
//=============================
// Emitter�\���̃|�C���^�[���Z�b�g����.
//=============================
void CX3DAudio::SetEmitterData(AUDIO_EMITTER& stEmitterData)
{
	m_Emitter = &stEmitterData;
}
//=============================
// �����̐��ʃx�N�g����ݒ�.
//=============================
void CX3DAudio::SetEmitterFront(const XAudio2Vec vec)
{
	m_Emitter->FrontVec.x = vec.x;
	m_Emitter->FrontVec.y = vec.y;
	m_Emitter->FrontVec.z = vec.z;
}
//=============================
// �����̏�����x�N�g����ݒ�.
//=============================
void CX3DAudio::SetEmitterUp(const XAudio2Vec vec)
{
	m_Emitter->Up.x = vec.x;
	m_Emitter->Up.y = vec.y;
	m_Emitter->Up.z = vec.z;
}
//=============================
// �����̃��[���h���W��ݒ�(3D�T�E���h�Đ���).
//=============================
void CX3DAudio::SetEmitterPosition(const XAudio2Vec vec)
{
	m_Emitter->Pos.x = vec.x;
	m_Emitter->Pos.y = vec.y;
	m_Emitter->Pos.z = vec.z;
}
//=============================
// �����̉����x��ݒ�.
//=============================
void CX3DAudio::SetEmittervelocity(const XAudio2Vec vec)
{
	m_Emitter->Velocity.x = vec.x;
	m_Emitter->Velocity.y = vec.y;
	m_Emitter->Velocity.z = vec.z;
}
//=============================
// �h�b�v���[���ʂ̋������w��.
//=============================
void CX3DAudio::SetEmitterDopplerLevel(const float value)
{
	m_Emitter->DopplerScaler = value;
}
//=============================
// �����̌��ʗ̈���w��.
//=============================
void CX3DAudio::SetEmitterRadius(const float value)
{
	m_Emitter->InnerRadius = value;
}

HRESULT CX3DAudio::SetReverb(const XAudio2Reverb::enReverbPreset nReverb)
{
	if (!m_stAudioState.IsInit) // ���������I����Ă��Ȃ���Ύ��s.
		return S_FALSE;

	if (nReverb < XAudio2Reverb::enReverbPreset::en_PRESET_FOREST || nReverb > XAudio2Reverb::enReverbPreset::Max)
		return E_FAIL;

	if (m_pSubmixVoice)
	{
		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative(&XAudio2Reverb::PRESET_PARAMS[static_cast<int>(nReverb)], &native);
		m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
	}

	return S_OK;
}

void CX3DAudio::Release()
{
	m_pSourceVoice = nullptr;
	m_pSubmixVoice = nullptr;
	m_Listener = nullptr;
	m_Emitter = nullptr;
}

HRESULT CX3DAudio::Init()
{
	
	// �R�[���ݒ�A�ʂɂȂ��Ă��悢.
	// �܂��A�G�~�b�^�[�̃R�[���͏������������ł悳��.
	m_stAudioState.Emitter.pCone = &m_stAudioState.EmitterCone;
	m_stAudioState.Emitter.pCone->InnerAngle	= 0.0f;
	// �O���̃R�[���̊p�x��0�ȊO�ɂ����.
	// �����̃R�[���̐ݒ�݂̂ŁA�|�C���g�G�~�b�^�[�̂悤�ɓ��삵�܂�.
	m_stAudioState.Emitter.pCone->OuterAngle	= 0.0f;
	// �O���̃R�[���̊p�x��0�ɂ����.
	// �G�~�b�^�[�́A�O�~���̐ݒ�݂̂��g�p�����|�C���g�G�~�b�^�[�̂悤�ɓ��삵�܂�.
	// �O���̃R�[���̐ݒ�݂̂ŁA�|�C���g�G�~�b�^�[�̂悤�ɓ��삵�܂�.
	m_stAudioState.Emitter.pCone->InnerVolume	= 0.0f;
	m_stAudioState.Emitter.pCone->OuterVolume	= 1.0f;
	m_stAudioState.Emitter.pCone->InnerLPF		= 0.0f;
	m_stAudioState.Emitter.pCone->OuterLPF		= 1.0f;
	m_stAudioState.Emitter.pCone->InnerReverb	= 0.0f;
	m_stAudioState.Emitter.pCone->OuterReverb	= 1.0f;
	// X3DAudioDefault_LinearCurve��X3DAudio.h�ɐ錾����Ă�.
	m_stAudioState.Emitter.pVolumeCurve			= (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	// LFE���x���̋����Ȑ����ALFE�ȊO�̃`�����l�������������[���I�t����悤�Ɏw�肷��.
	// �T�u�E�[�t�@�[�������ʓI�Ɏg�����Ƃ��ł��܂�.
	m_stAudioState.Emitter.pLFECurve			= (X3DAUDIO_DISTANCE_CURVE*)&EMITTER_LFE_CURVE;
	m_stAudioState.Emitter.pLPFDirectCurve		= nullptr;	// �f�t�H���g���g�p.
	m_stAudioState.Emitter.pLPFReverbCurve		= nullptr;	// �f�t�H���g���g�p.
	// ���o�[�u�Z���h���x���̋����Ȑ����w�肵�A���o�[�u�Z���h��.
	// �����ɉ����ă��o�[�u�Z���h���킸���ɑ���������A�����ɂȂ�悤�ɂ��܂�.
	// �_�C���N�g�`�����l���͋����ƂƂ��Ɍ������Ă����܂�.
	// ����̓��o�[�u�ƃ_�C���N�g�̔䗦�����߂���ʂ�����.
	// ����ɂ��A���o�[�u�ƒ��ډ��̔䗦�����܂�A����������������܂�.
	m_stAudioState.Emitter.pReverbCurve				= (X3DAUDIO_DISTANCE_CURVE*)&EMITTER_REVERB_CURVE;
	m_stAudioState.Emitter.ChannelCount				= 1;
	m_stAudioState.Emitter.ChannelRadius			= 1.0f;

	m_stAudioState.DspSettings.SrcChannelCount		= m_SrcChannelCount;		// �o�̓`���l����.
	m_stAudioState.DspSettings.DstChannelCount		= CXAudio2MasterVoice::GetGlobalSystem().m_InputChannels;
	m_stAudioState.DspSettings.pMatrixCoefficients	= m_stAudioState.MatrixCoefficients;

	m_stAudioState.IsInit = true;

	return S_OK;
}

bool CX3DAudio::Apply3DSoundValue(const bool CallOnce)
{
	if (m_pSourceVoice == nullptr) return false;

	// XAudio2�}�X�^�[�{�C�X�̃C���X�^���X�擾.
	CXAudio2MasterVoice& xAudioMaster = CXAudio2MasterVoice::GetGlobalSystem();

	
	do{
		// �R�[�����g�p����ꍇ�K�p.
		if (m_stAudioState.IsUseListenerCone)
		{
			m_stAudioState.Listener.pCone = (X3DAUDIO_CONE*)&LISTENER_DIRECTIONALCONE;
		}
		else
		{
			m_stAudioState.Listener.pCone = nullptr;
		}
		// �C���i�[���f�B�E�X���g���ꍇ�K�p.
		if (m_stAudioState.IsUseInnerRadius)
		{
			m_stAudioState.Emitter.InnerRadius = m_Emitter->InnerRadius;
			m_stAudioState.Emitter.InnerRadiusAngle = m_Emitter->InnerRadiusAngle;
		}
		else
		{
			m_stAudioState.Emitter.InnerRadius = 0.0f;
			m_stAudioState.Emitter.InnerRadiusAngle = 0.0f;
		}
		// ���ʃx�N�g���Z�b�g.
		m_stAudioState.Listener.OrientFront.x = m_Listener->FrontVec.x;
		m_stAudioState.Listener.OrientFront.y = m_Listener->FrontVec.y;
		m_stAudioState.Listener.OrientFront.z = m_Listener->FrontVec.z;
		// ��x�N�g���Z�b�g.
		m_stAudioState.Listener.OrientTop.x = m_Listener->Up.x;
		m_stAudioState.Listener.OrientTop.y = m_Listener->Up.y;
		m_stAudioState.Listener.OrientTop.z = m_Listener->Up.z;
		// ���W�Z�b�g.
		m_stAudioState.Listener.Position.x = m_Listener->Pos.x;
		m_stAudioState.Listener.Position.y = m_Listener->Pos.y;
		m_stAudioState.Listener.Position.z = m_Listener->Pos.z;
		// �����x�v�Z.
		// XAudio2Vec Pos = { m_Listener.Pos.x ,m_Listener.Pos.y,m_Listener.Pos.z };
		// XAudio2Vec Pos2 = { listener.Position.x ,listener.Position.y,listener.Position.z };
		// m_Listener.Velocity = (Pos2 - Pos) / fElapsedTime;
		m_stAudioState.Listener.Velocity.x = m_Listener->Velocity.x;
		m_stAudioState.Listener.Velocity.y = m_Listener->Velocity.y;
		m_stAudioState.Listener.Velocity.z = m_Listener->Velocity.z;
		
		// �G�~�b�^�[�ɏ����Z�b�g.
		// ���ʃx�N�g���Z�b�g.
		m_stAudioState.Emitter.OrientFront.x = m_Emitter->FrontVec.x;
		m_stAudioState.Emitter.OrientFront.y = m_Emitter->FrontVec.y;
		m_stAudioState.Emitter.OrientFront.z = m_Emitter->FrontVec.z;
		// ��x�N�g���Z�b�g.
		m_stAudioState.Emitter.OrientTop.x = m_Emitter->Up.x;
		m_stAudioState.Emitter.OrientTop.y = m_Emitter->Up.y;
		m_stAudioState.Emitter.OrientTop.z = m_Emitter->Up.z;
		// ���W�Z�b�g.
		m_stAudioState.Emitter.Position.x = m_Emitter->Pos.x;
		m_stAudioState.Emitter.Position.y = m_Emitter->Pos.y;
		m_stAudioState.Emitter.Position.z = m_Emitter->Pos.z;
			
		// �����x�v�Z.
		// XAudio2Vec Pos = { m_Emitter.Pos.x ,m_Emitter.Pos.y,m_Emitter.Pos.z };
		// XAudio2Vec Pos2 = { m_stAudioState.Emitter.Position.x ,m_stAudioState.Emitter.Position.y,m_stAudioState.Emitter.Position.z };
		// m_Emitter.Velocity = (Pos2 - Pos) / fElapsedTime;
		m_stAudioState.Emitter.Velocity.x = m_Emitter->Velocity.x;
		m_stAudioState.Emitter.Velocity.y = m_Emitter->Velocity.y;
		m_stAudioState.Emitter.Velocity.z = m_Emitter->Velocity.z;
		// �͈�.
		m_stAudioState.Emitter.CurveDistanceScaler = m_Emitter->CurveDistanceScaler;
		
		// 3D�v�Z.
		X3DAudioCalculate(xAudioMaster.m_x3DHandle,
			&m_stAudioState.Listener,
			&m_stAudioState.Emitter,
			X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
			| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
			| X3DAUDIO_CALCULATE_REVERB,
			&m_stAudioState.DspSettings);	// out.

		if (m_pSourceVoice != nullptr && m_IsPlayEnd == false)
		{
			// Submix�����ĂԂƁA3D���K������ĂȂ�����ɂȂ�.

			// �h�b�v���[���ʂ��g�p����Ȃ�K��.
			if (m_stAudioState.IsUseDoppler == true) m_pSourceVoice->SetFrequencyRatio(m_stAudioState.DspSettings.DopplerFactor);
			m_pSourceVoice->SetOutputMatrix(xAudioMaster.GetMasteringVoice(), 1, xAudioMaster.m_InputChannels, m_stAudioState.MatrixCoefficients);
			m_pSourceVoice->SetOutputMatrix(m_pSubmixVoice, 1, 1, &m_stAudioState.DspSettings.ReverbLevel);
			// XAUDIO2_FILTER_PARAMETERS.Frequency�Ŏg�p����Ă���t�B���^�̃J�b�g�I�t���g�����A�w���c�ŕ\�����ꂽ���̂��烉�W�A���ɕϊ����܂�.
			// XAUDIO2_FILTER_PARAMETERS.Frequency�Ŏg�p�������g���l�A��ԕϐ�.
			// �t�B���^�[�^�C�v�̂݁B 1 �|�[���̃t�B���^�[�^�C�v�ɂ́AXAudio2CutoffFrequencyToOnePoleCoefficient() ���g�p���Ă�������.
			// �T�|�[�g�����ō���CutoffFrequency��SampleRate/6�ł��邱�Ƃɒ���.
			// CutoffFrequency�̒l���傫���ƁAXAUDIO2_MAX_FILTER_FREQUENCY���Ԃ����.
			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_stAudioState.DspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			m_pSourceVoice->SetOutputFilterParameters(xAudioMaster.GetMasteringVoice(), &FilterParametersDirect);
			// see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_stAudioState.DspSettings.LPFReverbCoefficient), 1.0f };
			m_pSourceVoice->SetOutputFilterParameters(m_pSubmixVoice, &FilterParametersReverb);
		}
		else {
			return false;
		}
	} while (CallOnce == false && !m_IsPlayEnd);

	return true;
}
