#include "SimpleCamera.h"
#include "..\..\Main\MyMacro.h"
CSimpleCamera::CSimpleCamera()
	: m_enCamMoveState	( EMovePosState::MoveTargetPos )
	, m_enLookAtState	( EMoveLook::MoveLookAt )
	, m_vTargetVector	( 0.0f,0.0f,0.0f )
	, m_fLookLeapSpeed	( 0.08f )
	, m_fMoveSpeed		( 0.2f )
	, m_IsPosMoveEnd	( false )
	, m_IsLookAtMoveEnd	( false )
{
}

CSimpleCamera::~CSimpleCamera()
{
}
//=====================================
// 注視座標を更新する.
//=====================================
void CSimpleCamera::MoveUpdate(const D3DXVECTOR3& vMovePos, const bool& LookFront)
{
	SetUpVector(D3DXVECTOR3(0.0f,1.0f,0.0f));
	float Distance = 0.0f;
	switch (m_enCamMoveState)
	{
	case EMovePosState::MoveTargetPos:
		Distance = GetTwoPointDistance(m_vPosition, vMovePos);
		if (Distance <= 0.6f * m_fMoveSpeed) {
			m_IsPosMoveEnd = true;
			m_vPosition = vMovePos;
			break;
		}
		else {
			m_IsPosMoveEnd = false;
		}
		m_vTargetVector = vMovePos - m_vPosition;					// 移動方向へのベクトル計算.
		D3DXVec3Normalize(&m_vTargetVector, &m_vTargetVector);
		MoveForward(LookFront);		// 移動関数.
		break;
	case EMovePosState::End:
		m_IsPosMoveEnd = false;
		break;
	default:
		break;
	}
}
//=====================================
// 注視座標を更新する.
//=====================================
void CSimpleCamera::LookAtUpdate(const D3DXVECTOR3 & vLookPos)
{
	float Distance = 0.0f;
	switch (m_enLookAtState)
	{
	case EMoveLook::MoveLookAt:
		Distance = GetTwoPointDistance(m_vLookPosition, vLookPos);
		if (Distance <= 0.6f + m_fLookLeapSpeed ) {
			m_IsLookAtMoveEnd = true;
			SetLookPosition(m_vLookPosition);
			break;
		}
		else {
			m_IsLookAtMoveEnd = false;
		}
		*D3DXVec3Lerp(&m_vLookPosition, &m_vLookPosition, &vLookPos, m_fLookLeapSpeed);
		SetLookPosition(m_vLookPosition);
		break;
	case EMoveLook::End:
		m_IsLookAtMoveEnd = false;
		break;
	default:
		break;
	}
}
//=====================================
// 注視座標を更新する.
//=====================================
void CSimpleCamera::MoveForward(const bool& LookFront)
{
	D3DXVECTOR3 vTmpPos;
	vTmpPos = m_vPosition;

	if (m_IsPosMoveEnd == true) return;

	vTmpPos += m_vTargetVector * m_fMoveSpeed;
	m_vPosition = vTmpPos;
	if (LookFront == false)return;
	D3DXVECTOR3	vFrontPos = m_vPosition + m_vTargetVector * 4.0f;

	*D3DXVec3Lerp(&m_vLookPosition, &m_vLookPosition, &vFrontPos, 0.3f);
	SetLookPosition(m_vLookPosition);
}