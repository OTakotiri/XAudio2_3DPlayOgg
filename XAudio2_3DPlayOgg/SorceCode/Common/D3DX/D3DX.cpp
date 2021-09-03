#include "D3DX.h"

#include "..\..\Camera\CameraManager\CameraManager.h"
#include <mutex>
//=============================
// 定数宣言.
//=============================
namespace {
	constexpr float CLEAR_BACK_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// バックカラー.
}

CDirectXDevice::CDirectXDevice()
	: m_hWnd					( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
{
}

CDirectXDevice::~CDirectXDevice()
{
}

//-----------------------------------.
// インスタンスの取得.
//-----------------------------------.
CDirectXDevice* CDirectXDevice::GetInstance()
{
	static std::unique_ptr<CDirectXDevice> pInstance =
		std::make_unique<CDirectXDevice>();
	return pInstance.get();
}


//-----------------------------------.
// 作成関数.
//-----------------------------------.
HRESULT CDirectXDevice::Init( HWND hWnd )
{
	GetInstance()->m_hWnd = hWnd;
	GetInstance()->m_Width = WND_W;
	GetInstance()->m_Height = WND_H;

	if( FAILED(GetInstance()->InitDevice9()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitDevice11()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitTexRTV()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitDSTex()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitViewports()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizer()) )		return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// 解放関数.
//-----------------------------------.
HRESULT CDirectXDevice::Destroy()
{
	SAFE_RELEASE(GetInstance()->m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(GetInstance()->m_pBackBuffer_DSTex);
	SAFE_RELEASE(GetInstance()->m_pBackBuffer_TexRTV);
	SAFE_RELEASE(GetInstance()->m_pSwapChain);
	SAFE_RELEASE(GetInstance()->m_pContext11);
	SAFE_RELEASE(GetInstance()->m_pDevice11);
	SAFE_RELEASE(GetInstance()->m_pDevice9);

	return S_OK;
}
//-----------------------------------.
// クリアバックバッファ.
//-----------------------------------.
void CDirectXDevice::ClearBackBuffer()
{
	// カラーバックバッファ.
	GetInstance()->m_pContext11->ClearRenderTargetView(GetInstance()->m_pBackBuffer_TexRTV, CLEAR_BACK_COLOR );

	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}
//-----------------------------------.
// スワップチェーンプレゼント.
//-----------------------------------.
void CDirectXDevice::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
}
//-----------------------------------.
// ウィンドウハンドルを取得.
//-----------------------------------.
HWND CDirectXDevice::GetWindowHundle()
{
	if (GetInstance()->m_hWnd == nullptr) return nullptr;
	return GetInstance()->m_hWnd;
}
//-----------------------------------.
// バックバッファをターゲットにセット.
//-----------------------------------.
void CDirectXDevice::SetBackBuffer()
{
	// レンダーターゲットの設定.
	GetInstance()->m_pContext11->OMSetRenderTargets(
		1,
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV);
	// デプスステンシルバッファ.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);
}
//-----------------------------------.
// D3DX10CompileFromFileマクロを呼び出す.
//-----------------------------------.
HRESULT CDirectXDevice::MutexD3DX10CompileFromFile(const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump  *pPump,
	ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult)
{

	HRESULT hr;
	hr = D3DX10CompileFromFile(
		filepath,		// シェーダーファイルパス.
		pDefines,		// マクロ定義のポインター : オプション.
		pInclude,		// インクルードファイルを処理するためのポインタ : オプション.
		entryPoint,		// シェーダーのエントリーポイント関数名.
		shaderModel,	// シェーダーモデルの名前.
		compileFlag,	// シェーダーコンパイルフラグ.
		Flags2,			// エフェクトコンパイルフラグ (0推奨).
		pPump,			// スレッドポンプインターフェイスへのポインター.
		ppOutShader,	// コンパイルされたシェーダーのデータ (out).
		ppOutErrorMsgs,	// コンパイル中のエラー出力 (out).
		pOutHResult);	// 戻り値のポインタ.


	return hr;
}
//-----------------------------------.
// D3DX11CompileFromFileマクロを呼び出す.
//-----------------------------------.
HRESULT CDirectXDevice::MutexD3DX11CompileFromFile(const LPCTSTR filepath, const D3D_SHADER_MACRO * pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump * pPump,
	ID3D10Blob ** ppOutShader, ID3D10Blob ** ppOutErrorMsgs, HRESULT * pOutHResult)
{

	HRESULT hr;
	hr = D3DX11CompileFromFile(	
		filepath,		// シェーダーファイルパス.
		pDefines,		// マクロ定義のポインター : オプション.
		pInclude,		// インクルードファイルを処理するためのポインタ : オプション.
		entryPoint,		// シェーダーのエントリーポイント関数名.
		shaderModel,	// シェーダーモデルの名前.
		compileFlag,	// シェーダーコンパイルフラグ.
		Flags2,			// エフェクトコンパイルフラグ (0推奨).
		pPump,			// スレッドポンプインターフェイスへのポインター.
		ppOutShader,	// コンパイルされたシェーダーのデータ (out).
		ppOutErrorMsgs,	// コンパイル中のエラー出力 (out).
		pOutHResult);	// 戻り値のポインタ.


	return hr;
}
//-----------------------------------.
// D11CreateVertexShaderを呼び出す.
//-----------------------------------.
HRESULT CDirectXDevice::DX11CreateVertexShader(ID3DBlob * pCompiledShader, ID3D11ClassLinkage * pClassLinkage, ID3D11VertexShader ** pVertexShader)
{

	HRESULT hr;

	hr = GetInstance()->m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		pClassLinkage,
		pVertexShader);

	return hr;
}
//-----------------------------------.
// D11CreatePixelShaderを呼び出す.
//-----------------------------------.
HRESULT CDirectXDevice::DX11CreatePixelShader(ID3DBlob * pCompiledShader, ID3D11ClassLinkage * pClassLinkage, ID3D11PixelShader ** pPixelShader)
{

	HRESULT hr;

	hr = GetInstance()->m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		pClassLinkage,
		pPixelShader);

	return hr;
}
//-----------------------------------.
// D11CreateInputLayoutを呼び出す.
//-----------------------------------.
HRESULT CDirectXDevice::DX11CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * pLayout, UINT numElements, ID3DBlob * pCompiledShader, ID3D11InputLayout ** m_pVertexLayout)
{

	HRESULT hr;
	hr = GetInstance()->m_pDevice11->CreateInputLayout(
		pLayout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		m_pVertexLayout);

	return hr;
}
//-----------------------------------.
// デバイス9の作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitDevice9()
{
	LPDIRECT3D9 m_pD3d9; // Dx9オブジェクト.
	
	//「Direct3D」オブジェクトの作成.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		_ASSERT_EXPR( false, L"Dx9オブジェクト作成失敗" );
		MessageBox( nullptr, "Dx9オブジェクト作成失敗", "警告", MB_OK );
		return E_FAIL;
	}

	int a = 0;

	// Diret3D デバイスオブジェクトの作成.
	D3DPRESENT_PARAMETERS d3dpp = {};
	CDirectXDevice::MutexZeroMemory(d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// バックバッファのフォーマット.
	d3dpp.BackBufferCount = 1;					// バックバッファの数.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// スワップエフェクト.
	d3dpp.Windowed = true;						// ウィンドウモード.
	d3dpp.EnableAutoDepthStencil = true;		// ステンシル有効.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// ステンシルのフォーマット(16bit).
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	auto createDevice = [&]( const D3DDEVTYPE& deviceType, const DWORD& behaviorFlags )
	{
		return m_pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT, 
			deviceType, 
			m_hWnd,
			behaviorFlags,
			&d3dpp, 
			&m_pDevice9 );
	};

	// デバイス作成
	// (HALﾓｰﾄﾞ:描画と頂点処理をGPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (HALﾓｰﾄﾞ:描画はGPU、頂点処理をCPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}
	MessageBox( nullptr, "HALﾓｰﾄﾞでﾃﾞﾊﾞｲｽ作成できません\nREFﾓｰﾄﾞで再試行します", "警告", MB_OK );

	// (REFﾓｰﾄﾞ:描画はCPU、頂点処理をGPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (REFﾓｰﾄﾞ:描画と頂点処理をCPUで行う).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	MessageBox( nullptr, "Direct3Dﾃﾞﾊﾞｲｽ作成失敗", "警告", MB_OK );

	SAFE_RELEASE( m_pD3d9 );
	return E_FAIL;
}

//-----------------------------------.
// デバイス11の作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitDevice11()
{
	// スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// バックバッファの数.
	sd.BufferDesc.Width		= WND_W;							// バックバッファの幅.
	sd.BufferDesc.Height	= WND_H;							// バックバッファの高さ.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// フォーマット(32ﾋﾞｯﾄｶﾗｰ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// リフレッシュレート(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// リフレッシュレート(分子).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 使い方(表示先).
	sd.OutputWindow			= m_hWnd;							// ウィンドウハンドル.
	sd.SampleDesc.Count		= 1;								// マルチサンプルの数.
	sd.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	sd.Windowed				= TRUE;								// ウィンドウモード(フルスクリーン時はFALSE).

	//作成を試みる機能レベルの優先を指定.
	// (GPUがサポートする機能ｾｯﾄの定義).
	// D3D_FEATURE_LEVEL列挙型の配列.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 の GPUレベル.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// 配列の要素数.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// ビデオアダプタへのポインタ.
			type,				// 作成するﾃデバイスの種類.
			nullptr,			// ソフトウェア ラスタライザを実装するDLLのハンドル.
			0,					// 有効にするランタイムレイヤー.
			&pFeatureLevels,	// 作成を試みる機能レベルの順序を指定する配列へのポインタ.
			1,					// ↑の要素数.
			D3D11_SDK_VERSION,	// SDKのバージョン.
			&sd,				// スワップチェーンの初期化パラメータのポインタ.
			&m_pSwapChain,		// (out) レンダリングに使用するスワップチェーン.
			&m_pDevice11,		// (out) 作成されたデバイス.
			pFeatureLevel,		// 機能ﾚﾍﾞﾙの配列にある最初の要素を表すポインタ.
			&m_pContext11 );	// (out) デバイスコンテキスト.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				MessageBox( nullptr,
					"デバイスとスワップチェーン作成失敗",
					"Error", MB_OK );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

	// ALT + Enterでフルスクリーンを無効化する.
	IDXGIFactory* pFactory = nullptr;
	// 上で作ったIDXGISwapChainを使う.
	m_pSwapChain->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	// 余計な機能を無効にする設定をする.
	pFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

//-----------------------------------.
// レンダーターゲットビューの作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得.
		(LPVOID*)&pBackBuffer_Tex);	//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対してﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.

	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// ステンシルビューの作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitDSTex()
{
	int wnd_Width = WND_W;
	int wnd_Height = WND_H;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= m_Width;				// 幅.
	descDepth.Height				= m_Height;				// 高さ.
	descDepth.MipLevels				= 1;						// ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize				= 1;						// 配列数:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count		= 1;						// ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality	= 0;						// ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// 深度(ｽﾃﾝｼﾙとして使用).
	descDepth.CPUAccessFlags		= 0;						// CPUからはｱｸｾｽしない.
	descDepth.MiscFlags				= 0;						// その他の設定なし.


	// そのﾃｸｽﾁｬに対してﾃﾞﾌﾟｽｽﾃﾝｼﾙ(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	// そのﾃｸｽﾁｬに対してﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		_ASSERT_EXPR( false, L"デプスステンシルビュー作成失敗" );
		return E_FAIL;
	}
	// ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );

	return S_OK;
}

//-----------------------------------.
// ビューポートの作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitViewports()
{
	m_Vp.Width	= (FLOAT)m_Width;	// 幅.
	m_Vp.Height	= (FLOAT)m_Height;	// 高さ.
	m_Vp.MinDepth = 0.0f;			// 最小深度(手前).
	m_Vp.MaxDepth = 1.0f;			// 最大深度(奥).
	m_Vp.TopLeftX = 0.0f;			// 左上位置x.
	m_Vp.TopLeftY = 0.0f;			// 左上位置y.

	m_pContext11->RSSetViewports(1, &m_Vp);

	return S_OK;
}

//-----------------------------------.
// ラスタライザの作成.
//-----------------------------------.
HRESULT CDirectXDevice::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc = {};
	CDirectXDevice::MutexZeroMemory(rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:背面を描画しない, FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;			// ﾎﾟﾘｺﾞﾝの表裏を決定するﾌﾗｸﾞ.
	rdc.DepthClipEnable			= FALSE;			// 距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		_ASSERT_EXPR( false, L"ラスタライザー作成失敗" );
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}