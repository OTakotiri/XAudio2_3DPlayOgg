#ifndef D3DX_H
#define D3DX_H

#include "..\..\Main\Global.h"

//----------------------------------------
// DirectX 9,11 �̃f�o�C�X�N���X.

class CDirectXDevice
{
private:
	std::mutex mtx;
public:
	CDirectXDevice();
	~CDirectXDevice();
	// �C���X�^���X�̎擾.
	static CDirectXDevice* GetInstance();
	// �쐬�֐�.
	static HRESULT Init( HWND hWnd);
	// ����֐�.
	static HRESULT Destroy();

	// �N���A�o�b�N�o�b�t�@.
	static void ClearBackBuffer();
	// �X���b�v�`�F�[���v���[���g.
	static void SwapChainPresent();
	// �f�o�C�X11�̎擾�֐�.
	inline static ID3D11Device*			GetDevice11()		{ return GetInstance()->m_pDevice11; }
	// �R���e�L�X�g11�̎擾�֐�.
	inline static ID3D11DeviceContext*	GetContext11()		{ return GetInstance()->m_pContext11; }
	// �f�o�C�X9�̎擾�֐�.
	inline static LPDIRECT3DDEVICE9		GetDevice9()		{ return GetInstance()->m_pDevice9; }
	// �r���[�|�[�g�̎擾�֐�.
	inline static D3D11_VIEWPORT		GetViewport()		{ return GetInstance()->m_Vp; }
	// �f�v�X�X�e���V���r���[�̎擾.
	inline static ID3D11DepthStencilView* GetDepthSV()		{ return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// �f�v�X�X�e���V��Tex�̎擾.
	inline static ID3D11Texture2D*		GetDepthTex()		{ return GetInstance()->m_pBackBuffer_DSTex; }
	// ���C���ō쐬�����E�B���h�E�n���h�����擾.
	static HWND GetWindowHundle();
	// BackBuffer�̐ݒ�.
	static void SetBackBuffer();
	// D3DX10CompileFromFile�}�N�����Ăяo��.
	static HRESULT MutexD3DX10CompileFromFile(
		const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
		LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump  *pPump,
		ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult);
	// D3DX11CompileFromFile�}�N�����Ăяo��.
	static HRESULT MutexD3DX11CompileFromFile(
		const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
		LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump  *pPump,
		ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult);
	// D11CreateVertexShader���Ăяo��.
	static HRESULT	DX11CreateVertexShader(ID3DBlob* pCompiledShader,ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader** pVertexShader);
	// D11CreatePixelShader���Ăяo��.
	static HRESULT	DX11CreatePixelShader(ID3DBlob* pCompiledShader, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader** pPixelShader);
	// D11CreateInputLayout���Ăяo��.
	static HRESULT	DX11CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* pLayout,UINT numElements,ID3DBlob* pCompiledShader,ID3D11InputLayout** m_pVertexLayout);

	static const UINT GetWidth() { return GetInstance()->m_Width; }
	static const UINT GetHeight() { return GetInstance()->m_Height; }
	// zeromemory.
	template <class Address>
	static void MutexZeroMemory(Address& Ad, size_t Len)
	{
		std::unique_lock<std::mutex> lock(GetInstance()->mtx);

		ZeroMemory(&Ad, Len);
	}
private: //----- �֐� ------.
	// �f�o�C�X9�̍쐬.
	HRESULT InitDevice9();
	// �f�o�C�X11�̍쐬.
	HRESULT InitDevice11();
	// �����_�[�^�[�Q�b�g�r���[�̍쐬.
	HRESULT InitTexRTV();
	// �X�e���V���r���[�̍쐬.
	HRESULT InitDSTex();
	// �r���[�|�[�g�̍쐬.
	HRESULT InitViewports();
	// ���X�^���C�U�̍쐬.
	HRESULT InitRasterizer();

private: //----- �ϐ� ------.
	HWND	m_hWnd;	// �E�B���h�E�n���h��.
	UINT	m_Width;
	UINT	m_Height;

	ID3D11Device*			m_pDevice11;			// �f�o�C�X11.
	ID3D11DeviceContext*	m_pContext11;			// �R���e�L�X�g11.
	LPDIRECT3DDEVICE9		m_pDevice9;				// �f�o�C�X9.

	IDXGISwapChain*			m_pSwapChain;			// �X���b�v�`�F�[��.

	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// �����_�[�^�[�Q�b�g�r���[.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// �e�N�X�`���[2D.

	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// �X�e���V���r���[.

	D3D11_VIEWPORT m_Vp;		// �r���[�|�[�g.
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectXDevice(const CDirectXDevice &) = delete;
	CDirectXDevice& operator = (const CDirectXDevice &) = delete;
	CDirectXDevice(CDirectXDevice &&) = delete;
	CDirectXDevice& operator = (CDirectXDevice &&) = delete;
};

#endif	// #ifndef D3DX_H.