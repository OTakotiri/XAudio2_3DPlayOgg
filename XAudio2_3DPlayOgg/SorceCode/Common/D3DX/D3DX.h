#ifndef D3DX_H
#define D3DX_H

#include "..\..\Main\Global.h"

//----------------------------------------
// DirectX 9,11 のデバイスクラス.

class CDirectXDevice
{
private:
	std::mutex mtx;
public:
	CDirectXDevice();
	~CDirectXDevice();
	// インスタンスの取得.
	static CDirectXDevice* GetInstance();
	// 作成関数.
	static HRESULT Init( HWND hWnd);
	// 解放関数.
	static HRESULT Destroy();

	// クリアバックバッファ.
	static void ClearBackBuffer();
	// スワップチェーンプレゼント.
	static void SwapChainPresent();
	// デバイス11の取得関数.
	inline static ID3D11Device*			GetDevice11()		{ return GetInstance()->m_pDevice11; }
	// コンテキスト11の取得関数.
	inline static ID3D11DeviceContext*	GetContext11()		{ return GetInstance()->m_pContext11; }
	// デバイス9の取得関数.
	inline static LPDIRECT3DDEVICE9		GetDevice9()		{ return GetInstance()->m_pDevice9; }
	// ビューポートの取得関数.
	inline static D3D11_VIEWPORT		GetViewport()		{ return GetInstance()->m_Vp; }
	// デプスステンシルビューの取得.
	inline static ID3D11DepthStencilView* GetDepthSV()		{ return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// デプスステンシルTexの取得.
	inline static ID3D11Texture2D*		GetDepthTex()		{ return GetInstance()->m_pBackBuffer_DSTex; }
	// メインで作成したウィンドウハンドルを取得.
	static HWND GetWindowHundle();
	// BackBufferの設定.
	static void SetBackBuffer();
	// D3DX10CompileFromFileマクロを呼び出す.
	static HRESULT MutexD3DX10CompileFromFile(
		const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
		LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump  *pPump,
		ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult);
	// D3DX11CompileFromFileマクロを呼び出す.
	static HRESULT MutexD3DX11CompileFromFile(
		const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
		LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump  *pPump,
		ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult);
	// D11CreateVertexShaderを呼び出す.
	static HRESULT	DX11CreateVertexShader(ID3DBlob* pCompiledShader,ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader** pVertexShader);
	// D11CreatePixelShaderを呼び出す.
	static HRESULT	DX11CreatePixelShader(ID3DBlob* pCompiledShader, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader** pPixelShader);
	// D11CreateInputLayoutを呼び出す.
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
private: //----- 関数 ------.
	// デバイス9の作成.
	HRESULT InitDevice9();
	// デバイス11の作成.
	HRESULT InitDevice11();
	// レンダーターゲットビューの作成.
	HRESULT InitTexRTV();
	// ステンシルビューの作成.
	HRESULT InitDSTex();
	// ビューポートの作成.
	HRESULT InitViewports();
	// ラスタライザの作成.
	HRESULT InitRasterizer();

private: //----- 変数 ------.
	HWND	m_hWnd;	// ウィンドウハンドル.
	UINT	m_Width;
	UINT	m_Height;

	ID3D11Device*			m_pDevice11;			// デバイス11.
	ID3D11DeviceContext*	m_pContext11;			// コンテキスト11.
	LPDIRECT3DDEVICE9		m_pDevice9;				// デバイス9.

	IDXGISwapChain*			m_pSwapChain;			// スワップチェーン.

	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// レンダーターゲットビュー.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// テクスチャー2D.

	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// ステンシルビュー.

	D3D11_VIEWPORT m_Vp;		// ビューポート.
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDirectXDevice(const CDirectXDevice &) = delete;
	CDirectXDevice& operator = (const CDirectXDevice &) = delete;
	CDirectXDevice(CDirectXDevice &&) = delete;
	CDirectXDevice& operator = (CDirectXDevice &&) = delete;
};

#endif	// #ifndef D3DX_H.