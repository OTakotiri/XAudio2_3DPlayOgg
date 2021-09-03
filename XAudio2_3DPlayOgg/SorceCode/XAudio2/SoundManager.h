#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <map>
#include <iostream>
#include <thread>
#include "XAudio2Play\XAudio2PlayBGM.h"
#include "XAudio2Play\XAudio2PlaySE.h"
#include "X3DAudio/X3DAudioPlay/X3DAudioPlayBGM.h"
#include "X3DAudio/X3DAudioPlay/X3DAudioPlaySE.h"
#include "OggLoad/OggLoad.h"

/**
 * @file SoundManager.h
 * @brief XAudio2�̃T�E���h�Đ��@�\���܂Ƃ߂��V���O���g���N���X.
 * @author OTakotiri.
 */

class CSoundManager
{
public:
//=============================
// �\���̐錾.
//=============================
	struct SoundVolume
	{
		float MasterVolume = 1.0f;
		float BGMVolume = 1.0f;
		float SEVolume = 1.0f;
	};
public:
	CSoundManager();
	~CSoundManager();
	// �T�E���h�f�[�^�쐬.
	static void CreateSoundData();
	// �C���X�^���X�̎擾.
	static CSoundManager* GetInstance();
	// BGM�N���X�̎擾.
	static std::shared_ptr<CXAudio2PlayBGM>		GetBGM(const std::string& filename);
	// SE�N���X�̎擾.
	static std::shared_ptr<CXAudio2PlaySE>		GetSE(const std::string& filename);
	// 3DBGM�N���X�̎擾.
	static std::shared_ptr<CX3DAudioPlayBGM>	Get3DBGM(const std::string& filename);
	// 3DSE�N���X�̎擾.
	static std::shared_ptr<CX3DAudioPlaySE>		Get3DSE(const std::string& filename);
	// �Q�[���S�̂�BGM�̍ő剹�ʂ��擾.
	inline static const float GetGameBGMVolume()		{ return GetInstance()->m_stSound.BGMVolume * GetInstance()->m_stSound.MasterVolume; }
	// �ő�}�X�^�[���ʎ擾
	inline static const float GetMasterVolume()			{ return GetInstance()->m_stSound.MasterVolume; }
	// �ő�BGM���ʎ擾
	inline static const float GetBGMVolume()			{ return GetInstance()->m_stSound.BGMVolume; }
	// �Q�[���S�̂�SE�̍ő剹�ʂ��擾.
	inline static const float GetGameSEVolume()			{ return GetInstance()->m_stSound.SEVolume * GetInstance()->m_stSound.MasterVolume; }
	// ��������֐�.
	static void Release();
	//========================================================================================
	// �I�v�V������ʗp�S�\�[�X�̃T�E���h����.
	// �}�X�^�[���ʃZ�b�g.
	static void SetMasterVolume(float& MasterVolume);
	// BGM,SE�̉��ʂ�ύX���邽�߂̃X���b�h�𗧂��グ��.
	static void CreateChangeSoundVolumeThread();
	// BGM,SE�̉��ʂ�ύX���邽�߂̃X���b�h���������.
	static bool ReleaseChangeSoundVolumeThread();
	//===========================================================
	// �I�v�V�����p�֐�.
	// ���ʂ�ύX�ł���X���b�h�̃X�e�[�g�ύX.
	static void StateChangeVolumeThread(const bool& bFlag);
	// �I�v�V�����p�X���b�h����ς݂��ǂ����̃t���O�擾�֐�.
	inline static const bool GetCreateOptionThread()	{ return GetInstance()->m_isCreateThread; }
	// �A�v���I���t���O�擾.
	inline const bool GetEndAppFlag()					{ return m_IsEndApp; }

public:
	SoundVolume m_stSound;
private:
	// ���ʂ�ύX����X���b�h�𗧂Ă�.
	static void SetChangeVolumeThread();
	// ���ʏ����ݒ�֐�.
	static void VolumeInit();
private:

	std::unordered_map <std::string, std::shared_ptr<CX3DAudioPlayBGM>> m_p3DBGMSource;
	std::unordered_map <std::string, std::shared_ptr<CX3DAudioPlaySE>>	m_p3DSESource;
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlayBGM>>	m_pBGMSource;
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlaySE>>	m_pSESource;

	// �I���p�t���O.
	bool	m_IsEndApp;
	// BGM,SE�̍쐬���I������ォ�ǂ���.
	bool	m_bEndCreate;
//=================================================
// �ȉ��I�v�V������ʗp.
	// �ȉ���̓X���b�h�ŉ񂳂Ȃ��ƃX�g���[�~���O�̓��ꍞ�݂�҂��Ă���̕ύX�ɂȂ邽��
	// �X���b�h�ɉ�.
	std::thread m_SoundSourceVolume;	// �I�v�V������ʂŁA���ʂ�ύX���邽�߂̃X���b�h.
	bool	m_bMoveSoundVolumeThread;	// �I�v�V������ʂŁABGM��ݒ�Ɠ����ɕύX���邽�߂̃X���b�h�p�ϐ�.
	bool	m_isCreateThread;	// ���ʕύX�̃X���b�h�������Ă��邩�ǂ���.
//------------------------------------------
// �X���b�h���b�N�p.
	bool	m_bResumeSoundVolumeThread;	// �����ϐ�.
	std::mutex m_SoundVolumemtx;
	std::condition_variable m_SoundVolumeCv;
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSoundManager(const CSoundManager &) = delete;
	CSoundManager& operator = (const CSoundManager &) = delete;
	CSoundManager(CSoundManager &&) = delete;
	CSoundManager& operator = (CSoundManager &&) = delete;
};

#endif // #ifndef SOUNDMANAGER_H.
