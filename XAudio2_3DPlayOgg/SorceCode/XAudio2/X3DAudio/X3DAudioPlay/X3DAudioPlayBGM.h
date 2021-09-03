#ifndef X3DAUDIO_PLAYBGM_H
#define X3DAUDIO_PLAYBGM_H

#include <mutex>
#include "..\..\XAudio2.h"
#include "..\..\XAudio2Master\XAudio2MasterVoice.h"
#include "..\..\OggLoad\OggLoad.h"
#include "..\..\XAudio2CallBack\VoiceCallBack.h"

#include "..\..\X3DAudio\X3DAudio.h"
class CX3DAudioPlayBGM
{
public:
	CX3DAudioPlayBGM();
	~CX3DAudioPlayBGM();
	// �����ݒ�.
	HRESULT InitSourceVoice(const char* filename);
	// �Đ����Ă邩�ǂ����擾�֐�.
	const bool IsPlaying();
	// �X���b�h�������BGM���Đ�����.
	void PlayBGM(const bool LoopFlag = true);
	// BGM�X���b�h���~����.
	bool StopBGMThread();
	// BGM�T�E���h�����S��~.(�ʒu��ێ����Ȃ�).
	bool Stop();
	// �T�E���h�ꎞ��~.
	bool Pause();
	// �T�E���h�Đ����ĊJ.
	bool PlayStart();
	// �s�b�`�ݒ�֐�.
	bool SetPitch(float value);
	// �s�b�`���擾�֐�.
	const float GetPitch();
	// �ő�s�b�`��ݒ肵�܂�.
	void SetMaxPitch(float value);
	// �s�b�`��ݒ�.
	void SetPitchValue(const float value) { m_fPitch = value; }
	// Ogg����\�[�X�{�C�X���쐬����.
	HRESULT CreateOggSound();
	//=======================================================================
	// 3D�T�E���h�Đ�.
	//====
	// ������ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
	void SetListenerInfo(CX3DAudio::AUDIO_LISTENER& stListenerData);
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
	// �����ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
	void SetEmitterInfo(CX3DAudio::AUDIO_EMITTER& stEmitterData);
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
	// ���o�[�u��ݒ�.
	HRESULT SetReverb(const XAudio2Reverb::enReverbPreset Preset);
	// ���X�i�[�̃R�[�����g�p���邩�̃t���O���Z�b�g����.
	void SetUseListenerConeFlag(const bool isFlag);
	// InnerRadius���g�p���邩�̃t���O���Z�b�g����.
	void SetUseInnerRadiusFlag(const bool isFlag);
	// �h�b�v���[���ʂ��g�p���邩�̃t���O���Z�b�g����.
	// �L���̏ꍇ�A���̃s�b�`�̕ύX�͂ł��Ȃ��Ȃ�.
	void SetUseDopplerFlag(const bool isFlag);
	//=======================================================================
	// ��{���ʑ���n.
	//====
	// BGM�̉��ʂ��擾���܂�.
	const float GetVolume();
	// BGM�̉��ʂ�ݒ肵�܂�.
	bool SetBGMVolume(float value);
	// ���݂̉��ʂ�����Z�܂��͌��Z.
	bool  AdjustVolume(float value);
	// SoundSource��j�󂷂�֐�.
	void DestoroySource();
	// �t�F�[�h�A�E�g�t���O�̃Z�b�^�[.
	inline void SetFadeOutFlag(bool BGMFadeOutFlag)			{ m_bFadeOutStart = BGMFadeOutFlag; }
	// �t�F�[�h�A�E�g�t���O�̃Q�b�^�[.
	inline const bool GetFadeOutFlag()						{ return m_bFadeOutStart; }
	// �t�F�[�h�C���t���O�̃Z�b�^�[.
	inline void SetFadeInFlag(bool BGMFadeInFlag)			{ m_bFadeInStart = BGMFadeInFlag; }
	// �t�F�[�h�C���t���O�̃Q�b�^�[.
	inline const bool GetFadeInFlag()						{ return m_bFadeInStart; }

	void Release();
protected:

	// BGM���t�F�[�h�A�E�g����֐�.
	bool FadeOutBGM(float value, const bool& isGameEnd);
	// BGM���t�F�[�h�C������֐�.
	bool FadeInBGM(float value, const bool& isGameEnd);
	// �����f�[�^���X�g���[���ɗ�������.
	HRESULT OggSubmit();
private:
	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	bool Play(bool& isEnd, const bool IsLoop = true);
	// �t�F�[�h�A�E�g�p�̃X���b�h���N������.
	void CallFadeOutBGM(bool& isEnd);
	// �t�F�[�h�C���p�̃X���b�h���N������.
	void CallFadeInBGM(bool& isEnd);
protected: //--------------- �ϐ� ---------------.
	// Ogg�f�[�^�N���X�̔�
	std::shared_ptr<COggLoad>	m_pOggWavData;
	// BGM�X�g���[�~���O�Đ��p�X���b�h.
	std::thread					m_pBGMThread;
	// ���̃N���X�ōĐ�����t�@�C����.
	std::string					m_SourceFileName;
	// �Đ��I��(�X���b�h�I��)�t���O.
	bool						m_IsEndPlay;

	std::thread m_Calc3DSoundThread;

	std::mutex			m_mtx;
	VoiceCallback		m_Callback;		// �R�[���o�b�N.
	XAUDIO2_BUFFER		m_Buffer;		// �I�[�f�B�I�o�b�t�@�[
	int					m_Buff_len;		// �o�b�t�@�����W.
	BYTE**				m_Buff;			// �o�b�t�@�T�C�Y.
	int					m_Len;			// �����W.
	int					m_Buff_cnt;		// �o�b�t�@�J�E���g.
	int					m_Size;

	std::unique_ptr<CX3DAudio>	m_pX3DAudio;	// 3D�T�E���h�N���X.
	IXAudio2SourceVoice*	m_pSourceVoice; //  �T�E���h���Đ�����X�g���[��.
	IXAudio2SubmixVoice*	m_pSubmixVoice;	// �T�u�~�b�N�X�{�C�X.

	std::thread	m_FadeInThread;			// �{�����[���t�F�[�h�C���p�X���b�h.
	std::thread	m_FadeOutThread;		// �{�����[���t�F�[�h�A�E�g�p�X���b�h.

	bool	m_isMoveFadeInThread;		// �t�F�[�h�C���X���b�h�������Ă��邩.
	bool	m_isMoveFadeOutThread;		// �t�F�[�h�A�E�g�X���b�h�������Ă��邩.
	float	m_fMaxPitch;				// �ő�s�b�`.
	float	m_fPitch;					// �s�b�`.
	bool	m_bFadeOutStart;			// �t�F�[�h�A�E�g�J�n�t���O.
	bool	m_bFadeInStart;				// �t�F�[�h�C���J�n�t���O.
	float	m_fFadeInValue;				// �t�F�[�h�C���̒l.
	float	m_fFadeOutValue;			// �t�F�[�h�A�E�g�̒l.
	bool	m_bFirstPlay;				// ����Đ����ǂ���.
};



#endif // !X3DAUDIO_PLAYBGM_H
