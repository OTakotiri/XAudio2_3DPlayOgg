#include "CameraManager.h"
#include "..\Camera.h"

CCameraManager::CCameraManager()
	: m_pCamera		( nullptr )
	, m_pOldCamera	( nullptr )
{
}

CCameraManager::~CCameraManager()
{
}
//=====================================
// �J�����N���X�̐ݒ�.
//=====================================
void CCameraManager::SetCamera(std::shared_ptr<CCamera> pCamera)
{
	if (GetInstance()->m_pOldCamera == pCamera) return;
	GetInstance()->m_pCamera = pCamera;
	GetInstance()->m_pOldCamera = pCamera;

}
//=====================================
// �w�肵���J�����N���X�|�C���^�ɍ��Z�b�g����Ă���J�����̍��W����ݒ肷��.
//=====================================
void CCameraManager::GetNowCamera(std::shared_ptr<CCamera> pCamera)
{
	GetInstance()->m_pCamera->GetCameraValue(pCamera);
}
//=====================================
// �X�V�֐�.
//=====================================
void CCameraManager::Update()
{
	if (GetInstance()->m_pCamera == nullptr) return;
	GetInstance()->m_pCamera->Update();
}
//=====================================
// �r���[�s��A�v���W�F�N�V�����s��̏�����.
//=====================================
void CCameraManager::InitViewProj()
{
	if (GetInstance()->m_pCamera == nullptr) return;
	GetInstance()->m_pCamera->InitViewProj();
}
//=====================================
// �r���[�s��擾�֐�.
//=====================================
const D3DXMATRIX CCameraManager::GetViewMatrix()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetViewMatrix();
}
//=====================================
// �v���W�F�N�V�����s��擾�֐�.
//=====================================
const D3DXMATRIX CCameraManager::GetProjMatrix()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetProjMatrix();
}
//=====================================
// �J�������W�擾�֐�.
//=====================================
const D3DXVECTOR3 CCameraManager::GetPosition()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetPosition();
}
//=====================================
// �J�����������W�擾�֐�.
//=====================================
const D3DXVECTOR3 CCameraManager::GetLookPosition()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetLookPosition();
}
//=====================================
// �J�����̈ړ����x�擾�֐�.
//=====================================
const float CCameraManager::GetMoveSpeed()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetMoveSpeed();
}
//=====================================
// ������x�N�g���擾�֐�.
//=====================================
const D3DXVECTOR3 CCameraManager::GetUpVector()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetUpVector();
}
//=====================================
// �Ώۂ̍��W���X�N���[�����W�ŕԂ��֐�.
//=====================================
const D3DXVECTOR2 CCameraManager::GetWorldToScreenPos(const D3DXVECTOR3 & Pos)
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->WorldToScreenPos(Pos);
}
//=====================================
// �J�����������Ă�������̌����x�N�g����Ԃ��֐�.
//=====================================
const D3DXVECTOR3 CCameraManager::GetCameraDirVector()
{
	D3DXMATRIX Tes = GetViewMatrix();
	D3DXVECTOR3 Vec;
	D3DXMatrixTranspose(&Tes, &Tes);//�s���]�u����.
	Vec.x = Tes._31;
	Vec.y = Tes._32;
	Vec.z = Tes._33;
	return Vec;
}
//=====================================
// �J�����������Ă�������ɏ����i�񂾍��W��Ԃ��֐�.
//=====================================
const D3DXVECTOR3 CCameraManager::GetCameraFrontPosition()
{
	D3DXVECTOR3 tmp = GetInstance()->m_pCamera->GetPosition();

	tmp += GetCameraDirVector() * 4.0f;

	return tmp;
}
const bool CCameraManager::IsOutCamera(const D3DXVECTOR3 & vTargetPos, const float& fOutRad)
{
	float TargetRot = 0.0f;
	D3DXVECTOR3 MoveVector = { 0.0f, 0.0f, 0.0f };

	TargetRot = GetTwoPointRadian(vTargetPos, GetInstance()->m_pCamera->GetPosition());

	MoveVector.x = sinf(TargetRot);
	MoveVector.z = cosf(TargetRot);

	// �J�����̑O�x�N�g����p��.
	D3DXVECTOR3 myVector = GetCameraDirVector();

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(MoveVector.x*MoveVector.x + MoveVector.z*MoveVector.z);

	// �J��������Ώۂւ̃x�N�g���ƁA�J�����̑O�x�N�g���̊O�ς����߂�.
	float cross = myVector.x * MoveVector.z - myVector.z * MoveVector.x;
	float dot = myVector.x * MoveVector.x + myVector.z * MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// ���ς��������l�𒴂�����Ώۂ���ʊO�ɏo��.
	bool IsOffScreen = dot < static_cast<float>(D3DXToRadian(fOutRad)) ? true : false;

	// true�ŉ�ʊO.
	return IsOffScreen;
}
//=====================================
// �C���X�^���X�̎擾.
//=====================================
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance =
		std::make_unique<CCameraManager>();

	return pInstance.get();
}