#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <string>

#include "..\Scene\SceneManager\SceneManager.h"
#include "Global.h"
class CCamera;
class CFPSCounter;
/**************************************************
*	ﾒｲﾝｸﾗｽ.
**/
class CMain
{
	std::string WND_TITLE;
	std::string APP_NAME;
	const char* WINDOW_INFO_TEXT_PATH = "Data\\WindowInfo.txt";
public:
	CMain();
	~CMain();
	// ウィンドウ初期化関数.
	HRESULT InitWindow( HINSTANCE hInstance );

	// メッセージループ.
	void Loop();

	HRESULT Init();
	void Destroy();

private:
	// 更新処理.
	void Update();
	// 描画処理.
	void Render();

	// メッシュの読み込み関数.
	HRESULT Load();

	// ウィンドウ情報読み込み.
	void ReadWindowInfo();
private:
	HWND m_hWnd;

	// Dx11.
	ID3D11Device*				m_pDevice11;	// デバイス "11".
	ID3D11DeviceContext*		m_pContext11;	// コンテキスト "11".
	// Dx9.
	LPDIRECT3DDEVICE9			m_pDevice9;		// デバイス "9".

	std::shared_ptr<CCamera>	m_pCamera;		// カメラクラス.

	std::shared_ptr<CSceneManager>	m_pSceneManager;
	std::unique_ptr<CFPSCounter>	m_pFPS;
	bool	m_isResourceLoadEnd;				// 初めのリソースのロードが終わったかどうか.
};

#endif	// #ifndef MAIN_H.
