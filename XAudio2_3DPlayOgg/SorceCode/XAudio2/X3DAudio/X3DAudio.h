#ifndef X3DAUDIO_H
#define X3DAUDIO_H

#include "..\XAudio2Vector.h"
#include "..\XAudio2.h"
#include <x3daudio.h>	// XAudio2.h����O�ŃC���N���[�h�����Windows.h ���� windef.h�Ƌ�������.
class CX3DAudio
{
public:
	static constexpr int MAXOUTPUT_CHANNEL	= 8;
	static constexpr int INPUTCHANNEL		= 1;
public:
	// 3D�T�E���h���X�i�[(������)�f�[�^.
	struct AUDIO_LISTENER
	{
		XAudio2Vec FrontVec;	// ���ʃx�N�g��.
		XAudio2Vec Up;			// ��x�N�g��(���ʃx�N�g���Ɛ��K�����ł���K�v����).
		XAudio2Vec Pos;			// ���X�i�[�̍��W.
		XAudio2Vec Velocity;	// �h�b�y���Ŏg�������x.
		AUDIO_LISTENER()
			: FrontVec	( 0.0f,0.0f,1.0f )
			, Up		( 0.0f,1.0f,0.0f )
			, Pos		( 0.0f,0.0f,0.0f )
			, Velocity	( 0.0f,0.0f,0.0f )
		{}
	};
	// 3D�T�E���h�G�~�b�^�[(����)�f�[�^.
	// ���X�i�[�f�[�^�\���̂�����.
	struct AUDIO_EMITTER : AUDIO_LISTENER
	{
		float InnerRadius;				// �����a�̌v�Z�Ɏg�p�����l(0f ~ MAX_FLT)�A���̒l��0�ł�InnerRadiusAngle�͎g�p�\ .
		float InnerRadiusAngle;			// �����a�p�x�̌v�Z�Ɏg�p�����l(0f ~ X3DAUDIO_PI /4.0f).
		unsigned int ChannelCount;		// X3DAUDIO_EMITTER�\���̂ɂ���Ē�`���ꂽ�G�~�b�^�[�̐��B0���傫���Ȃ���΂Ȃ�Ȃ�.
		float ChannelRadius;			// �`���l�����z�u�����ʒu����̋����A�s��v�Z�̃}���`�`���l���G�~�b�^�ł̂ݎg�p�����B0.0f�ȏ�ł���K�v������.
		float CurveDistanceScaler;		// ���̕�������͈͋���"�Ȃ�"�Ɏg�p�����.
										// ������0.0f��CurveDistanceScaler �~1.0f�̊Ԃɂ���ꍇ�A�������K�p����Ȃ��悤�ɋt���̖@���ɏ����������ȃf�t�H���g�Ȑ����w��.
										// ������CurveDistanceScaler �~1.0f���傫���ꍇ�A�������́iCurveDistanceScaler �~1.0f�j/����.
										// CurveDistanceScaler �~2.0f�̋����ł́A�T�E���h�͔����̃{�����[���܂���-6 dB�ɂȂ�.
										// CurveDistanceScaler �~4.0f�̋����ł́A�T�E���h��1/4�{�����[���܂���-12dB�ɂȂ�.
		float DopplerScaler;			// �h�b�v���[�V�t�g���ʂ��֒����邽�߂Ɏg�p�����h�b�v���[�V�t�g�X�P�[���[(0f ~ FLT_MAX).
		AUDIO_EMITTER()
			: InnerRadius			( 2.0f )
			, InnerRadiusAngle		( 1.0f )
			, ChannelRadius			( 0.0f )
			, CurveDistanceScaler	( 14.0f )											
			, DopplerScaler			( 1.0f )
			, ChannelCount			( 2 )
		{}
	};
private:
	// ���̃N���X���Ŏg��3DAudio�X�e�[�g�\����.
	struct stAudioState
	{
		X3DAUDIO_DSP_SETTINGS	DspSettings;		// DSP�擾�p.
		X3DAUDIO_LISTENER		Listener;			// ���X�i�[�\����.
		X3DAUDIO_EMITTER		Emitter;			// �G�~�b�^�[�\����.
		X3DAUDIO_CONE			EmitterCone;		// �G�~�b�^�[�̃R�[��.
		FLOAT32					MatrixCoefficients[INPUTCHANNEL * MAXOUTPUT_CHANNEL];
		bool					IsInit;				// �����������t���O.
		bool					IsUseListenerCone;	// ���X�i�[�̃R�[�����g�p���邩.
		bool					IsUseInnerRadius;	// InnerRadius���g�p���邩.
		bool					IsUseDoppler;		// �h�b�y�����ʂ��g�p���邩.
		stAudioState()
			: IsInit				( false )
			, IsUseListenerCone		( true )
			, IsUseInnerRadius		( true )
			, IsUseDoppler			( false )
			, MatrixCoefficients	()
		{
			DspSettings		= { 0 };
			Listener		= { 0 };
			Emitter			= { 0 };
			EmitterCone		= { 0 };
		}
	};
public:
	CX3DAudio() {};

	CX3DAudio(IXAudio2SourceVoice* pSource, IXAudio2SubmixVoice* pSubMix,UINT SrcChannelCount);
	~CX3DAudio();

	// Listener�\���̃|�C���^�[���Z�b�g����.
	void SetListenerData(AUDIO_LISTENER& stListenerData);
	// ������̐��ʃx�N�g����ݒ�.
	void SetListenerFront(const XAudio2Vec vec);
	// ������̏�����x�N�g����ݒ�.
	// ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ���΂Ȃ�Ȃ�.
	void SetListenerUp(const XAudio2Vec vec);
	// ������̃��[���h�ʒu��ݒ�.
	void SetListenerPosition(const XAudio2Vec vec);
	// ������̉����x��ݒ�.
	// �����x�̓h�b�v���[���ʂɗ��p�����(�K�{�ł͂Ȃ�).
	void SetListenervelocity(const XAudio2Vec vec);
	// Emitter�\���̃|�C���^�[���Z�b�g����.
	void SetEmitterData(AUDIO_EMITTER& stEmitterData);
	// �����̐��ʃx�N�g����ݒ�.
	void SetEmitterFront(const XAudio2Vec vec);
	// �����̏�����x�N�g����ݒ�.
	// ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ���΂Ȃ�Ȃ�.
	void SetEmitterUp(const XAudio2Vec vec);
	// �����̃��[���h���W��ݒ�(3D�T�E���h�Đ���).
	void SetEmitterPosition(const XAudio2Vec vec);
	// �����̉����x��ݒ�.
	// �����x�̓h�b�v���[���ʂɗ��p�����(�K�{�ł͂Ȃ�).
	void SetEmittervelocity(const XAudio2Vec vec);
	// �h�b�v���[���ʂ̋������w��.
	void SetEmitterDopplerLevel(const float value);
	// �����̌��ʗ̈���w��.
	void SetEmitterRadius(const float value);
	// 3D�T�E���h�̏���ݒ肷��.
	bool Apply3DSoundValue(const bool CallOnce = false);
	// ���X�i�[�̃R�[�����g�p���邩�̃t���O���Z�b�g����.
	inline void SetUseListenerConeFlag(const bool isFlag)	{ m_stAudioState.IsUseListenerCone	= isFlag; }
	// InnerRadius���g�p���邩�̃t���O���Z�b�g����.
	inline void SetUseInnerRadiusFlag(const bool isFlag)	{ m_stAudioState.IsUseInnerRadius	= isFlag; }
	// �h�b�v���[���ʂ��g�p���邩�̃t���O���Z�b�g����.
	// �L���̏ꍇ�A���̃s�b�`�̕ύX�͂ł��Ȃ��Ȃ�.
	inline void SetUseDopplerFlag(const bool isFlag)		{ m_stAudioState.IsUseDoppler		= isFlag; }
	// �T�E���h�̌v�Z���I������.
	inline void StopCalc3DValue()							{ m_IsPlayEnd = true; }
	// ���łɗp�ӂ���Ă��郊�o�[�u��ݒ肷��.
	HRESULT SetReverb(const XAudio2Reverb::enReverbPreset nReverb);
	// ���.
	void Release();
private:
	HRESULT	Init();
private:
	stAudioState			m_stAudioState;
	AUDIO_EMITTER			m_InitEmitter;		// null�h�~�����ݒ�pEmitter�\����.
	AUDIO_EMITTER*			m_Emitter;			// �G�~�b�^�[�f�[�^�\����.
	AUDIO_EMITTER			m_InitListener;		// null�h�~�����ݒ�pListener�\����.
	AUDIO_LISTENER*			m_Listener;			// ���X�i�[�f�[�^�\����.
	IXAudio2SourceVoice*	m_pSourceVoice;		// �T�E���h���Đ�����X�g���[��.
	IXAudio2SubmixVoice*	m_pSubmixVoice;		// �T�u�~�b�N�X�{�C�X.
	XAUDIO2FX_REVERB_PARAMETERS m_ReverbParameters;	// ���o�[�u�̃p�����[�^.
	bool					m_IsPlayEnd;		// �Đ��I��.
	UINT					m_SrcChannelCount;	// �o�͐�`�����l���̐�.
												// �����ɕs���Ȓl������ƁA�����̃p�����������A�����ꉹ�ʂŏo�͂����̂Œ���.
};



#endif // !X3DAUDIO2_H
