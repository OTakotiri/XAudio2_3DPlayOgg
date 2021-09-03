#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

// �V�[���񋓑�.
enum class enSceneList
{
	None  = -1,	// �����Ȃ�.

	Title,		// �^�C�g��.

	Max

} typedef EScene;


/**
 * @file SceneManager.h
 * @brief �V�[�����Ǘ�����}�l�[�W���N���X.
 * @author OTakotiri.
 */
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// �X�V�֐�.
	void Update();
	// ���̃V�[���Ɉړ�.
	void NextSceneMove();
private:
	std::shared_ptr<CSceneBase> m_pScene;
	EScene	m_NowScene;		// ���݂̃V�[��.
	EScene	m_NextScene;	// ���̃V�[��.
	bool	m_IsLoadEnd;	// ���[�h���I��������.
	bool	m_IsGameEnd;	// �Q�[�����I��������.
};
#endif // #ifndef SCENE_MANAGER_H.