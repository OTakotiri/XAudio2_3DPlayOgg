#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include "..\..\Main\Global.h"

class CShaderBase
{
public:
	CShaderBase();
	virtual ~CShaderBase();

	// 初期化.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) = 0;
	// 解放.
	virtual HRESULT Release() = 0;

	// シェーダー作成.
	virtual HRESULT InitShader() = 0;

protected:
	// シェーダー読込関数.
	static HRESULT InitShader(
		const char* filePath,
		const char* entryPoint,
		const char* shaderModel,
		UINT compileFlag,
		ID3DBlob** ppCompiledShader,
		ID3DBlob** ppErrors );

	// バーテックスシェーダー作成関数.
	HRESULT CreateVertexShader(
		ID3DBlob* pCompiledShader,
		ID3D11VertexShader** pVertexShader );

	// ピクセルスシェーダー作成関数.
	HRESULT CreatePixelShader(
		ID3DBlob* pCompiledShader,
		ID3D11PixelShader**	pPixelShader );

	// インプットレイアウト作成関数.
	HRESULT CreateInputLayout(
		D3D11_INPUT_ELEMENT_DESC* pLayout,
		UINT numElements,
		ID3DBlob* pCompiledShader, 
		ID3D11InputLayout** m_pVertexLayout );

	// コンスタントバッファ作成関数.
	HRESULT CreateConstantBuffer( 
		ID3D11Buffer** pConstantBuffer,
		UINT size );

	// 頂点インプットレイアウトの取得.
	D3D11_INPUT_ELEMENT_DESC GetPositionInputElement() const; 
	// TEXCOORDの取得.
	D3D11_INPUT_ELEMENT_DESC GetTexcoordInputElement() const;
protected:
	ID3D11Device*			m_pDevice11;		// デバイスオブジェクト.
	ID3D11DeviceContext*	m_pContext11;		// デバイスコンテキスト.

	ID3D11VertexShader*		m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*		m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*		m_pVertexLayout;	// 頂点レイアウト.
	ID3D11Buffer*			m_pConstantBuffer;	// コンスタントバッファ.
};

#endif	// #ifndef SHADER_BASE_H.