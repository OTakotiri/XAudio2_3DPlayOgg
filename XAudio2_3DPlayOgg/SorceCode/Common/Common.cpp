#include "Common.h"
#include "D3DX/D3DX.h"
#include <mutex>

namespace {
	std::mutex mtx_;
	std::mutex localmtx_;
}
CCommon::CCommon()
	: m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pDepthStencilState		( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_pAlphaBlend				( nullptr )
	, m_pNoAlphaBlend			( nullptr )
	, m_pAlphaToCoverage		( nullptr )
	, m_pRsSoldAndNone			( nullptr )
	, m_pRsSoldAndBack			( nullptr )
	, m_pRsSoldAndFront			( nullptr )
	, m_pRsWireFrame			( nullptr )
	, m_vPos					( 0.0f, 0.0f, 0.0f )
	, m_vRot					( 0.0f, 0.0f, 0.0f )
	, m_vScale					( 1.0f, 1.0f, 1.0f )
	, m_vColor					( 1.0f, 1.0f, 1.0f, 1.0f )
{
}

CCommon::~CCommon()
{
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pDepthStencilStateOff);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pNoAlphaBlend);
	SAFE_RELEASE(m_pAlphaToCoverage);
	SAFE_RELEASE(m_pRsSoldAndNone);
	SAFE_RELEASE(m_pRsSoldAndBack);
	SAFE_RELEASE(m_pRsSoldAndFront);
	SAFE_RELEASE(m_pRsWireFrame);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//--------------------------------------------.
// �f�o�C�X�̎擾�A�e���̏�����.
//--------------------------------------------.
HRESULT CCommon::InitPram(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	std::lock_guard<std::mutex> lock(mtx_);

	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitBlend())) {
		_ASSERT_EXPR(false, "�u�����h�쐬���s");
		return E_FAIL;
	}
	if (FAILED(InitDeprh())) {
		_ASSERT_EXPR(false, "�[�x�e�X�g�쐬���s");
		return E_FAIL;
	}
	if (FAILED(InitRasterizerState())) {
		_ASSERT_EXPR(false, "���X�^���C�U�X�e�[�g�쐬���s");
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// �u�����h��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetBlend(bool EnableAlpha)
{
	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend =
		EnableAlpha == true ? m_pAlphaBlend : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState(blend, nullptr, mask);
}

//--------------------------------------------.
// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetCoverage(bool EnableCoverage)
{
	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend =
		EnableCoverage == true ? m_pAlphaToCoverage : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState(blend, nullptr, mask);
}

//--------------------------------------------.
// �[�x�e�X�g��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CCommon::SetDepth(bool flag)
{
	ID3D11DepthStencilState* pTmp
		= (flag == true) ? m_pDepthStencilState : m_pDepthStencilStateOff;
	// �[�x�ݒ���Z�b�g.
	m_pContext11->OMSetDepthStencilState(pTmp, 1);
}

//--------------------------------------------.
// ���X�^���C�U�X�e�[�g�ݒ�.
//--------------------------------------------.
void CCommon::SetRasterizerState(const enRS_STATE& rsState)
{
	switch (rsState)
	{
	case enRS_STATE::None:	// ���w�ʕ`��.
		m_pContext11->RSSetState(m_pRsSoldAndNone);
		break;
	case enRS_STATE::Back:	// �w�ʂ�`�悵�Ȃ�.
		m_pContext11->RSSetState(m_pRsSoldAndBack);
		break;
	case enRS_STATE::Front:	// ���ʂ�`�悵�Ȃ�.
		m_pContext11->RSSetState(m_pRsSoldAndFront);
		break;
	case enRS_STATE::Wire:	// ���C���[�t���[���`��.
		m_pContext11->RSSetState(m_pRsWireFrame);
		break;
	default:
		break;
	}
}

//--------------------------------------------.
// �u�����h�쐬.
//--------------------------------------------.
HRESULT CCommon::InitBlend()
{
	std::lock_guard<std::mutex> locallock(localmtx_);

	// ��̧�����ޗp�����޽ðč\����.
	// pnģ�ٓ��ɱ�̧��񂪂���̂ŁA���߂���悤�������޽ðĂŐݒ肷��.
	D3D11_BLEND_DESC BlendDesc = {};
	CDirectXDevice::MutexZeroMemory(BlendDesc, sizeof(BlendDesc));

	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//�����޽ðč쐬.
	if (FAILED(m_pDevice11->CreateBlendState(&BlendDesc, &m_pAlphaBlend))) {
		_ASSERT_EXPR(false, "�u�����h�X�e�[�g�쐬���s");
		return E_FAIL;
	}
	BlendDesc.AlphaToCoverageEnable = true;
	if (FAILED(m_pDevice11->CreateBlendState(&BlendDesc, &m_pAlphaToCoverage))) {
		_ASSERT_EXPR(false, "�u�����h�X�e�[�g�쐬���s");
		return E_FAIL;
	}
	//�����޽ðč쐬.
	BlendDesc.RenderTarget[0].BlendEnable = false;	//false:��̧�����ނ��g�p���Ȃ�.
	BlendDesc.AlphaToCoverageEnable = false;
	if (FAILED(m_pDevice11->CreateBlendState(&BlendDesc, &m_pNoAlphaBlend))) {
		_ASSERT_EXPR(false, "�u�����h�X�e�[�g�쐬���s");
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------.
// �[�x�e�X�g�쐬.
//--------------------------------------------.
HRESULT CCommon::InitDeprh()
{
	std::lock_guard<std::mutex> locallock(localmtx_);

	// �[�x�e�X�g(z�e�X�g)��L���ɂ���.
	D3D11_DEPTH_STENCIL_DESC dsDesc = D3D11_DEPTH_STENCIL_DESC();

	dsDesc.DepthEnable = TRUE;	// �L��.
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// �[�x�ݒ�쐬.
	if (FAILED(m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilState)))
	{
		_ASSERT_EXPR(false, "�[�x�ݒ�쐬���s");
		return E_FAIL;
	}

	// �[�x�e�X�g(z�e�X�g)�𖳌��ɂ���.
	dsDesc.DepthEnable = FALSE;	// ����.
	// �[�x�ݒ�쐬.
	if (FAILED(m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilStateOff)))
	{
		_ASSERT_EXPR(false, "�[�x�ݒ�쐬���s");
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// ���X�^���C�U�X�e�[�g�쐬.
//--------------------------------------------.
HRESULT CCommon::InitRasterizerState()
{
	std::lock_guard<std::mutex> locallock(localmtx_);

	D3D11_RASTERIZER_DESC rdc = {};
	CDirectXDevice::MutexZeroMemory(rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;	// �h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_NONE;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise = FALSE;			// ��غ�݂̕\�������肷���׸�.
	rdc.DepthClipEnable = FALSE;			// �����ɂ��Ă̸د��ݸޗL��.

	auto createRasterizerState = [&](ID3D11RasterizerState** ppRs)
	{
		if (FAILED(m_pDevice11->CreateRasterizerState(&rdc, ppRs))) {
			_ASSERT_EXPR(false, L"���X�^���C�U�[�쐬���s");
		}
	};
	createRasterizerState(&m_pRsSoldAndNone);

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_BACK;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	createRasterizerState(&m_pRsSoldAndBack);

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_FRONT;// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	createRasterizerState(&m_pRsSoldAndFront);

	rdc.FillMode = D3D11_FILL_WIREFRAME;// �h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_NONE;		// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	createRasterizerState(&m_pRsWireFrame);

	m_pContext11->RSSetState(m_pRsSoldAndNone);

	return S_OK;
}