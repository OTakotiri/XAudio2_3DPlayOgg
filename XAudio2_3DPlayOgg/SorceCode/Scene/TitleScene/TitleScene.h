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

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// ���b�V���`��֐�.
	virtual void MeshRender() override;
	// �X�v���C�g(UI)�`��֐�.
	virtual void SpriteRender() override;
private:
	// �`��Ƃ������̓V�[���N���X�ɓ����ƒ����Ȃ�̂Ŗ{�����͕ʃN���X��.
	void RenderAikoMesh();
private:
	std::shared_ptr<CDX9StaticMesh>		m_pStatic;	// �X�^�e�B�b�N���b�V��.
	std::shared_ptr<CDX9StaticMesh>		m_pStatic2;	// �X�^�e�B�b�N���b�V��.

	std::shared_ptr<CSimpleCamera>		m_pCamera;		// �J����.
};

#endif	// #ifndef TITLE_H.