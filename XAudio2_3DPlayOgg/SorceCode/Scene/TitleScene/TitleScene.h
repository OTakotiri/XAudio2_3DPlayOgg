#ifndef TITLE_H
#define TITLE_H

#include "..\SceneBase\SceneBase.h"

class CDX9SkinMesh;
class CDX9StaticMesh;
class CSimpleCamera;

class CTitleScene : public CSceneBase
{
public:
	CTitleScene(CSceneManager* pSceneManager);
	virtual ~CTitleScene();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// メッシュ描画関数.
	virtual void MeshRender() override;
	// スプライト(UI)描画関数.
	virtual void SpriteRender() override;
private:
	// 描画とか処理はシーンクラスに入れると長くなるので本実装は別クラスで.
	void RenderAikoMesh();
private:
	std::shared_ptr<CDX9StaticMesh>		m_pStatic;	// スタティックメッシュ.
	std::shared_ptr<CDX9StaticMesh>		m_pStatic2;	// スタティックメッシュ.

	std::shared_ptr<CSimpleCamera>		m_pCamera;		// カメラ.
};

#endif	// #ifndef TITLE_H.