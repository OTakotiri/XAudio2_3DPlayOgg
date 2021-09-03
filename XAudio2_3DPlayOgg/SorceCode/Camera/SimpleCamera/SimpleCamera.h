#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "..\Camera.h"

class CSimpleCamera : public CCamera
{
public:
	// カメラの移動行動の状態.
	enum class enCameraMovePoState
	{
		None = -1,
		MoveTargetPos,		// 移動.
		End,
	}typedef EMovePosState;
	// カメラの視点移動行動の状態.
	enum class enCameraMoveLookState
	{
		None = -1,
		MoveLookAt,		// 移動.
		End,
	}typedef EMoveLook;
public:
	CSimpleCamera();
	virtual ~CSimpleCamera();
	// 以下の二つのUpdateは常時呼んでも大丈夫.
	// 座標を入れるとそこに移動する.
	void MoveUpdate(const D3DXVECTOR3& vMovePos,const bool& LookFront = false);
	// 視点移動.
	void LookAtUpdate(const D3DXVECTOR3& vLookPos);
	// カメラの移動速度セット(0で停止).
	inline void SetMovePosSpeed(const float& fSpeed) { m_fMoveSpeed = fSpeed; }
	// カメラの視点移動速度セット.
	inline void SetLookAtLeapSpped(const float& fSpeed) { m_fLookLeapSpeed = fSpeed; }
	inline void ResetMovePosEnd() { m_IsPosMoveEnd = false; }			// 移動終了フラグの初期化.
	inline const bool GetMovePosEnd() { return m_IsPosMoveEnd; }		// 移動終了フラグの取得.
	inline void ResetMoveLookAtEnd() { m_IsLookAtMoveEnd = false; }		// 視点移動終了フラグの初期化.
	inline const bool GetMoveLookAtEnd() { return m_IsLookAtMoveEnd; }	// 視点移動終了フラグの取得.
private:
	void MoveForward(const bool& LookFront = false);	// 前進.
private:
	EMovePosState	m_enCamMoveState;			// 現在のカメラ移動ステート.
	EMoveLook		m_enLookAtState;			// 視点移動ステート.
	D3DXVECTOR3		m_vTargetVector;			// 移動位置へのベクトル.
	D3DXVECTOR3		m_vOldMovePos;
	float			m_fLookLeapSpeed;
	float			m_fMoveSpeed;
	bool			m_IsPosMoveEnd;
	bool			m_IsLookAtMoveEnd;

};



#endif // !SIMPLE_CAMERA_H
