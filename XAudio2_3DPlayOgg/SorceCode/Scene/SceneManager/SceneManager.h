#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

// シーン列挙体.
enum class enSceneList
{
	None  = -1,	// 何もなし.

	Title,		// タイトル.

	Max

} typedef EScene;


/**
 * @file SceneManager.h
 * @brief シーンを管理するマネージャクラス.
 * @author OTakotiri.
 */
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// 更新関数.
	void Update();
	// 次のシーンに移動.
	void NextSceneMove();
private:
	std::shared_ptr<CSceneBase> m_pScene;
	EScene	m_NowScene;		// 現在のシーン.
	EScene	m_NextScene;	// 次のシーン.
	bool	m_IsLoadEnd;	// ロードが終了したか.
	bool	m_IsGameEnd;	// ゲームを終了したか.
};
#endif // #ifndef SCENE_MANAGER_H.