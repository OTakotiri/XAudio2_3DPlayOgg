#ifndef COMMON_H
#define COMMON_H

#include "..\Main\Global.h"

class CCommon
{
public:
	enum class enRS_STATE
	{
		None,	// ���w�ʕ`��.
		Back,	// �w�ʂ�`�悵�Ȃ�.
		Front,	// ���ʂ�`�悵�Ȃ�.
		Wire,	// ���C���[�t���[���`��.

		Max,
	};

public:
	CCommon();
	virtual ~CCommon();

	// ���W�̐ݒ�.
	void SetPosition(const D3DXVECTOR3& vPos) { m_vPos = vPos; }
	D3DXVECTOR3 GetPosition() { return m_vPos; }
	// ��]���̐ݒ�.
	void SetRotation(const D3DXVECTOR3& vRot) { m_vRot = vRot; }
	D3DXVECTOR3 GetRotation() { return m_vRot; }
	// �T�C�Y�̐ݒ�.
	void SetScale(const D3DXVECTOR3& vScale) { m_vScale = vScale; }
	void SetScale(const float& fScale) { m_vScale = { fScale, fScale, fScale }; }
	D3DXVECTOR3 GetScale() { return m_vScale; }
	// �F�̐ݒ�.
	void SetColor(const D3DXVECTOR4& vColor) { m_vColor = vColor; }
	void SetColor(const D3DXVECTOR3& vColor) { m_vColor = D3DXVECTOR4(vColor.x, vColor.y, vColor.z,m_vColor.w); }
	// �A���t�@�l�̐ݒ�.
	void SetAlpha(const float& fAlpha) { m_vColor.w = fAlpha; }

	// �u�����h��L��:�����ɐݒ肷��.
	void SetBlend(bool EnableAlpha);
	// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
	void SetCoverage(bool EnableCoverage);
	// �[�x�e�X�g��L��:�����ɐݒ肷��.
	void SetDepth(bool flag);
	// ���X�^���C�U�X�e�[�g�ݒ�.
	void SetRasterizerState(const enRS_STATE& rsState);
protected:
	// �f�o�C�X�̎擾�A�e���̏�����.
	HRESULT InitPram(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);

private:
	// �u�����h�쐬.
	HRESULT InitBlend();
	// �[�x�e�X�g�쐬.
	HRESULT InitDeprh();
	// ���X�^���C�U�X�e�[�g�쐬.
	HRESULT InitRasterizerState();
private:
	ID3D11DepthStencilState*	m_pDepthStencilState;		// �f�v�X�X�e���V���L��.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	// �f�v�X�X�e���V������.

	ID3D11BlendState*			m_pAlphaBlend;				// �A���t�@�u�����h�L��.
	ID3D11BlendState*			m_pNoAlphaBlend;			// �A���t�@�u�����h����.
	ID3D11BlendState*			m_pAlphaToCoverage;			// �A���t�@�J�o���[�W�L��.

	ID3D11RasterizerState*		m_pRsSoldAndNone;	// �\���b�hAnd���w�ʕ`��.
	ID3D11RasterizerState*		m_pRsSoldAndBack;	// �\���b�hAnd�w�ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pRsSoldAndFront;	// �\���b�hand���ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pRsWireFrame;		// ���C���[�t���[���`��.
protected:
	ID3D11Device*			m_pDevice11;	// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;	// �f�o�C�X�R���e�L�X�g.

	D3DXVECTOR3 m_vPos;		// ���W���.
	D3DXVECTOR3 m_vRot;		// ��]���.
	D3DXVECTOR3	m_vScale;	// �X�P�[�����.
	D3DXVECTOR4 m_vColor;	// �F.
};

#endif	// #ifndef COMMON_H.