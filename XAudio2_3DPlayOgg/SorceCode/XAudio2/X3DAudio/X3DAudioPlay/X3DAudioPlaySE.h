#ifndef X3DAUDIO_PLAYSE_H
#define X3DAUDIO_PLAYSE_H

#include <mutex>
#include "..\..\XAudio2.h"
#include "..\..\XAudio2Master\XAudio2MasterVoice.h"
#include "..\..\OggLoad\OggLoad.h"
#include "..\..\XAudio2CallBack\VoiceCallBack.h"

#include "..\..\X3DAudio\X3DAudio.h"
class CX3DAudioPlaySE
{
private:
	static const int SE_VOICE_SIZE = 6;	// SE�̗\���̐�.
public:
	CX3DAudioPlaySE();
	~CX3DAudioPlaySE();
	// �����ݒ�.
	HRESULT InitSourceVoice(const char* filename);
	// 3D���̃A�b�v�f�[�g�֐�.
	void Update3DSound();
	// SE���Đ�������Ԃ�.
	const bool IsPlayingSE(const size_t ArrayNum);
	// ���d�Đ��\�ȃT�E���h�Đ�.
	bool Play();
	// ���d�Đ����Ȃ��T�E���h�Đ�.
	bool NoMultiplePlay();
	// SE�T�E���h��~.
	bool SeStop(const size_t ArrayNum);
	// �쐬���Ă�\������SE����~.
	bool AllSeStop();

	HRESULT CreateOggSound();
	//=======================================================================
	// 3D�T�E���h�Đ�.
	//====
	// ������ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
	void SetListenerInfo(CX3DAudio::AUDIO_LISTENER& stListenerData);
	// �����ɐݒ肷��\���̃|�C���^�[���Z�b�g����.
	void SetEmitterInfo(CX3DAudio::AUDIO_EMITTER& stEmitterData);
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
	// SE��Voice�\�[�X���ő剹�ʂ�ݒ�.
	void SetMaxSEVolume(const float& fMaxVolume) { m_fSEVolume = fMaxVolume; }
	// SE��Voice�\�[�X���ő剹�ʂ��擾.
	const float GetMaxSEVolume() { return m_fSEVolume; }
	// SE�̉��ʂ��擾���܂�.
	const float GetSEVolume(size_t Array);
	// SE�̉��ʂ�ݒ肵�܂�.
	bool SetSEVolume(float value, size_t Array);
	// SoundSource��j�󂷂�֐�.
	void DestoroySource();
	// �S��SE���ʂƂ͕ʂ̃\�[�X���̉��ʂ��Z�b�g.
	void SetAnotherSEVolume(const float& fVolume) { m_fAnotherVolume = fVolume; }
	// �\�[�X���Őݒ肳��Ă��鉹�ʂ�K�p���邩�ǂ���.
	void SetUseAnotherSEVolume(const float& bFlag) { m_bisAnotherVolume = bFlag; }

	void Release();
protected:
	int		CheckPlaySeBuffer();
protected:
	// Ogg�f�[�^�N���X�̔�
	std::shared_ptr<COggLoad>	m_pOggWavData;
	// ���̃N���X�ōĐ�����t�@�C����.
	std::string					m_SourceFileName;
	bool	m_IsAppEnd;


	std::unique_ptr<CX3DAudio>	m_pX3DAudio[SE_VOICE_SIZE];	// 3D�T�E���h�N���X.
	IXAudio2SubmixVoice*	m_pSubmixVoice[SE_VOICE_SIZE];		// �T�u�~�b�N�X�{�C�X.

	IXAudio2SourceVoice*	m_pSourceVoice[SE_VOICE_SIZE];		// ����SE�𓯎��ɂȂ点��悤�ɔz��ɂ���.
	bool				m_bisAnotherVolume;
	float				m_fAnotherVolume;
	int					m_CheckNonePlaySeVoice;					// �Đ����Ă��Ȃ�SE��􂢏o���p.
	int					m_RepeatPlaySe;							// �S�����Đ����̎��Ɏ��Ɋ����߂��čĐ�����z��ԍ���Ԃ�.		

	float m_fSEVolume;		// SE����.
};



#endif // !X3DAUDIO_PLAYSE_H