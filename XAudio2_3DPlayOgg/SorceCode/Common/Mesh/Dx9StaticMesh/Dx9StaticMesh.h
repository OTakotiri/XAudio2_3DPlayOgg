#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"


// �X�^�e�B�b�N���b�V���N���X.
class CDX9StaticMesh : public CCommon
{
	struct VERTEX;
public:
	CDX9StaticMesh(
		HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName)
		: CDX9StaticMesh()
	{
		Init(hWnd, pDevice11, pContext11, pDevice9, fileName);
	}

	CDX9StaticMesh();	// �R���X�g���N�^.
	~CDX9StaticMesh();	// �f�X�g���N�^.

	// ������.
	HRESULT Init(
		HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName);

	// ����֐�.
	void Release();

	// �����_�����O�p.
	void Render();

	// ���b�V�����擾.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

private:
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh(const char* fileName);
	// �V�F�[�_�쐬.
	HRESULT InitShader();
	//�����ݸފ֐�(�׽���ł̂ݎg�p����).
	void RenderMesh(
		D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj);
private:
	//======================================
	//	�\����.
	//======================================
	// ���b�V���P�ʂœn�����.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;		// ���[���h�s��.
		D3DXMATRIX	mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.	
	};
	// �}�e���A���P�ʂœn�����.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	// �A���r�G���g�F(���F).
		D3DXVECTOR4	vDiffuse;	// �f�B�q���[�Y�F(�g�U���ːF).
		D3DXVECTOR4	vSpecular;	// �X�y�L�����F(���ʔ��ːF).
	};
	// �t���[���P�ʂœn�����.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4	vCamPos;	//��׈ʒu(���_�ʒu).
		D3DXVECTOR4	vLightPos;	//ײĈʒu.
		D3DXVECTOR4	vLightDir;	//ײĕ���.
		D3DXMATRIX	mLightRot;	//ײĉ�]�s��.
		D3DXVECTOR4	fIntensity;	//ײċ��x(���邳). ��x�̂ݎg�p����.
		D3DXVECTOR4 vColor;		//�F.
		D3DXMATRIX	mLightWVP[4];
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR3	Normal;	// �@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
	// �}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	// �A���r�G���g.
		D3DXVECTOR4	Diffuse;	// �f�B�q���[�Y.
		D3DXVECTOR4	Specular;	// �X�y�L����.
		char szTextureName[64];	// �e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView* pTexture;	// �e�N�X�`��.
		DWORD dwNumFace;		// ���̃}�e���A���̃|���S����.
		// �R���X�g���N�^.
		MY_MATERIAL()
			: Ambient(), Diffuse(), Specular(), szTextureName()
			, pTexture(nullptr), dwNumFace(0)
		{
		}
		// �f�X�g���N�^.
		~MY_MATERIAL()
		{
			SAFE_RELEASE(pTexture);
		}
	};

	HWND				m_hWnd;		//����޳�����.
	//Dx9.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9���޲���޼ު��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pCBufferPerMesh;		//�ݽ����ޯ̧(ү����).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//�ݽ����ޯ̧(��رٖ�).
	ID3D11Buffer*			m_pCBufferPerFrame;		//�ݽ����ޯ̧(�ڰі�).

	ID3D11Buffer*				m_pVertexBuffer;	//���_�ޯ̧.
	ID3D11Buffer**				m_ppIndexBuffer;	//���ޯ���ޯ̧.
	ID3D11SamplerState*			m_pSampleLinear;	//�����:ø����Ɋe��̨����������.

	LPD3DXMESH			m_pMesh;		//ү����޼ު��.
	LPD3DXMESH			m_pMeshForRay;	//ڲ��ү���p.
	DWORD				m_NumMaterials;	//��رِ�.

	MY_MATERIAL*	m_pMaterials;	//��رٍ\����.
	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_EnableTexture;//ø�������.
};

#endif	// #ifndef DX9_STATIC_MESH_H.