#include "..\SceneList.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\..\Common\D3DX\D3DX.h"
#include "..\..\Camera\SimpleCamera\SimpleCamera.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\Common\FPSCounter\FPSCounter.h"
//=====================================
// 定数宣言.
//=====================================
namespace {
	constexpr float CAMERA_POS[3] = { 0.0f,5.0f,-20.0f };
	constexpr float CAMERA_LOOKPOS[3] = { 0.0f,0.0f,0.0f };
	constexpr char	BGM_NAME[] = "pien";
	constexpr char	SE_NAME[] = "TestSE";
}
static D3DXVECTOR3 vPos = { 0.0f,0.0f,-10.0f };
static D3DXVECTOR3 vRot = { 0.0f,90.0f,0.0f };
static CX3DAudio::AUDIO_LISTENER stLis;
static CX3DAudio::AUDIO_EMITTER stEmi;

CTitleScene::CTitleScene(CSceneManager* pSceneManager)
	: CSceneBase			( pSceneManager )
	, m_pStatic				( nullptr )
	, m_pCamera				( std::make_shared<CSimpleCamera>() )
{
	CSoundManager::CreateSoundData();

	m_pCamera->SetPosition(CAMERA_POS);	// カメラ座標設定.
	m_pCamera->SetLookPosition(CAMERA_LOOKPOS);// カメラ視点座標設定.
	CCameraManager::SetCamera(m_pCamera);

	stLis.Pos = { vPos.x, vPos.y, vPos.z };
	stLis.FrontVec = { 0.0f,0.0f,-1.0f };
	stLis.Up = { 0.0f,1.0f, 0.0f };
	stLis.Velocity = { 0.0f,0.0f, 0.0f };

	CSoundManager::Get3DSE("TestSEa")->SetListenerInfo(stLis);

	CSoundManager::Get3DSE("TestSEa")->SetEmitterInfo(stEmi);

	CSoundManager::Get3DBGM("Test3D")->SetListenerInfo(stLis);

	CSoundManager::Get3DBGM("Test3D")->PlayBGM();
}

CTitleScene::~CTitleScene()
{
}
bool CTitleScene::Load()
{
	if ((m_pStatic = CMeshResource::GetStatic("asa"))			== nullptr) return false;	// スキンメッシュの読み込み.
	if ((m_pStatic2 = CMeshResource::GetStatic("asa")) == nullptr) return false;	// スタティックメッシュの読み込み.
	
	return true;
}
void CTitleScene::Update()
{

	CSoundManager::Get3DBGM("Test3D")->SetEmitterPosition({ CCameraManager::GetPosition().x, CCameraManager::GetPosition().y, CCameraManager::GetPosition().z });
	CSoundManager::Get3DBGM("Test3D")->SetEmitterFront({ 0.0f,0.0f,1.0f });


	stLis.Pos = { vPos.x, vPos.y, vPos.z };
	stEmi.Pos = { CCameraManager::GetPosition().x, CCameraManager::GetPosition().y, CCameraManager::GetPosition().z };
	stEmi.FrontVec = { 0.0f,0.0f,1.0f };




	CSoundManager::Get3DSE("TestSEa")->Update3DSound();
	if (GetAsyncKeyState('B') & 0x0001) {
		CSoundManager::Get3DSE("TestSEa")->Play();
	}

	if (GetAsyncKeyState('E') & 0x8000){
		if (GetAsyncKeyState('1') & 0x0001)
			CSoundManager::Get3DBGM("Test3D")->SetReverb(XAudio2Reverb::enReverbPreset::en_PRESET_SEWERPIPE);
		if (GetAsyncKeyState('2') & 0x0001)
			CSoundManager::Get3DBGM("Test3D")->SetReverb(XAudio2Reverb::enReverbPreset::en_PRESET_FOREST);
		if (GetAsyncKeyState('3') & 0x0001)
			CSoundManager::Get3DBGM("Test3D")->SetReverb(XAudio2Reverb::enReverbPreset::en_PRESET_UNDERWATER);
	}
	if (GetAsyncKeyState('Y') & 0x8000) {
		CSoundManager::Get3DBGM("Test3D")->SetPitch(1.45f);
	}
	if (GetAsyncKeyState('P') & 0x0001) {
		vPos = CCameraManager::GetPosition();
	}
}
void CTitleScene::MeshRender()
{
	RenderAikoMesh();
}
void CTitleScene::SpriteRender()
{
}

void CTitleScene::RenderAikoMesh()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)		vPos.z += 0.1f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		vPos.z -= 0.1f;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		vPos.x -= 0.1f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	vPos.x += 0.1f;

	m_pStatic2->SetPosition(vPos);
	m_pStatic2->SetAlpha(1.0f);
	m_pStatic2->SetScale(1.0f);
	m_pStatic2->Render();
}
