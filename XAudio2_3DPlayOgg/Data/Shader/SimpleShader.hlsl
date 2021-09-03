// �O���[�o��.
cbuffer global
{
	matrix g_mWVP;
	float4 g_vColor;
};

// ���_�V�F�[�_�[.
float4 VS_Main( float4 Pos : POSITION ) : SV_Position
{
	Pos = mul(Pos, g_mWVP);
	return Pos;
}

// �s�N�Z���V�F�[�_�[.
float4 PS_Main( float4 Pos : SV_Position ) : SV_Target
{
	return g_vColor;
}