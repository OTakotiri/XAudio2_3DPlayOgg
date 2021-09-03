#include "D3DX.h"

#include "..\..\Camera\CameraManager\CameraManager.h"
#include <mutex>
//=============================
// �萔�錾.
//=============================
namespace {
	constexpr float CLEAR_BACK_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// �o�b�N�J���[.
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
// �C���X�^���X�̎擾.
//-----------------------------------.
CDirectXDevice* CDirectXDevice::GetInstance()
{
	static std::unique_ptr<CDirectXDevice> pInstance =
		std::make_unique<CDirectXDevice>();
	return pInstance.get();
}


//-----------------------------------.
// �쐬�֐�.
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
// ����֐�.
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
// �N���A�o�b�N�o�b�t�@.
//-----------------------------------.
void CDirectXDevice::ClearBackBuffer()
{
	// �J���[�o�b�N�o�b�t�@.
	GetInstance()->m_pContext11->ClearRenderTargetView(GetInstance()->m_pBackBuffer_TexRTV, CLEAR_BACK_COLOR );

	// �f�v�X�X�e���V���o�b�t�@.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}
//-----------------------------------.
// �X���b�v�`�F�[���v���[���g.
//-----------------------------------.
void CDirectXDevice::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
}
//-----------------------------------.
// �E�B���h�E�n���h�����擾.
//-----------------------------------.
HWND CDirectXDevice::GetWindowHundle()
{
	if (GetInstance()->m_hWnd == nullptr) return nullptr;
	return GetInstance()->m_hWnd;
}
//-----------------------------------.
// �o�b�N�o�b�t�@���^�[�Q�b�g�ɃZ�b�g.
//-----------------------------------.
void CDirectXDevice::SetBackBuffer()
{
	// �����_�[�^�[�Q�b�g�̐ݒ�.
	GetInstance()->m_pContext11->OMSetRenderTargets(
		1,
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV);
	// �f�v�X�X�e���V���o�b�t�@.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);
}
//-----------------------------------.
// D3DX10CompileFromFile�}�N�����Ăяo��.
//-----------------------------------.
HRESULT CDirectXDevice::MutexD3DX10CompileFromFile(const LPCTSTR filepath, const D3D_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX10ThreadPump  *pPump,
	ID3D10Blob **ppOutShader, ID3D10Blob **ppOutErrorMsgs, HRESULT *pOutHResult)
{

	HRESULT hr;
	hr = D3DX10CompileFromFile(
		filepath,		// �V�F�[�_�[�t�@�C���p�X.
		pDefines,		// �}�N����`�̃|�C���^�[ : �I�v�V����.
		pInclude,		// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		entryPoint,		// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		shaderModel,	// �V�F�[�_�[���f���̖��O.
		compileFlag,	// �V�F�[�_�[�R���p�C���t���O.
		Flags2,			// �G�t�F�N�g�R���p�C���t���O (0����).
		pPump,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		ppOutShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		ppOutErrorMsgs,	// �R���p�C�����̃G���[�o�� (out).
		pOutHResult);	// �߂�l�̃|�C���^.


	return hr;
}
//-----------------------------------.
// D3DX11CompileFromFile�}�N�����Ăяo��.
//-----------------------------------.
HRESULT CDirectXDevice::MutexD3DX11CompileFromFile(const LPCTSTR filepath, const D3D_SHADER_MACRO * pDefines, LPD3D10INCLUDE pInclude,
	LPCSTR entryPoint, LPCSTR shaderModel, UINT compileFlag, UINT Flags2, ID3DX11ThreadPump * pPump,
	ID3D10Blob ** ppOutShader, ID3D10Blob ** ppOutErrorMsgs, HRESULT * pOutHResult)
{

	HRESULT hr;
	hr = D3DX11CompileFromFile(	
		filepath,		// �V�F�[�_�[�t�@�C���p�X.
		pDefines,		// �}�N����`�̃|�C���^�[ : �I�v�V����.
		pInclude,		// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		entryPoint,		// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		shaderModel,	// �V�F�[�_�[���f���̖��O.
		compileFlag,	// �V�F�[�_�[�R���p�C���t���O.
		Flags2,			// �G�t�F�N�g�R���p�C���t���O (0����).
		pPump,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		ppOutShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		ppOutErrorMsgs,	// �R���p�C�����̃G���[�o�� (out).
		pOutHResult);	// �߂�l�̃|�C���^.


	return hr;
}
//-----------------------------------.
// D11CreateVertexShader���Ăяo��.
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
// D11CreatePixelShader���Ăяo��.
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
// D11CreateInputLayout���Ăяo��.
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
// �f�o�C�X9�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitDevice9()
{
	LPDIRECT3D9 m_pD3d9; // Dx9�I�u�W�F�N�g.
	
	//�uDirect3D�v�I�u�W�F�N�g�̍쐬.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		_ASSERT_EXPR( false, L"Dx9�I�u�W�F�N�g�쐬���s" );
		MessageBox( nullptr, "Dx9�I�u�W�F�N�g�쐬���s", "�x��", MB_OK );
		return E_FAIL;
	}

	int a = 0;

	// Diret3D �f�o�C�X�I�u�W�F�N�g�̍쐬.
	D3DPRESENT_PARAMETERS d3dpp = {};
	CDirectXDevice::MutexZeroMemory(d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g.
	d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g.
	d3dpp.Windowed = true;						// �E�B���h�E���[�h.
	d3dpp.EnableAutoDepthStencil = true;		// �X�e���V���L��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// �X�e���V���̃t�H�[�}�b�g(16bit).
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

	// �f�o�C�X�쐬
	// (HALӰ��:�`��ƒ��_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (HALӰ��:�`���GPU�A���_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}
	MessageBox( nullptr, "HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�", "�x��", MB_OK );

	// (REFӰ��:�`���CPU�A���_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (REFӰ��:�`��ƒ��_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	MessageBox( nullptr, "Direct3D���޲��쐬���s", "�x��", MB_OK );

	SAFE_RELEASE( m_pD3d9 );
	return E_FAIL;
}

//-----------------------------------.
// �f�o�C�X11�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitDevice11()
{
	// �X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// �o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width		= WND_W;							// �o�b�N�o�b�t�@�̕�.
	sd.BufferDesc.Height	= WND_H;							// �o�b�N�o�b�t�@�̍���.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// �t�H�[�}�b�g(32�ޯĶװ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// ���t���b�V�����[�g(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// ���t���b�V�����[�g(���q).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �g����(�\����).
	sd.OutputWindow			= m_hWnd;							// �E�B���h�E�n���h��.
	sd.SampleDesc.Count		= 1;								// �}���`�T���v���̐�.
	sd.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	sd.Windowed				= TRUE;								// �E�B���h�E���[�h(�t���X�N���[������FALSE).

	//�쐬�����݂�@�\���x���̗D����w��.
	// (GPU���T�|�[�g����@�\��Ă̒�`).
	// D3D_FEATURE_LEVEL�񋓌^�̔z��.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU���x��.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// �z��̗v�f��.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// �r�f�I�A�_�v�^�ւ̃|�C���^.
			type,				// �쐬����Ãf�o�C�X�̎��.
			nullptr,			// �\�t�g�E�F�A ���X�^���C�U����������DLL�̃n���h��.
			0,					// �L���ɂ��郉���^�C�����C���[.
			&pFeatureLevels,	// �쐬�����݂�@�\���x���̏������w�肷��z��ւ̃|�C���^.
			1,					// ���̗v�f��.
			D3D11_SDK_VERSION,	// SDK�̃o�[�W����.
			&sd,				// �X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
			&m_pSwapChain,		// (out) �����_�����O�Ɏg�p����X���b�v�`�F�[��.
			&m_pDevice11,		// (out) �쐬���ꂽ�f�o�C�X.
			pFeatureLevel,		// �@�\���ق̔z��ɂ���ŏ��̗v�f��\���|�C���^.
			&m_pContext11 );	// (out) �f�o�C�X�R���e�L�X�g.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				MessageBox( nullptr,
					"�f�o�C�X�ƃX���b�v�`�F�[���쐬���s",
					"Error", MB_OK );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

	// ALT + Enter�Ńt���X�N���[���𖳌�������.
	IDXGIFactory* pFactory = nullptr;
	// ��ō����IDXGISwapChain���g��.
	m_pSwapChain->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	// �]�v�ȋ@�\�𖳌��ɂ���ݒ������.
	pFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

//-----------------------------------.
// �����_�[�^�[�Q�b�g�r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾.
		(LPVOID*)&pBackBuffer_Tex);	//(out)�ޯ��ޯ̧ø���.

	//����ø����ɑ΂������ް���ޯ��ޭ�(RTV)���쐬.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.

	//�ޯ��ޯ̧ø��������.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// �X�e���V���r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitDSTex()
{
	int wnd_Width = WND_W;
	int wnd_Height = WND_H;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= m_Width;				// ��.
	descDepth.Height				= m_Height;				// ����.
	descDepth.MipLevels				= 1;						// Я��ϯ������:1.
	descDepth.ArraySize				= 1;						// �z��:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32�ޯ�̫�ϯ�.
	descDepth.SampleDesc.Count		= 1;						// �������ق̐�.
	descDepth.SampleDesc.Quality	= 0;						// �������ق̸��è.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// �g�p���@:��̫��.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// �[�x(��ݼقƂ��Ďg�p).
	descDepth.CPUAccessFlags		= 0;						// CPU����ͱ������Ȃ�.
	descDepth.MiscFlags				= 0;						// ���̑��̐ݒ�Ȃ�.


	// ����ø����ɑ΂������߽��ݼ�(DSTex)���쐬.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		_ASSERT_EXPR( false, L"�f�v�X�X�e���V���쐬���s" );
		return E_FAIL;
	}
	// ����ø����ɑ΂������߽��ݼ��ޭ�(DSV)���쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		_ASSERT_EXPR( false, L"�f�v�X�X�e���V���r���[�쐬���s" );
		return E_FAIL;
	}
	// ���ް���ޯ��ޭ������߽��ݼ��ޭ����߲��ײ݂ɾ��.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );

	return S_OK;
}

//-----------------------------------.
// �r���[�|�[�g�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitViewports()
{
	m_Vp.Width	= (FLOAT)m_Width;	// ��.
	m_Vp.Height	= (FLOAT)m_Height;	// ����.
	m_Vp.MinDepth = 0.0f;			// �ŏ��[�x(��O).
	m_Vp.MaxDepth = 1.0f;			// �ő�[�x(��).
	m_Vp.TopLeftX = 0.0f;			// ����ʒux.
	m_Vp.TopLeftY = 0.0f;			// ����ʒuy.

	m_pContext11->RSSetViewports(1, &m_Vp);

	return S_OK;
}

//-----------------------------------.
// ���X�^���C�U�̍쐬.
//-----------------------------------.
HRESULT CDirectXDevice::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc = {};
	CDirectXDevice::MutexZeroMemory(rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// �h��Ԃ�(�د��).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;			// ��غ�݂̕\�������肷���׸�.
	rdc.DepthClipEnable			= FALSE;			// �����ɂ��Ă̸د��ݸޗL��.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		_ASSERT_EXPR( false, L"���X�^���C�U�[�쐬���s" );
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}