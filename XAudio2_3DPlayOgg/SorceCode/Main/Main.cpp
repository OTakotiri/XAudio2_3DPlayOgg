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

	// �J�����̏�����.
	m_pCamera = std::make_shared<CCamera>();
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 10.0f, 20.0f));		// ���W.
	m_pCamera->SetLookPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ���_���W.

}

CMain::~CMain()
{
}

//====================================.
//	�������֐�.
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
//	����֐�.
//====================================.
void CMain::Destroy()
{
	m_pDevice11		= nullptr;
	m_pContext11	= nullptr;
	m_pDevice9		= nullptr;

	CDirectXDevice::Destroy();
}

//====================================.
//	�ǂݍ��݊֐�.
//====================================.
HRESULT CMain::Load()
{
	CMeshResource::Load(m_hWnd, m_pDevice11, m_pContext11, m_pDevice9);
	return S_OK;
}

//====================================.
//	�X�V����.
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
//	�`�揈��.
//====================================.
void CMain::Render()
{
	m_pCamera->InitViewProj();

	CDirectXDevice::SwapChainPresent();
	CDirectXDevice::ClearBackBuffer();
}

//====================================.
//	����֐�.
//====================================.
void CMain::Loop()
{
	Load();	// �ǂݍ��݊֐�.

	//ү����ٰ��.
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
	//���ع���݂̏I��.

}

//====================================.
// �E�B���h�E�������֐�.
//====================================.
HRESULT CMain::InitWindow( HINSTANCE hInstance )
{
	ReadWindowInfo();
	
	// �E�B���h�E�̒�`.
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

	// �E�B���h�E�N���X��Windows�ɓo�^.
	if( !RegisterClassEx( &wc ) ){
		_ASSERT_EXPR(false,"�E�B���h�E�N���X�̓o�^�Ɏ��s" );
		return E_FAIL;
	}


	// �E�B���h�E�̈�̒���.
	RECT rect = { 0, 0, WND_W, WND_H }; // ��`�\����.
	DWORD dwStyle;	// �E�B���h�E�X�^�C��.
	dwStyle = WS_OVERLAPPEDWINDOW;

	if (AdjustWindowRect(&rect, dwStyle, FALSE) == 0) {
		MessageBox(nullptr, "�E�B���h�E�̈�̒����Ɏ��s",
			"�G���[���b�Z�[�W", MB_OK);
		return E_FAIL;
	}
	RECT deskRect = {};	// ��ʃT�C�Y.
	SystemParametersInfo( SPI_GETWORKAREA, 0, &deskRect, 0 );
	int pos_x = 0, pos_y = 0;
	// ��ʒ����ɔz�u�ł���悤�Ɍv�Z.
	pos_x = (((deskRect.right - deskRect.left) - (rect.right - rect.left)) / 2 + deskRect.left);
	pos_y = (((deskRect.bottom - deskRect.top) - (rect.bottom - rect.top)) / 2 + deskRect.top);
	
	// �E�B���h�E�̍쐬.
	m_hWnd = CreateWindow(
		APP_NAME.c_str(),		// �A�v����.
		WND_TITLE.c_str(),		// �E�B���h�E�^�C�g��.
		WS_OVERLAPPEDWINDOW& ~WS_THICKFRAME& ~WS_MAXIMIZEBOX,	// �E�B���h�E���(����).
		pos_x, pos_y,			// �\���ʒux,y���W.
		rect.right - rect.left,	// �E�B���h�E��.
		rect.bottom - rect.top,	// �E�B���h�E����.
		nullptr,				// �e�E�B���h�E�n���h��..
		nullptr,				// ���j���[�ݒ�.
		hInstance,				// �C���X�^���X�ԍ�.
		nullptr);				// �E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.
	if (!m_hWnd) {
		_ASSERT_EXPR(false, "�E�B���h�E�N���X�쐬���s");
		return E_FAIL;
	}

	// �E�B���h�E�̕\��.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//====================================.
// �E�B���h�E���ǂݍ���.
//====================================.
void CMain::ReadWindowInfo()
{
	WND_TITLE = "ParallelShooting";
	APP_NAME = "ParallelShooting";
}