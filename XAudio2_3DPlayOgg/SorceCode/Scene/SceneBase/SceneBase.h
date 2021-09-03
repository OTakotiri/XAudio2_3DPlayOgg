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
 * @brief シーンベースクラス.
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


	
	// 読込関数.
	virtual bool Load() = 0;
	// 更新関数.
	virtual void Update() = 0;
	// メッシュ描画関数.
	virtual void MeshRender() = 0;
	// スプライト(UI)描画関数.
	virtual void SpriteRender() = 0;
protected:
	CSceneManager* m_pSceneManager;	// シーン情報.
	bool	m_isChangeScene;
	bool	m_isNextScene;
	enChangeSceneStep	m_IsChangeStep;
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSceneBase( const CSceneBase & )				= delete;
	CSceneBase& operator = ( const CSceneBase & )	= delete;
	CSceneBase( CSceneBase && )						= delete;
	CSceneBase& operator = ( CSceneBase && )		= delete;
};

#endif // #ifndef SCENE_BASE_H.