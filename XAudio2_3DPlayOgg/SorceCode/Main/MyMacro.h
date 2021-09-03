#ifndef MYMACRO_H
#define MYMACRO_H

#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// �uD3DX�`�v�̒�`�g�p�ɕK�v.
#include <D3D10.h>
#include <mutex>
#include <random>
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )
#pragma comment (lib, "winmm.lib") 
//===========================================================
//	ϸ�.
//===========================================================

//���.
#define SAFE_RELEASE(p)	if(p!=nullptr){(p)->Release();(p)=nullptr;}
//�j��.
#define SAFE_DELETE(p) if(p!=nullptr){delete (p);(p)=nullptr;}

#define SAFE_DELETE_ARRAY(p) if(p!=nullptr){delete[] (p);(p)=nullptr;}

#define ALIGN16	_declspec(align(16))
#define NoAma(n)    ((n) % 10 )		// ���̗]��o��.
#define NoDig(n)    ((n) /= 10 )	// ������.
//===============================================
// 2�_�̋����𑪂�֐�.
//====
static const float GetTwoPointDistance(const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vTargetPos) {
	float distance = sqrtf((vTargetPos.x - vMyPos.x) * (vTargetPos.x - vMyPos.x)
		+ (vTargetPos.y - vMyPos.y) * (vTargetPos.y - vMyPos.y)
		+ (vTargetPos.z - vMyPos.z) * (vTargetPos.z - vMyPos.z));

	return distance;
}
//===============================================
// 2�_�̊p�x�𑪂�֐�.
//====
static const float GetTwoPointRadian(const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos)
{
	float radian = atan2f(vtargetPos.x - vMyPos.x, vtargetPos.z - vMyPos.z);

	return radian;
}
//===============================================
// �^�[�Q�b�g���W���AMyPos���O�ɂ��邩���ɂ��邩.
//====
static const float TargetPosIsFrontOrRear(const D3DXVECTOR3& vMyFrontDir,const D3DXVECTOR3& MyPos, const D3DXVECTOR3& vTargetPos) {
	// �O����
	D3DXVECTOR3 direction = vMyFrontDir;
	// �����̈ʒu
	D3DXVECTOR3 position = MyPos;
	// �Ώۂ̍��W
	D3DXVECTOR3 TargetPos = vTargetPos;
	float result;
	// ��������̑��΍��W
	D3DXVECTOR3 relative;

	relative.x = TargetPos.x - position.x;
	relative.y = TargetPos.y - position.y;
	relative.z = TargetPos.z - position.z;

	result = direction.x * relative.x + direction.y * relative.y + direction.z * relative.z;
	// result���v���X = �Ώۍ��W�͎������O�ɂ���̂�true.
	// 0.0�͈ʒu.
	return result >= 0.0f ? true : false;
}
// string��wstirng�^�ɕϊ�.
static const std::wstring ConvertStringToWstring(std::string string)
{
	// �o�b�t�@�T�C�Y�擾.
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);
	// �o�b�t�@�̎擾.
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];
	// SJIS����wstring��.
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2
		, iBufferSize);
	// wstring�̐���.
	std::wstring ws(cpUCS2, cpUCS2 + iBufferSize - 1);
	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	return ws;
}
//=====================================
// �����_���l���쐬����֐�.
//=====================================
static float CreateRandomValue(float minRand, float maxRand)
{
	//�V�[�h�l����������
	std::random_device rnd;
	//�����Z���k�E�c�C�X�^�����g���ė��������
	std::mt19937_64 RandomNum(rnd());
	//�͈͓��̗��U���z�����
	std::uniform_real_distribution<float> genRandFloat(minRand, maxRand);
	//���z�̒����琶�������������g����1�����l�����o��
	return genRandFloat(RandomNum);
}
// �ςȂƂ����ShowCursor�֐����Ăё����Ȃ���Ί�{���[�v�͂��Ȃ�.
// �}�E�X�J�[�\���\��.
static void ShowMouseCursor()
{
	int count = ShowCursor(TRUE);
	while (count < 0)
	{
		count = ShowCursor(TRUE);
	}
}
// �ςȂƂ����ShowCursor�֐����Ăё����Ȃ���Ί�{���[�v�͂��Ȃ�.
// �}�E�X�J�[�\����\��.
static void HideMouseCursor()
{
	int count = ShowCursor(FALSE);
	while (count > -1)
	{
		count = ShowCursor(FALSE);
	}
}


#endif