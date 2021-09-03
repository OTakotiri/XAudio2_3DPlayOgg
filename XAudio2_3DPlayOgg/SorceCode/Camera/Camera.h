#ifndef CAMERA_H
#define CAMERA_H

#include "..\Main\Global.h"

//===========================.
//	カメラクラス.
//===========================.
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	// カメラの更新処理.
	void Update();
	// ビュー行列、プロジェクション行列の初期化.
	void InitViewProj();

	// ビュー行列取得関数.
	D3DXMATRIX& GetViewMatrix() { return m_ViewMatrix; }
	// プロジェクション行列取得関数.
	D3DXMATRIX& GetProjMatrix() { return m_ProjMatrix; }
	// カメラ座標取得関数.
	D3DXVECTOR3 GetPosition() { return m_vPosition; }
	// カメラ注視座標取得関数.
	D3DXVECTOR3 GetLookPosition() { return m_vLookPosition; }
	// カメラ移動速度取得関数.
	float GetMoveSpeed() const { return m_fMoveSpeed; }

	// 上方向ベクトル取得関数.
	D3DXVECTOR3& GetUpVector() { return m_vUpVector; }
	// カメラ座標設定関数.
	void SetPosition( const D3DXVECTOR3& vPos )			{ m_vPosition = vPos; }
	// カメラ注視座標設定関数.
	void SetLookPosition( const D3DXVECTOR3& vLookPos )	{ m_vLookPosition = vLookPos; }
	// カメラ移動速度設定関数.
	void SetMoveSpeed(const float& fSpeed) { m_fMoveSpeed = fSpeed; }

	// 上方向ベクトルセット関数.
	void SetUpVector(const D3DXVECTOR3& vUpVec) { m_vUpVector = vUpVec; }
	// 継承先のカメラ情報の設定関数.
	void SetCamera( std::shared_ptr<CCamera> camera );
	// 指定したカメラクラスポインタに今セットされているカメラの座標情報を設定する.
	void GetCameraValue(std::shared_ptr<CCamera> camera);
	// 指定三次元座標を二次元座標に変換する関数.
	D3DXVECTOR2 WorldToScreenPos(const D3DXVECTOR3& Pos);
protected:
	D3DXVECTOR3	m_vPosition;					// カメラ位置.
	D3DXVECTOR3	m_vLookPosition;				// カメラ注視位置.
	float		m_fMoveSpeed;					// カメラ移動速度.

	D3DXVECTOR3 AxisZProc(const D3DXVECTOR3& Rot);	// Z軸ベクトル移動.
	D3DXVECTOR3 AxisZProc(float x,float y,float z);	// Z軸ベクトル移動.
protected:

private:
	D3DXMATRIX m_ViewMatrix;		// ビュー行列.
	D3DXMATRIX m_ProjMatrix;		// プロジェクション行列.
	
	D3DXVECTOR3 m_vUpVector;		// 上方向ベクトル.
	float m_fViewingAngle;			// 視野角.
	float m_fMaxClippingDistance;	// 最大クリッピング距離.
	float m_fMinClippingDistance;	// 最小クリッピング距離.
};

#endif	// #ifndef CAMERA_H.