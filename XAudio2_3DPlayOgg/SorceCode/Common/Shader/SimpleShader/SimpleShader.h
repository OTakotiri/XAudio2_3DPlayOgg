#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include "..\ShaderBase.h"

class CSimpleShader : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\SimpleShader.hlsl";	// シェーダーファイル名.

public:
	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		// ワールド,ビュー,プロジェクションの合成変換行列.	
		D3DXVECTOR4	vColor;		// カラー(RGBAの型に合わせる).
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
	};

public:
	CSimpleShader();
	virtual ~CSimpleShader();

	// 初期化.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// 解放.
	virtual HRESULT Release() override;

	// コンスタントバッファにデータを渡す.
	void SetConstantBufferData( const D3DXMATRIX& mWVP, const D3DXVECTOR4& color );
	// 各種シェーダの設定.
	void ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer );

private:
	// シェーダー作成.
	virtual HRESULT InitShader() override;
};

#endif	// #ifndef SIMPLE_SHADER_H.