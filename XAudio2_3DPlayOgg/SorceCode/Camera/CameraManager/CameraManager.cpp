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
// カメラクラスの設定.
//=====================================
void CCameraManager::SetCamera(std::shared_ptr<CCamera> pCamera)
{
	if (GetInstance()->m_pOldCamera == pCamera) return;
	GetInstance()->m_pCamera = pCamera;
	GetInstance()->m_pOldCamera = pCamera;

}
//=====================================
// 指定したカメラクラスポインタに今セットされているカメラの座標情報を設定する.
//=====================================
void CCameraManager::GetNowCamera(std::shared_ptr<CCamera> pCamera)
{
	GetInstance()->m_pCamera->GetCameraValue(pCamera);
}
//=====================================
// 更新関数.
//=====================================
void CCameraManager::Update()
{
	if (GetInstance()->m_pCamera == nullptr) return;
	GetInstance()->m_pCamera->Update();
}
//=====================================
// ビュー行列、プロジェクション行列の初期化.
//=====================================
void CCameraManager::InitViewProj()
{
	if (GetInstance()->m_pCamera == nullptr) return;
	GetInstance()->m_pCamera->InitViewProj();
}
//=====================================
// ビュー行列取得関数.
//=====================================
const D3DXMATRIX CCameraManager::GetViewMatrix()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetViewMatrix();
}
//=====================================
// プロジェクション行列取得関数.
//=====================================
const D3DXMATRIX CCameraManager::GetProjMatrix()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetProjMatrix();
}
//=====================================
// カメラ座標取得関数.
//=====================================
const D3DXVECTOR3 CCameraManager::GetPosition()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetPosition();
}
//=====================================
// カメラ注視座標取得関数.
//=====================================
const D3DXVECTOR3 CCameraManager::GetLookPosition()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetLookPosition();
}
//=====================================
// カメラの移動速度取得関数.
//=====================================
const float CCameraManager::GetMoveSpeed()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetMoveSpeed();
}
//=====================================
// 上方向ベクトル取得関数.
//=====================================
const D3DXVECTOR3 CCameraManager::GetUpVector()
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->GetUpVector();
}
//=====================================
// 対象の座標をスクリーン座標で返す関数.
//=====================================
const D3DXVECTOR2 CCameraManager::GetWorldToScreenPos(const D3DXVECTOR3 & Pos)
{
	if (GetInstance()->m_pCamera == nullptr) return {};
	return GetInstance()->m_pCamera->WorldToScreenPos(Pos);
}
//=====================================
// カメラが向いている方向の向きベクトルを返す関数.
//=====================================
const D3DXVECTOR3 CCameraManager::GetCameraDirVector()
{
	D3DXMATRIX Tes = GetViewMatrix();
	D3DXVECTOR3 Vec;
	D3DXMatrixTranspose(&Tes, &Tes);//行列を転置する.
	Vec.x = Tes._31;
	Vec.y = Tes._32;
	Vec.z = Tes._33;
	return Vec;
}
//=====================================
// カメラが向いている方向に少し進んだ座標を返す関数.
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

	// カメラの前ベクトルを用意.
	D3DXVECTOR3 myVector = GetCameraDirVector();

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(MoveVector.x*MoveVector.x + MoveVector.z*MoveVector.z);

	// カメラから対象へのベクトルと、カメラの前ベクトルの外積を求める.
	float cross = myVector.x * MoveVector.z - myVector.z * MoveVector.x;
	float dot = myVector.x * MoveVector.x + myVector.z * MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// 内積がしきい値を超えたら対象が画面外に出た.
	bool IsOffScreen = dot < static_cast<float>(D3DXToRadian(fOutRad)) ? true : false;

	// trueで画面外.
	return IsOffScreen;
}
//=====================================
// インスタンスの取得.
//=====================================
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance =
		std::make_unique<CCameraManager>();

	return pInstance.get();
}