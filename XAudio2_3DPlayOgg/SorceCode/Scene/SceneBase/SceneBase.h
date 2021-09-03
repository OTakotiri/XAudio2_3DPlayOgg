#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Main\Global.h"

class CSceneManager;
/**
 * @file SceneBase.h
 * @brief �V�[���x�[�X�N���X.
 * @author OTakotiri.
 */
class CSceneBase
{
public:
	enum class enChangeSceneStep
	{
		None	= -1,
		FadeOut,
		BGMFade,
		WaitAllFadeEnd,
		ChangeScene
	};
public:
	CSceneBase(CSceneManager* pSceneManager)
		: m_pSceneManager	( pSceneManager )
		, m_isChangeScene	( false )
		, m_isNextScene		( false )
		, m_IsChangeStep	( enChangeSceneStep::None )
	{}
	virtual ~CSceneBase(){};


	
	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// ���b�V���`��֐�.
	virtual void MeshRender() = 0;
	// �X�v���C�g(UI)�`��֐�.
	virtual void SpriteRender() = 0;
protected:
	CSceneManager* m_pSceneManager;	// �V�[�����.
	bool	m_isChangeScene;
	bool	m_isNextScene;
	enChangeSceneStep	m_IsChangeStep;
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSceneBase( const CSceneBase & )				= delete;
	CSceneBase& operator = ( const CSceneBase & )	= delete;
	CSceneBase( CSceneBase && )						= delete;
	CSceneBase& operator = ( CSceneBase && )		= delete;
};

#endif // #ifndef SCENE_BASE_H.