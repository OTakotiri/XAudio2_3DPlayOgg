#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"


// スタティックメッシュクラス.
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

	CDX9StaticMesh();	// コンストラクタ.
	~CDX9StaticMesh();	// デストラクタ.

	// 初期化.
	HRESULT Init(
		HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName);

	// 解放関数.
	void Release();

	// レンダリング用.
	void Render();

	// メッシュを取得.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

private:
	// メッシュ読み込み.
	HRESULT LoadXMesh(const char* fileName);
	// シェーダ作成.
	HRESULT InitShader();
	//ﾚﾝﾀﾞﾘﾝｸﾞ関数(ｸﾗｽ内でのみ使用する).
	void RenderMesh(
		D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj);
private:
	//======================================
	//	構造体.
	//======================================
	// メッシュ単位で渡す情報.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;		// ワールド行列.
		D3DXMATRIX	mWVP;	// ワールド,ビュー,プロジェクションの合成変換行列.	
	};
	// マテリアル単位で渡す情報.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	// アンビエント色(環境色).
		D3DXVECTOR4	vDiffuse;	// ディヒューズ色(拡散反射色).
		D3DXVECTOR4	vSpecular;	// スペキュラ色(鏡面反射色).
	};
	// フレーム単位で渡す情報.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
		D3DXVECTOR4	vLightPos;	//ﾗｲﾄ位置.
		D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
		D3DXMATRIX	mLightRot;	//ﾗｲﾄ回転行列.
		D3DXVECTOR4	fIntensity;	//ﾗｲﾄ強度(明るさ). ※xのみ使用する.
		D3DXVECTOR4 vColor;		//色.
		D3DXMATRIX	mLightWVP[4];
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR3	Normal;	// 法線(陰影計算に必須).
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
	// マテリアル構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	// アンビエント.
		D3DXVECTOR4	Diffuse;	// ディヒューズ.
		D3DXVECTOR4	Specular;	// スペキュラ.
		char szTextureName[64];	// テクスチャファイル名.
		ID3D11ShaderResourceView* pTexture;	// テクスチャ.
		DWORD dwNumFace;		// そのマテリアルのポリゴン数.
		// コンストラクタ.
		MY_MATERIAL()
			: Ambient(), Diffuse(), Specular(), szTextureName()
			, pTexture(nullptr), dwNumFace(0)
		{
		}
		// デストラクタ.
		~MY_MATERIAL()
		{
			SAFE_RELEASE(pTexture);
		}
	};

	HWND				m_hWnd;		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	//Dx9.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pCBufferPerMesh;		//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾒｯｼｭ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾏﾃﾘｱﾙ毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾌﾚｰﾑ毎).

	ID3D11Buffer*				m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.
	ID3D11Buffer**				m_ppIndexBuffer;	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*			m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.

	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	LPD3DXMESH			m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.
	DWORD				m_NumMaterials;	//ﾏﾃﾘｱﾙ数.

	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_EnableTexture;//ﾃｸｽﾁｬあり.
};

#endif	// #ifndef DX9_STATIC_MESH_H.