#include "Main.h"

#include <fstream>
#include <sstream>

#include "..\Common\D3DX\D3DX.h"
#include "..\Camera\Camera.h"
#include "..\Camera\CameraManager\CameraManager.h"
//#include "..\Scene\SceneList.h"
#include "..\Resource\MeshResource\MeshResource.h"
#include "..\XAudio2\SoundManager.h"
#include "..\Common\FPSCounter\FPSCounter.h"
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CMain::CMain()
	: m_hWnd				( nullptr )
	, m_pDevice11			( nullptr )
	, m_pContext11			( nullptr )
	, m_pDevice9			( nullptr )
	, m_pCamera				( nullptr )
	, m_pSceneManager		( nullptr )
	, m_pFPS				( nullptr )
	, m_isResourceLoadEnd	( false )
{
	m_pSceneManager = std::make_shared<CSceneManager>();
	m_pFPS = std::make_unique<CFPSCounter>();

	// カメラの初期化.
	m_pCamera = std::make_shared<CCamera>();
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 10.0f, 20.0f));		// 座標.
	m_pCamera->SetLookPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 視点座標.

}

CMain::~CMain()
{
}

//====================================.
//	初期化関数.
//====================================.
HRESULT CMain::Init()
{
	if( FAILED(CDirectXDevice::Init( m_hWnd ) )) return E_FAIL;

	m_pDevice11		= CDirectXDevice::GetDevice11();
	m_pContext11	= CDirectXDevice::GetContext11();
	m_pDevice9		= CDirectXDevice::GetDevice9();
	return S_OK;
}

//====================================.
//	解放関数.
//====================================.
void CMain::Destroy()
{
	m_pDevice11		= nullptr;
	m_pContext11	= nullptr;
	m_pDevice9		= nullptr;

	CDirectXDevice::Destroy();
}

//====================================.
//	読み込み関数.
//====================================.
HRESULT CMain::Load()
{
	CMeshResource::Load(m_hWnd, m_pDevice11, m_pContext11, m_pDevice9);
	return S_OK;
}

//====================================.
//	更新処理.
//====================================.
void CMain::Update()
{
	CCameraManager::InitViewProj();

	CCameraManager::Update();
	m_isResourceLoadEnd = true;
	if (CMeshResource::ThreadRelease() == false) 
	{
		m_isResourceLoadEnd = false;
	}
	else {
		m_pSceneManager->Update();
	}
}

//====================================.
//	描画処理.
//====================================.
void CMain::Render()
{
	m_pCamera->InitViewProj();

	CDirectXDevice::SwapChainPresent();
	CDirectXDevice::ClearBackBuffer();
}

//====================================.
//	解放関数.
//====================================.
void CMain::Loop()
{
	Load();	// 読み込み関数.

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (m_pFPS->UpdateFPS() == true)
		{
			Update();
			Render();	
			std::string a = std::to_string(static_cast<int>(m_pFPS->GetFPS()));
			std::string WindowTitle = WND_TITLE + " [FPS:" + a + "]";
			SetWindowTextA(m_hWnd, WindowTitle.c_str());

		}
	}
	//ｱﾌﾟﾘｹｰｼｮﾝの終了.

}

//====================================.
// ウィンドウ初期化関数.
//====================================.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	ReadWindowInfo();
	
	// ウィンドウの定義.
	WNDCLASSEX wc = {};
	UINT windowStyle = CS_HREDRAW | CS_VREDRAW;

	wc.cbSize			= sizeof(wc);
	wc.style			= windowStyle;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= APP_NAME.c_str();
	wc.hIconSm			= LoadIcon( nullptr, IDI_APPLICATION );

	// ウィンドウクラスをWindowsに登録.
	if( !RegisterClassEx( &wc ) ){
		_ASSERT_EXPR(false,"ウィンドウクラスの登録に失敗" );
		return E_FAIL;
	}


	// ウィンドウ領域の調整.
	RECT rect = { 0, 0, WND_W, WND_H }; // 矩形構造体.
	DWORD dwStyle;	// ウィンドウスタイル.
	dwStyle = WS_OVERLAPPEDWINDOW;

	if (AdjustWindowRect(&rect, dwStyle, FALSE) == 0) {
		MessageBox(nullptr, "ウィンドウ領域の調整に失敗",
			"エラーメッセージ", MB_OK);
		return E_FAIL;
	}
	RECT deskRect = {};	// 画面サイズ.
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );
	int pos_x = 0, pos_y = 0;
	// 画面中央に配置できるように計算.
	pos_x = (((deskRect.right - deskRect.left) - (rect.right - rect.left)) / 2 + deskRect.left);
	pos_y = (((deskRect.bottom - deskRect.top) - (rect.bottom - rect.top)) / 2 + deskRect.top);
	
	// ウィンドウの作成.
	m_hWnd = CreateWindow(
		APP_NAME.c_str(),		// アプリ名.
		WND_TITLE.c_str(),		// ウィンドウタイトル.
		WS_OVERLAPPEDWINDOW& ~WS_THICKFRAME& ~WS_MAXIMIZEBOX,	// ウィンドウ種別(普通).
		pos_x, pos_y,			// 表示位置x,y座標.
		rect.right - rect.left,	// ウィンドウ幅.
		rect.bottom - rect.top,	// ウィンドウ高さ.
		nullptr,				// 親ウィンドウハンドル..
		nullptr,				// メニュー設定.
		hInstance,				// インスタンス番号.
		nullptr);				// ウィンドウ作成時に発生するイベントに渡すデータ.
	if (!m_hWnd) {
		_ASSERT_EXPR(false, "ウィンドウクラス作成失敗");
		return E_FAIL;
	}

	// ウィンドウの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//====================================.
// ウィンドウ情報読み込み.
//====================================.
void CMain::ReadWindowInfo()
{
	WND_TITLE = "ParallelShooting";
	APP_NAME = "ParallelShooting";
}