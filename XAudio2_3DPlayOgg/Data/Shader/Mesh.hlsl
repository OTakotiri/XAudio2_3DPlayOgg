//-------------------------------------------------.
// グローバル変数.
//-------------------------------------------------.
// テクスチャ.
Texture2D		g_Texture	: register(t0);	// モデルのテクスチャ.
// サンプラ.
SamplerState	g_SamLinear	: register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// メッシュ単位.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	// ワールド行列.
	matrix	g_mWVP;	// ワールド,ビュー,プロジェクションの合成行列.
};
// マテリアル単位.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// アンビエント色(環境色).
	float4	g_vDiffuse;		// ディヒューズ色(拡散反射色).
	float4	g_vSpecular;	// スペキュラ色(鏡面反射色).
};
// フレーム単位.
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos;       //ｶﾒﾗ位置.
	float4 g_vLightPos;     //ﾗｲﾄ位置.
	float4 g_vLightDir;     //ﾗｲﾄ方向.
	matrix g_mLightRot;     //ﾗｲﾄ回転行列.
	float4 g_fIntensity;    //ﾗｲﾄ強度(明るさ). ※xのみ使用する.
	float4 g_Color;         // 色.
	matrix g_LightWVP[4];
	float4 g_SpritPos;
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点インプット.
struct VS_INPUT
{
	float4 Pos : POSITION;	// 座標.
	float4 Norm: NORMAL;	// 法線.m_pGround
	float2 Tex : TEXCOORD;	// テクスチャ座標.
};
// 頂点アウトプット.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;	// WVPでの座標.
	float4	PosW		: Position;		// Wでの座標.
	float4	Color		: COLOR;
	float3	LightDir	: TEXCOORD0;	// ライト方向.
	float3	Normal		: TEXCOORD1;	// 法線.
	float3	EyeVector	: TEXCOORD2;	// 視点ベクトル.
	float2	Tex			: TEXCOORD3;	// テクスチャ座標.
};

//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(input.Pos, g_mWVP);	// WVP座標.
	Out.PosW = mul(input.Pos, g_mW);	// world座標.
	Out.Normal = mul(input.Norm.xyz, (float3x3) g_mW);		// 法線.
	Out.LightDir = normalize(g_vLightDir).xyz;				// ライト方向.
	Out.EyeVector = normalize(g_vCamPos - Out.PosW).xyz;	// 視点ベクトル.
	Out.Tex = input.Tex;	//　テクスチャ座標.

	return Out;
}

struct PS_OUTPUT
{
    float4 Color : SV_Target0;
};
//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);

    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Color = color;

    return output;
}

//========= テクスチャ無し用 ========================================
//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(Pos, g_mWVP);	// WVP座標.
	Out.PosW = mul(Pos, g_mW);	// world座標.
	Out.Normal = mul(Norm.xyz, (float3x3) g_mW);		// 法線.
	Out.LightDir = normalize(g_vLightDir).xyz;				// ライト方向.
	Out.EyeVector = normalize(g_vCamPos - Out.PosW).xyz;	// 視点ベクトル.

	return Out;
}

//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
PS_OUTPUT PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Color = color;

    return output;
}