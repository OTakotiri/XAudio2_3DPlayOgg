#include "SceneManager.h"
#include "..\TitleScene\TitleScene.h"
#include "..\..\Common\D3DX\D3DX.h"
CSceneManager::CSceneManager()
	: m_pScene			( nullptr )
	, m_NowScene		( EScene::Title )
	, m_NextScene		( EScene::Title)
	, m_IsLoadEnd		( false )
	, m_IsGameEnd		( false )
{
	NextSceneMove();

}
CSceneManager::~CSceneManager()
{
}

//=================================
//	�X�V�֐�.
//====
void CSceneManager::Update()
{
	if (m_pScene == nullptr) return;

	if (m_IsLoadEnd == false) {
		// �ǂݍ��݂��I�����Ă��Ȃ��ꍇ.
		// �ǂݍ��݂��s��.
		m_IsLoadEnd = m_pScene->Load();
	}
	else {
		// �ǂݍ��݂��I��������.
		m_pScene->Update();	// �V�[���̍X�V.
		// �Q�[���I�����ĂȂ���Ε`�悷��.
		if (m_IsGameEnd == false) {
			if (m_IsLoadEnd == false) return;
			m_pScene->MeshRender();	// �V�[���̃��b�V���`��.
			m_pScene->SpriteRender();	// �V�[���̃X�v���C�g(UI)�`��.
		}
	}
}

//=================================.
// ���̃V�[���Ɉړ�.
//=================================.
void CSceneManager::NextSceneMove()
{
	m_IsLoadEnd = false;
	switch (m_NextScene)
	{
	case EScene::Title:
		m_pScene = std::make_shared<CTitleScene>(this);
		m_NowScene = m_NextScene;
		m_NextScene = EScene::Title;
		break;
	default:
		break;
	}
}