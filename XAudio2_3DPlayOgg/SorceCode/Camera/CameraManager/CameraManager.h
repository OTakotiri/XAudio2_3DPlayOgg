#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "..\..\Main\Global.h"

class CCamera;

class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	// �J�����̐ݒ�.
	static void SetCamera(std::shared_ptr<CCamera> pCamera);
	// �w�肵���J�����N���X�|�C���^�ɍ��Z�b�g����Ă���J�����̍��W����ݒ肷��.
	static void GetNowCamera(std::shared_ptr<CCamera> pCamera);
	// �X�V�֐�.
	static void Update();
	// �r���[�s��A�v���W�F�N�V�����s��̏�����.
	static void InitViewProj();

	// �r���[�s��擾�֐�.
	static const D3DXMATRIX GetViewMatrix();
	// �v���W�F�N�V�����s��擾�֐�.
	static const D3DXMATRIX GetProjMatrix();
	// �J�������W�擾�֐�.
	static const D3DXVECTOR3 GetPosition();
	// �J�����������W�擾�֐�.
	static const D3DXVECTOR3 GetLookPosition();
	// �J�����ړ����x�擾�֐�.
	static const float GetMoveSpeed();
	// ������x�N�g���擾�֐�.
	static const D3DXVECTOR3 GetUpVector();
	// �Ώۂ̍��W���X�N���[�����W�ŕԂ��֐�.
	static const D3DXVECTOR2 GetWorldToScreenPos(const D3DXVECTOR3& Pos);
	// �J�����������Ă�������̌����x�N�g����Ԃ��֐�.
	static const D3DXVECTOR3 GetCameraDirVector();
	// �J�����������Ă�������ɏ����i�񂾍��W��Ԃ��֐�.
	static const D3DXVECTOR3 GetCameraFrontPosition();
	// �J�����ɂ����Ă��邩��Ԃ��֐�.
	static const bool IsOutCamera(const D3DXVECTOR3& vTargetPos, const float& fOutRad = 140.0f);
private:
	std::shared_ptr<CCamera> m_pCamera;	// �J�����N���X.
	std::shared_ptr<CCamera> m_pOldCamera;	// �J�����N���X.

private:
	// �C���X�^���X�̎擾.
	static CCameraManager* GetInstance();
};



#endif // #ifndef CAMERA_MANAGER_H.