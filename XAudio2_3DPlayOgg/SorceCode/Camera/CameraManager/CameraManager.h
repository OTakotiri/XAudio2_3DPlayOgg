#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "..\..\Main\Global.h"

class CCamera;

class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	// カメラの設定.
	static void SetCamera(std::shared_ptr<CCamera> pCamera);
	// 指定したカメラクラスポインタに今セットされているカメラの座標情報を設定する.
	static void GetNowCamera(std::shared_ptr<CCamera> pCamera);
	// 更新関数.
	static void Update();
	// ビュー行列、プロジェクション行列の初期化.
	static void InitViewProj();

	// ビュー行列取得関数.
	static const D3DXMATRIX GetViewMatrix();
	// プロジェクション行列取得関数.
	static const D3DXMATRIX GetProjMatrix();
	// カメラ座標取得関数.
	static const D3DXVECTOR3 GetPosition();
	// カメラ注視座標取得関数.
	static const D3DXVECTOR3 GetLookPosition();
	// カメラ移動速度取得関数.
	static const float GetMoveSpeed();
	// 上方向ベクトル取得関数.
	static const D3DXVECTOR3 GetUpVector();
	// 対象の座標をスクリーン座標で返す関数.
	static const D3DXVECTOR2 GetWorldToScreenPos(const D3DXVECTOR3& Pos);
	// カメラが向いている方向の向きベクトルを返す関数.
	static const D3DXVECTOR3 GetCameraDirVector();
	// カメラが向いている方向に少し進んだ座標を返す関数.
	static const D3DXVECTOR3 GetCameraFrontPosition();
	// カメラにうつっているかを返す関数.
	static const bool IsOutCamera(const D3DXVECTOR3& vTargetPos, const float& fOutRad = 140.0f);
private:
	std::shared_ptr<CCamera> m_pCamera;	// カメラクラス.
	std::shared_ptr<CCamera> m_pOldCamera;	// カメラクラス.

private:
	// インスタンスの取得.
	static CCameraManager* GetInstance();
};



#endif // #ifndef CAMERA_MANAGER_H.