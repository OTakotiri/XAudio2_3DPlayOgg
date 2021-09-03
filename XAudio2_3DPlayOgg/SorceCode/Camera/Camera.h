#ifndef CAMERA_H
#define CAMERA_H

#include "..\Main\Global.h"

//===========================.
//	�J�����N���X.
//===========================.
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	// �J�����̍X�V����.
	void Update();
	// �r���[�s��A�v���W�F�N�V�����s��̏�����.
	void InitViewProj();

	// �r���[�s��擾�֐�.
	D3DXMATRIX& GetViewMatrix() { return m_ViewMatrix; }
	// �v���W�F�N�V�����s��擾�֐�.
	D3DXMATRIX& GetProjMatrix() { return m_ProjMatrix; }
	// �J�������W�擾�֐�.
	D3DXVECTOR3 GetPosition() { return m_vPosition; }
	// �J�����������W�擾�֐�.
	D3DXVECTOR3 GetLookPosition() { return m_vLookPosition; }
	// �J�����ړ����x�擾�֐�.
	float GetMoveSpeed() const { return m_fMoveSpeed; }

	// ������x�N�g���擾�֐�.
	D3DXVECTOR3& GetUpVector() { return m_vUpVector; }
	// �J�������W�ݒ�֐�.
	void SetPosition( const D3DXVECTOR3& vPos )			{ m_vPosition = vPos; }
	// �J�����������W�ݒ�֐�.
	void SetLookPosition( const D3DXVECTOR3& vLookPos )	{ m_vLookPosition = vLookPos; }
	// �J�����ړ����x�ݒ�֐�.
	void SetMoveSpeed(const float& fSpeed) { m_fMoveSpeed = fSpeed; }

	// ������x�N�g���Z�b�g�֐�.
	void SetUpVector(const D3DXVECTOR3& vUpVec) { m_vUpVector = vUpVec; }
	// �p����̃J�������̐ݒ�֐�.
	void SetCamera( std::shared_ptr<CCamera> camera );
	// �w�肵���J�����N���X�|�C���^�ɍ��Z�b�g����Ă���J�����̍��W����ݒ肷��.
	void GetCameraValue(std::shared_ptr<CCamera> camera);
	// �w��O�������W��񎟌����W�ɕϊ�����֐�.
	D3DXVECTOR2 WorldToScreenPos(const D3DXVECTOR3& Pos);
protected:
	D3DXVECTOR3	m_vPosition;					// �J�����ʒu.
	D3DXVECTOR3	m_vLookPosition;				// �J���������ʒu.
	float		m_fMoveSpeed;					// �J�����ړ����x.

	D3DXVECTOR3 AxisZProc(const D3DXVECTOR3& Rot);	// Z���x�N�g���ړ�.
	D3DXVECTOR3 AxisZProc(float x,float y,float z);	// Z���x�N�g���ړ�.
protected:

private:
	D3DXMATRIX m_ViewMatrix;		// �r���[�s��.
	D3DXMATRIX m_ProjMatrix;		// �v���W�F�N�V�����s��.
	
	D3DXVECTOR3 m_vUpVector;		// ������x�N�g��.
	float m_fViewingAngle;			// ����p.
	float m_fMaxClippingDistance;	// �ő�N���b�s���O����.
	float m_fMinClippingDistance;	// �ŏ��N���b�s���O����.
};

#endif	// #ifndef CAMERA_H.