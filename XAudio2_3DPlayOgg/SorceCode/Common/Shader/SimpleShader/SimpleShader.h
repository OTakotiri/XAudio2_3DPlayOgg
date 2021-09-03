#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include "..\ShaderBase.h"

class CSimpleShader : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\SimpleShader.hlsl";	// �V�F�[�_�[�t�@�C����.

public:
	// �R���X�^���g�o�b�t�@�̃A�v�����̒�`.
	//���V�F�[�_�[���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.	
		D3DXVECTOR4	vColor;		// �J���[(RGBA�̌^�ɍ��킹��).
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
	};

public:
	CSimpleShader();
	virtual ~CSimpleShader();

	// ������.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// ���.
	virtual HRESULT Release() override;

	// �R���X�^���g�o�b�t�@�Ƀf�[�^��n��.
	void SetConstantBufferData( const D3DXMATRIX& mWVP, const D3DXVECTOR4& color );
	// �e��V�F�[�_�̐ݒ�.
	void ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer );

private:
	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() override;
};

#endif	// #ifndef SIMPLE_SHADER_H.