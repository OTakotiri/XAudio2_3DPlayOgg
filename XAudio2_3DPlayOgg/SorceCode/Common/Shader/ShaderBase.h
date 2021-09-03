#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include "..\..\Main\Global.h"

class CShaderBase
{
public:
	CShaderBase();
	virtual ~CShaderBase();

	// ������.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) = 0;
	// ���.
	virtual HRESULT Release() = 0;

	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() = 0;

protected:
	// �V�F�[�_�[�Ǎ��֐�.
	static HRESULT InitShader(
		const char* filePath,
		const char* entryPoint,
		const char* shaderModel,
		UINT compileFlag,
		ID3DBlob** ppCompiledShader,
		ID3DBlob** ppErrors );

	// �o�[�e�b�N�X�V�F�[�_�[�쐬�֐�.
	HRESULT CreateVertexShader(
		ID3DBlob* pCompiledShader,
		ID3D11VertexShader** pVertexShader );

	// �s�N�Z���X�V�F�[�_�[�쐬�֐�.
	HRESULT CreatePixelShader(
		ID3DBlob* pCompiledShader,
		ID3D11PixelShader**	pPixelShader );

	// �C���v�b�g���C�A�E�g�쐬�֐�.
	HRESULT CreateInputLayout(
		D3D11_INPUT_ELEMENT_DESC* pLayout,
		UINT numElements,
		ID3DBlob* pCompiledShader, 
		ID3D11InputLayout** m_pVertexLayout );

	// �R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateConstantBuffer( 
		ID3D11Buffer** pConstantBuffer,
		UINT size );

	// ���_�C���v�b�g���C�A�E�g�̎擾.
	D3D11_INPUT_ELEMENT_DESC GetPositionInputElement() const; 
	// TEXCOORD�̎擾.
	D3D11_INPUT_ELEMENT_DESC GetTexcoordInputElement() const;
protected:
	ID3D11Device*			m_pDevice11;		// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;		// �f�o�C�X�R���e�L�X�g.

	ID3D11VertexShader*		m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*		m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*			m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
};

#endif	// #ifndef SHADER_BASE_H.