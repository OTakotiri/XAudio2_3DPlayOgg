#include "Camera.h"
#include <random>
#include "..\Common\D3DX\D3DX.h"
#include "..\Main\MyMacro.h"

namespace {
	constexpr float FLOAT_WND_W = static_cast<float>(WND_W);			// (float)画面幅.
	constexpr float FLOAT_WND_H = static_cast<float>(WND_H);			// (float)画面幅.
	constexpr float VIEW_UP_VECTOR[3] = { 0.0f, 1.0f, 0.0f };			// 上方向ベクトル.
	constexpr float VIEWING_ANGLE = static_cast<float>(D3DX_PI / 4.0);	// 視野角.
	constexpr float SCREEN_ASPECT = FLOAT_WND_W / FLOAT_WND_H;			// 画面アスペクト比.
	constexpr float MAX_CLIPPING_DISTANCE = 400.0f;						// 最大描画距離.
	constexpr float MIN_CLIPPING_DISTANCE = 0.1f;						// 最小描画距離.
}

CCamera::CCamera()
	: m_ViewMatrix()
	, m_ProjMatrix()
	, m_vPosition			( 0.0f, 10.0f, -5.0f )
	, m_vLookPosition		( 0.0f, 0.0f, 0.0f )
	, m_fMoveSpeed			( 0.0f )
	, m_vUpVector			( VIEW_UP_VECTOR )
	, m_fViewingAngle		( VIEWING_ANGLE )
	, m_fMaxClippingDistance	( MAX_CLIPPING_DISTANCE )
	, m_fMinClippingDistance	( MIN_CLIPPING_DISTANCE )
{
}

CCamera::~CCamera()
{
}

//===============================================
// カメラの更新処理.
//====
void CCamera::Update()
{
}

//===============================================
// ビュー行列、プロジェクション行列の初期化.
//====
void CCamera::InitViewProj()
{
	// ビュー(カメラ)変換.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&m_vPosition,		// カメラ座標.
		&m_vLookPosition,	// カメラ注視座標.
		&m_vUpVector);	// カメラベクトル.

	// プロジェクション(射影)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_fViewingAngle,				// 視野角.
		SCREEN_ASPECT,				// 画面アスペクト比.
		m_fMinClippingDistance,		// 最小描画距離.
		m_fMaxClippingDistance );	// 最大描画距離.
}

//===============================================
// 継承先のカメラ情報の設定関数.
//====
void CCamera::SetCamera( std::shared_ptr<CCamera> camera )
{
	this->m_vPosition		= camera->GetPosition();
	this->m_vLookPosition	= camera->GetLookPosition();
}
//===============================================
// 指定したカメラクラスポインタに今セットされているカメラの座標情報を設定する.
//====
void CCamera::GetCameraValue(std::shared_ptr<CCamera> camera)
{
	camera->SetLookPosition(this->m_vLookPosition);
	camera->SetPosition(this->m_vPosition);
}
D3DXVECTOR2 CCamera::WorldToScreenPos(const D3DXVECTOR3& Pos)
{
	D3DXVECTOR3 screenOutPos;

	// ワールド変換行列.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);	// 単位行列作成.

	// ビューポート行列.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity(&mViewport);	// 単位行列作成.
	mViewport._11 = static_cast<float>(CDirectXDevice::GetWidth()) / 2.0f;
	mViewport._22 = static_cast<float>(CDirectXDevice::GetHeight()) / -2.0f;
	mViewport._41 = static_cast<float>(CDirectXDevice::GetWidth()) / 2.0f;
	mViewport._42 = static_cast<float>(CDirectXDevice::GetHeight()) / 2.0f;

	// ワールド, ビュー, プロジェ, ビューポートの計算.
	D3DXMATRIX m = mWorld * GetViewMatrix() * GetProjMatrix() * mViewport;
	// D3DXVECTOR3と掛け合わせ.
	D3DXVec3TransformCoord(&screenOutPos, &Pos, &m);

	// 計算結果のx, y座標を返す. 
	return D3DXVECTOR2(screenOutPos.x, screenOutPos.y);
}
D3DXVECTOR3 CCamera::AxisZProc(float x, float y, float z)
{
	// Z軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	// 変換前ベクトル.

	D3DXMATRIX mYaw, mPitch, mRoll, mRot;

	// Y軸回転.
	D3DXMatrixRotationY(&mYaw, y);
	//X軸回転行列作成.
	D3DXMatrixRotationX(&mPitch, x);
	//Z軸回転行列作成.
	D3DXMatrixRotationZ(&mRoll, z);

	//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	// Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(&vecAxisZ,	// (out)返還後ベクトル.
		&vecAxisZ, &mRot);

	return vecAxisZ;
}

D3DXVECTOR3 CCamera::AxisZProc(const D3DXVECTOR3& Rot)
{
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	// 変換前ベクトル.

	D3DXMATRIX mYaw, mPitch, mRoll, mRot;

	// Y軸回転.
	D3DXMatrixRotationY(&mYaw, Rot.y);
	//X軸回転行列作成.
	D3DXMatrixRotationX(&mPitch, Rot.x);
	//Z軸回転行列作成.
	D3DXMatrixRotationZ(&mRoll, Rot.z);

	//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	// Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(&vecAxisZ,	// (out)返還後ベクトル.
		&vecAxisZ, &mRot);

	return vecAxisZ;
}
