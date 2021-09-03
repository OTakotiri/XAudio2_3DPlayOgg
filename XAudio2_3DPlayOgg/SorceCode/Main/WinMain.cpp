#include "Main.h"
#include <Windows.h>
#include "..\Common\D3DX\D3DX.h"

#include "..\XAudio2\SoundManager.h"


// ImGuiで使用.
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hInstancePrev, 
	LPSTR pCmdLine, 
	int nCmdShow )
{
#ifdef _DEBUG
	// メモリリーク検出
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG.

	std::unique_ptr<CMain> pMain = std::make_unique<CMain>();

	if( pMain == nullptr ) return 0;

	// ウィンドウの作成.
	if( FAILED( pMain->InitWindow( hInstance ) )) return 0;
	// メイン初期化処理.
	if( FAILED( pMain->Init() )) return 0;

	// メインループ.
	pMain->Loop();

	// メイン解放処理.
	pMain->Destroy();

	return 0;
}

LRESULT CALLBACK WndProc(
	HWND hWnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam )
{
	switch( uMsg ){
	case WM_CREATE:
		break;
	case WM_SIZE:
		break;
	// ウィンドウの枠など、システムメニューが選択された場合に送信されるメッセージ.
	case WM_SYSCOMMAND:
		// ウィンドウの×ボタンが押された場合.
		if (wParam == SC_CLOSE) {
			if (MessageBox(nullptr,
				"ｹﾞｰﾑを終了しますか？", "警告", MB_YESNO) == IDYES)
			{
				//ｳｨﾝﾄﾞｳを破棄する.
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
			else {
				// 0を返すことで、WM_CLOSEに行かなくなる(ウィンドウを閉じない).
				// 0を返さないと、上で終了しない方を選択していても、ウィンドウが閉じられる.
				return 0;
			}
		}
		break;
	case VK_ESCAPE:	//ESCｷｰ.
#ifdef _DEBUG
			if( MessageBox( nullptr, "ゲームを終了しますか？", "警告", MB_YESNO ) == IDYES ){
				PostQuitMessage(0);
			}
		break;
#endif	// #ifdef _DEBUG.
		break;
	case WM_CLOSE:
		//サウンドマネージャーリリース.
		CSoundManager::Release();
		//ｳｨﾝﾄﾞｳを破棄する.
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}