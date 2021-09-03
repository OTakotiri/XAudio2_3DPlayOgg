// グローバル.
cbuffer global
{
	matrix g_mWVP;
	float4 g_vColor;
};

// 頂点シェーダー.
float4 VS_Main( float4 Pos : POSITION ) : SV_Position
{
	Pos = mul(Pos, g_mWVP);
	return Pos;
}

// ピクセルシェーダー.
float4 PS_Main( float4 Pos : SV_Position ) : SV_Target
{
	return g_vColor;
}