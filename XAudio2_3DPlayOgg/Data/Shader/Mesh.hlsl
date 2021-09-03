//-------------------------------------------------.
// �O���[�o���ϐ�.
//-------------------------------------------------.
// �e�N�X�`��.
Texture2D		g_Texture	: register(t0);	// ���f���̃e�N�X�`��.
// �T���v��.
SamplerState	g_SamLinear	: register(s0);

//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
// ���b�V���P��.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	// ���[���h�s��.
	matrix	g_mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
};
// �}�e���A���P��.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// �A���r�G���g�F(���F).
	float4	g_vDiffuse;		// �f�B�q���[�Y�F(�g�U���ːF).
	float4	g_vSpecular;	// �X�y�L�����F(���ʔ��ːF).
};
// �t���[���P��.
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos;       //��׈ʒu.
	float4 g_vLightPos;     //ײĈʒu.
	float4 g_vLightDir;     //ײĕ���.
	matrix g_mLightRot;     //ײĉ�]�s��.
	float4 g_fIntensity;    //ײċ��x(���邳). ��x�̂ݎg�p����.
	float4 g_Color;         // �F.
	matrix g_LightWVP[4];
	float4 g_SpritPos;
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
// ���_�C���v�b�g.
struct VS_INPUT
{
	float4 Pos : POSITION;	// ���W.
	float4 Norm: NORMAL;	// �@��.m_pGround
	float2 Tex : TEXCOORD;	// �e�N�X�`�����W.
};
// ���_�A�E�g�v�b�g.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;	// WVP�ł̍��W.
	float4	PosW		: Position;		// W�ł̍��W.
	float4	Color		: COLOR;
	float3	LightDir	: TEXCOORD0;	// ���C�g����.
	float3	Normal		: TEXCOORD1;	// �@��.
	float3	EyeVector	: TEXCOORD2;	// ���_�x�N�g��.
	float2	Tex			: TEXCOORD3;	// �e�N�X�`�����W.
};

//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(input.Pos, g_mWVP);	// WVP���W.
	Out.PosW = mul(input.Pos, g_mW);	// world���W.
	Out.Normal = mul(input.Norm.xyz, (float3x3) g_mW);		// �@��.
	Out.LightDir = normalize(g_vLightDir).xyz;				// ���C�g����.
	Out.EyeVector = normalize(g_vCamPos - Out.PosW).xyz;	// ���_�x�N�g��.
	Out.Tex = input.Tex;	//�@�e�N�X�`�����W.

	return Out;
}

struct PS_OUTPUT
{
    float4 Color : SV_Target0;
};
//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);

    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Color = color;

    return output;
}

//========= �e�N�X�`�������p ========================================
//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(Pos, g_mWVP);	// WVP���W.
	Out.PosW = mul(Pos, g_mW);	// world���W.
	Out.Normal = mul(Norm.xyz, (float3x3) g_mW);		// �@��.
	Out.LightDir = normalize(g_vLightDir).xyz;				// ���C�g����.
	Out.EyeVector = normalize(g_vCamPos - Out.PosW).xyz;	// ���_�x�N�g��.

	return Out;
}

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
PS_OUTPUT PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    output.Color = color;

    return output;
}