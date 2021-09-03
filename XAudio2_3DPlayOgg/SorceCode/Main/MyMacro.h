#ifndef MYMACRO_H
#define MYMACRO_H

#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// 「D3DX～」の定義使用に必要.
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
//	ﾏｸﾛ.
//===========================================================

//解放.
#define SAFE_RELEASE(p)	if(p!=nullptr){(p)->Release();(p)=nullptr;}
//破棄.
#define SAFE_DELETE(p) if(p!=nullptr){delete (p);(p)=nullptr;}

#define SAFE_DELETE_ARRAY(p) if(p!=nullptr){delete[] (p);(p)=nullptr;}

#define ALIGN16	_declspec(align(16))
#define NoAma(n)    ((n) % 10 )		// 桁の余り出し.
#define NoDig(n)    ((n) /= 10 )	// 桁だし.
//===============================================
// 2点の距離を測る関数.
//====
static const float GetTwoPointDistance(const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vTargetPos) {
	float distance = sqrtf((vTargetPos.x - vMyPos.x) * (vTargetPos.x - vMyPos.x)
		+ (vTargetPos.y - vMyPos.y) * (vTargetPos.y - vMyPos.y)
		+ (vTargetPos.z - vMyPos.z) * (vTargetPos.z - vMyPos.z));

	return distance;
}
//===============================================
// 2点の角度を測る関数.
//====
static const float GetTwoPointRadian(const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos)
{
	float radian = atan2f(vtargetPos.x - vMyPos.x, vtargetPos.z - vMyPos.z);

	return radian;
}
//===============================================
// ターゲット座標が、MyPosより前にいるか後ろにいるか.
//====
static const float TargetPosIsFrontOrRear(const D3DXVECTOR3& vMyFrontDir,const D3DXVECTOR3& MyPos, const D3DXVECTOR3& vTargetPos) {
	// 前方向
	D3DXVECTOR3 direction = vMyFrontDir;
	// 自分の位置
	D3DXVECTOR3 position = MyPos;
	// 対象の座標
	D3DXVECTOR3 TargetPos = vTargetPos;
	float result;
	// 自分からの相対座標
	D3DXVECTOR3 relative;

	relative.x = TargetPos.x - position.x;
	relative.y = TargetPos.y - position.y;
	relative.z = TargetPos.z - position.z;

	result = direction.x * relative.x + direction.y * relative.y + direction.z * relative.z;
	// resultがプラス = 対象座標は自分より前にいるのでtrue.
	// 0.0は位置.
	return result >= 0.0f ? true : false;
}
// stringをwstirng型に変換.
static const std::wstring ConvertStringToWstring(std::string string)
{
	// バッファサイズ取得.
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);
	// バッファの取得.
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];
	// SJISからwstringに.
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2
		, iBufferSize);
	// wstringの生成.
	std::wstring ws(cpUCS2, cpUCS2 + iBufferSize - 1);
	// バッファの破棄
	delete[] cpUCS2;

	return ws;
}
//=====================================
// ランダム値を作成する関数.
//=====================================
static float CreateRandomValue(float minRand, float maxRand)
{
	//シード値乱数生成器
	std::random_device rnd;
	//メルセンヌ・ツイスタ方を使って乱数を作る
	std::mt19937_64 RandomNum(rnd());
	//範囲内の離散分布を作る
	std::uniform_real_distribution<float> genRandFloat(minRand, maxRand);
	//分布の中から生成した乱数を使って1つだけ値を取り出す
	return genRandFloat(RandomNum);
}
// 変なところでShowCursor関数を呼び続けなければ基本ループはしない.
// マウスカーソル表示.
static void ShowMouseCursor()
{
	int count = ShowCursor(TRUE);
	while (count < 0)
	{
		count = ShowCursor(TRUE);
	}
}
// 変なところでShowCursor関数を呼び続けなければ基本ループはしない.
// マウスカーソル非表示.
static void HideMouseCursor()
{
	int count = ShowCursor(FALSE);
	while (count > -1)
	{
		count = ShowCursor(FALSE);
	}
}


#endif