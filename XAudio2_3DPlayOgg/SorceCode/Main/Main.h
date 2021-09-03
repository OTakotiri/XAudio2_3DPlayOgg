#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <string>

#include "..\Scene\SceneManager\SceneManager.h"
#include "Global.h"
class CCamera;
class CFPSCounter;
/**************************************************
*	Ҳݸ׽.
**/
class CMain
{
	std::string WND_TITLE;
	std::string APP_NAME;
	const char* WINDOW_INFO_TEXT_PATH = "Data\\WindowInfo.txt";
public:
	CMain();
	~CMain();
	// �E�B���h�E�������֐�.
	HRESULT InitWindow( HINSTANCE hInstance );

	// ���b�Z�[�W���[�v.
	void Loop();

	HRESULT Init();
	void Destroy();

private:
	// �X�V����.
	void Update();
	// �`�揈��.
	void Render();

	// ���b�V���̓ǂݍ��݊֐�.
	HRESULT Load();

	// �E�B���h�E���ǂݍ���.
	void ReadWindowInfo();
private:
	HWND m_hWnd;

	// Dx11.
	ID3D11Device*				m_pDevice11;	// �f�o�C�X "11".
	ID3D11DeviceContext*		m_pContext11;	// �R���e�L�X�g "11".
	// Dx9.
	LPDIRECT3DDEVICE9			m_pDevice9;		// �f�o�C�X "9".

	std::shared_ptr<CCamera>	m_pCamera;		// �J�����N���X.

	std::shared_ptr<CSceneManager>	m_pSceneManager;
	std::unique_ptr<CFPSCounter>	m_pFPS;
	bool	m_isResourceLoadEnd;				// ���߂̃��\�[�X�̃��[�h���I��������ǂ���.
};

#endif	// #ifndef MAIN_H.
