#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "..\Camera.h"

class CSimpleCamera : public CCamera
{
public:
	// �J�����̈ړ��s���̏��.
	enum class enCameraMovePoState
	{
		None = -1,
		MoveTargetPos,		// �ړ�.
		End,
	}typedef EMovePosState;
	// �J�����̎��_�ړ��s���̏��.
	enum class enCameraMoveLookState
	{
		None = -1,
		MoveLookAt,		// �ړ�.
		End,
	}typedef EMoveLook;
public:
	CSimpleCamera();
	virtual ~CSimpleCamera();
	// �ȉ��̓��Update�͏펞�Ă�ł����v.
	// ���W������Ƃ����Ɉړ�����.
	void MoveUpdate(const D3DXVECTOR3& vMovePos,const bool& LookFront = false);
	// ���_�ړ�.
	void LookAtUpdate(const D3DXVECTOR3& vLookPos);
	// �J�����̈ړ����x�Z�b�g(0�Œ�~).
	inline void SetMovePosSpeed(const float& fSpeed) { m_fMoveSpeed = fSpeed; }
	// �J�����̎��_�ړ����x�Z�b�g.
	inline void SetLookAtLeapSpped(const float& fSpeed) { m_fLookLeapSpeed = fSpeed; }
	inline void ResetMovePosEnd() { m_IsPosMoveEnd = false; }			// �ړ��I���t���O�̏�����.
	inline const bool GetMovePosEnd() { return m_IsPosMoveEnd; }		// �ړ��I���t���O�̎擾.
	inline void ResetMoveLookAtEnd() { m_IsLookAtMoveEnd = false; }		// ���_�ړ��I���t���O�̏�����.
	inline const bool GetMoveLookAtEnd() { return m_IsLookAtMoveEnd; }	// ���_�ړ��I���t���O�̎擾.
private:
	void MoveForward(const bool& LookFront = false);	// �O�i.
private:
	EMovePosState	m_enCamMoveState;			// ���݂̃J�����ړ��X�e�[�g.
	EMoveLook		m_enLookAtState;			// ���_�ړ��X�e�[�g.
	D3DXVECTOR3		m_vTargetVector;			// �ړ��ʒu�ւ̃x�N�g��.
	D3DXVECTOR3		m_vOldMovePos;
	float			m_fLookLeapSpeed;
	float			m_fMoveSpeed;
	bool			m_IsPosMoveEnd;
	bool			m_IsLookAtMoveEnd;

};



#endif // !SIMPLE_CAMERA_H
