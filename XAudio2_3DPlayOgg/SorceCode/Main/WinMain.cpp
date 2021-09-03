#include "Main.h"
#include <Windows.h>
#include "..\Common\D3DX\D3DX.h"

#include "..\XAudio2\SoundManager.h"


// ImGui�Ŏg�p.
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hInstancePrev, 
	LPSTR pCmdLine, 
	int nCmdShow )
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG.

	std::unique_ptr<CMain> pMain = std::make_unique<CMain>();

	if( pMain == nullptr ) return 0;

	// �E�B���h�E�̍쐬.
	if( FAILED( pMain->InitWindow( hInstance ) )) return 0;
	// ���C������������.
	if( FAILED( pMain->Init() )) return 0;

	// ���C�����[�v.
	pMain->Loop();

	// ���C���������.
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
	// �E�B���h�E�̘g�ȂǁA�V�X�e�����j���[���I�����ꂽ�ꍇ�ɑ��M����郁�b�Z�[�W.
	case WM_SYSCOMMAND:
		// �E�B���h�E�́~�{�^���������ꂽ�ꍇ.
		if (wParam == SC_CLOSE) {
			if (MessageBox(nullptr,
				"�ްт��I�����܂����H", "�x��", MB_YESNO) == IDYES)
			{
				//����޳��j������.
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
			else {
				// 0��Ԃ����ƂŁAWM_CLOSE�ɍs���Ȃ��Ȃ�(�E�B���h�E����Ȃ�).
				// 0��Ԃ��Ȃ��ƁA��ŏI�����Ȃ�����I�����Ă��Ă��A�E�B���h�E��������.
				return 0;
			}
		}
		break;
	case VK_ESCAPE:	//ESC��.
#ifdef _DEBUG
			if( MessageBox( nullptr, "�Q�[�����I�����܂����H", "�x��", MB_YESNO ) == IDYES ){
				PostQuitMessage(0);
			}
		break;
#endif	// #ifdef _DEBUG.
		break;
	case WM_CLOSE:
		//�T�E���h�}�l�[�W���[�����[�X.
		CSoundManager::Release();
		//����޳��j������.
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}