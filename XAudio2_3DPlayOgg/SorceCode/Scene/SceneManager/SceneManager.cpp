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
//	更新関数.
//====
void CSceneManager::Update()
{
	if (m_pScene == nullptr) return;

	if (m_IsLoadEnd == false) {
		// 読み込みが終了していない場合.
		// 読み込みを行う.
		m_IsLoadEnd = m_pScene->Load();
	}
	else {
		// 読み込みが終了したら.
		m_pScene->Update();	// シーンの更新.
		// ゲーム終了してなければ描画する.
		if (m_IsGameEnd == false) {
			if (m_IsLoadEnd == false) return;
			m_pScene->MeshRender();	// シーンのメッシュ描画.
			m_pScene->SpriteRender();	// シーンのスプライト(UI)描画.
		}
	}
}

//=================================.
// 次のシーンに移動.
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