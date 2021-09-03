#include "Camera.h"
#include <random>
#include "..\Common\D3DX\D3DX.h"
#include "..\Main\MyMacro.h"

namespace {
	constexpr float FLOAT_WND_W = static_cast<float>(WND_W);			// (float)��ʕ�.
	constexpr float FLOAT_WND_H = static_cast<float>(WND_H);			// (float)��ʕ�.
	constexpr float VIEW_UP_VECTOR[3] = { 0.0f, 1.0f, 0.0f };			// ������x�N�g��.
	constexpr float VIEWING_ANGLE = static_cast<float>(D3DX_PI / 4.0);	// ����p.
	constexpr float SCREEN_ASPECT = FLOAT_WND_W / FLOAT_WND_H;			// ��ʃA�X�y�N�g��.
	constexpr float MAX_CLIPPING_DISTANCE = 400.0f;						// �ő�`�拗��.
	constexpr float MIN_CLIPPING_DISTANCE = 0.1f;						// �ŏ��`�拗��.
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
// �J�����̍X�V����.
//====
void CCamera::Update()
{
}

//===============================================
// �r���[�s��A�v���W�F�N�V�����s��̏�����.
//====
void CCamera::InitViewProj()
{
	// �r���[(�J����)�ϊ�.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&m_vPosition,		// �J�������W.
		&m_vLookPosition,	// �J�����������W.
		&m_vUpVector);	// �J�����x�N�g��.

	// �v���W�F�N�V����(�ˉe)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		m_fViewingAngle,				// ����p.
		SCREEN_ASPECT,				// ��ʃA�X�y�N�g��.
		m_fMinClippingDistance,		// �ŏ��`�拗��.
		m_fMaxClippingDistance );	// �ő�`�拗��.
}

//===============================================
// �p����̃J�������̐ݒ�֐�.
//====
void CCamera::SetCamera( std::shared_ptr<CCamera> camera )
{
	this->m_vPosition		= camera->GetPosition();
	this->m_vLookPosition	= camera->GetLookPosition();
}
//===============================================
// �w�肵���J�����N���X�|�C���^�ɍ��Z�b�g����Ă���J�����̍��W����ݒ肷��.
//====
void CCamera::GetCameraValue(std::shared_ptr<CCamera> camera)
{
	camera->SetLookPosition(this->m_vLookPosition);
	camera->SetPosition(this->m_vPosition);
}
D3DXVECTOR2 CCamera::WorldToScreenPos(const D3DXVECTOR3& Pos)
{
	D3DXVECTOR3 screenOutPos;

	// ���[���h�ϊ��s��.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);	// �P�ʍs��쐬.

	// �r���[�|�[�g�s��.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity(&mViewport);	// �P�ʍs��쐬.
	mViewport._11 = static_cast<float>(CDirectXDevice::GetWidth()) / 2.0f;
	mViewport._22 = static_cast<float>(CDirectXDevice::GetHeight()) / -2.0f;
	mViewport._41 = static_cast<float>(CDirectXDevice::GetWidth()) / 2.0f;
	mViewport._42 = static_cast<float>(CDirectXDevice::GetHeight()) / 2.0f;

	// ���[���h, �r���[, �v���W�F, �r���[�|�[�g�̌v�Z.
	D3DXMATRIX m = mWorld * GetViewMatrix() * GetProjMatrix() * mViewport;
	// D3DXVECTOR3�Ɗ|�����킹.
	D3DXVec3TransformCoord(&screenOutPos, &Pos, &m);

	// �v�Z���ʂ�x, y���W��Ԃ�. 
	return D3DXVECTOR2(screenOutPos.x, screenOutPos.y);
}
D3DXVECTOR3 CCamera::AxisZProc(float x, float y, float z)
{
	// Z���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	// �ϊ��O�x�N�g��.

	D3DXMATRIX mYaw, mPitch, mRoll, mRot;

	// Y����].
	D3DXMatrixRotationY(&mYaw, y);
	//X����]�s��쐬.
	D3DXMatrixRotationX(&mPitch, x);
	//Z����]�s��쐬.
	D3DXMatrixRotationZ(&mRoll, z);

	//��]�s����쐬.
	mRot = mYaw * mPitch * mRoll;

	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(&vecAxisZ,	// (out)�ԊҌ�x�N�g��.
		&vecAxisZ, &mRot);

	return vecAxisZ;
}

D3DXVECTOR3 CCamera::AxisZProc(const D3DXVECTOR3& Rot)
{
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);	// �ϊ��O�x�N�g��.

	D3DXMATRIX mYaw, mPitch, mRoll, mRot;

	// Y����].
	D3DXMatrixRotationY(&mYaw, Rot.y);
	//X����]�s��쐬.
	D3DXMatrixRotationX(&mPitch, Rot.x);
	//Z����]�s��쐬.
	D3DXMatrixRotationZ(&mRoll, Rot.z);

	//��]�s����쐬.
	mRot = mYaw * mPitch * mRoll;

	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(&vecAxisZ,	// (out)�ԊҌ�x�N�g��.
		&vecAxisZ, &mRot);

	return vecAxisZ;
}
