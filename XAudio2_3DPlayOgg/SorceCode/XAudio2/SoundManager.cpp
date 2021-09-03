#include "SoundManager.h"
#include <crtdbg.h>
#include <filesystem>	// C++17 必須.
#include "..\Common\FileManager\FileManager.h"

namespace fs = std::filesystem;
//=============================
// 定数宣言.
//=============================
namespace {
	constexpr char FILE_PATH[128] = "Data\\Sound";					// Soundデータが入っているディレクトリパス.
	constexpr char BINARY_FILE_PATH[128] = "Data\\Sound\\Data.bin";	// バイナリデータが入ってるパス.
	constexpr CSoundManager::SoundVolume INIT_VOLUME = { 0.5f,0.5f,0.5f };	// バイナリデータが存在しない際に設定する初期音量.
}

CSoundManager::CSoundManager()
	: m_IsEndApp					( false )
	, m_bEndCreate					( false )
	, m_bMoveSoundVolumeThread		( true )
	, m_isCreateThread				( false )
	, m_bResumeSoundVolumeThread	( true )
{
}

CSoundManager::~CSoundManager()
{
}
//=============================
// インスタンスの作成.
//=============================
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance =
		std::make_unique<CSoundManager>();	// インスタンスの作成.
	return pInstance.get();
}
//=============================
// BGMクラスの取得.
//=============================
std::shared_ptr<CXAudio2PlayBGM> CSoundManager::GetBGM(const std::string& filename)
{
	if (GetInstance()->m_pBGMSource.count(filename) == 0) return nullptr;
	return GetInstance()->m_pBGMSource[filename];
}
//=============================
// SEクラスの取得.
//=============================
std::shared_ptr<CXAudio2PlaySE> CSoundManager::GetSE(const std::string& filename)
{
	if (GetInstance()->m_pSESource.count(filename) == 0) return nullptr;
	return GetInstance()->m_pSESource[filename];
}
//=============================
// 3DBGMクラスの取得.
//=============================
std::shared_ptr<CX3DAudioPlayBGM> CSoundManager::Get3DBGM(const std::string& filename)
{
	if (GetInstance()->m_p3DBGMSource.count(filename) == 0) return nullptr;
	return GetInstance()->m_p3DBGMSource[filename];
}
//=============================
// 3DSEクラスの取得.
//=============================
std::shared_ptr<CX3DAudioPlaySE> CSoundManager::Get3DSE(const std::string& filename)
{
	if (GetInstance()->m_p3DSESource.count(filename) == 0) return nullptr;
	return GetInstance()->m_p3DSESource[filename];
}

//=============================
// サウンドデータ作成.
//=============================
void CSoundManager::CreateSoundData()
{
	if (GetInstance()->m_bEndCreate == true) return;
	auto eachLoad = [&](const fs::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// 拡張子.
		const std::string filePath = entry.path().string();					// ファイルパス.
		const std::string fileName = entry.path().stem().string();			// ファイル名.
		// 拡張子がOggでなければ終了.
		if (extension != ".ogg" && extension != ".OGG") return;

		//ファイルパス内にBGMがなければSEの作成をする.
		if (filePath.find("BGM") == std::string::npos) {
			if (filePath.find("3D") == std::string::npos) {
				// 2D再生のSEクラス作成.
				GetInstance()->m_pSESource[fileName] = std::make_shared<CXAudio2PlaySE>();
				if (GetInstance()->m_pSESource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create SESource");
			}
			else {
				// 3D再生のSEクラス作成.
				GetInstance()->m_p3DSESource[fileName] = std::make_shared<CX3DAudioPlaySE>();
				if (GetInstance()->m_p3DSESource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create 3DSESource");
			}
		}
		else {
			if (filePath.find("3D") == std::string::npos) {
				// 2D再生のBGMクラス作成.
				GetInstance()->m_pBGMSource[fileName] = std::make_shared<CXAudio2PlayBGM>();
				if (GetInstance()->m_pBGMSource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create BGMSource");
			}
			else {
				// 3D再生のBGMクラス作成.
				GetInstance()->m_p3DBGMSource[fileName] = std::make_shared<CX3DAudioPlayBGM>();
				if( GetInstance()->m_p3DBGMSource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false,L"Failed Create 3DBGMSource");
			}
		}
	};

	try {
		//指定パス(FILE_PATH)ディレクトリ内を再帰的に走査
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		// eachLoad関数を行う.
		std::for_each(dir_itr, end_itr, eachLoad);
	}
	// エラーキャッチ.
	catch (const fs::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"サウンドデータ作成失敗");
		MessageBox(nullptr, errorMessage, "サウンドデータ作成失敗", MB_OK);
	}
	// 音量設定.
	VolumeInit();
	// サウンド作成終了.
	GetInstance()->m_bEndCreate = true;
}
//=============================
// 音量を変更できるスレッドのステート変更.
//=============================
void CSoundManager::StateChangeVolumeThread(const bool& bFlag)
{
	GetInstance()->m_bResumeSoundVolumeThread = bFlag;
	// 入れたフラグがtrueの時、元がfalseの場合、スレッドがサスペンドしているため.
	// 音量を変更するスレッドを起床させる.
	if (bFlag) GetInstance()->m_SoundVolumeCv.notify_one();
}
//=============================
// 解放関数.
//=============================
void CSoundManager::Release()
{
	// 終了フラグを立てる.
	GetInstance()->m_IsEndApp = true;
	// 音量を変更するスレッドの解放待ち.
	while (ReleaseChangeSoundVolumeThread() == false);
	// 音量をバイナリデータにセーブ.
	CFileManager::CreateBinary(BINARY_FILE_PATH, GetInstance()->m_stSound);
	for (auto& Data : GetInstance()->m_pBGMSource) {
		Data.second->Release();
	}
	for (auto& Data : GetInstance()->m_p3DBGMSource) {
		Data.second->Release();
	}
	for (auto& Data : GetInstance()->m_pSESource) {
		Data.second->Release();
	}
	for (auto& Data : GetInstance()->m_p3DSESource) {
		Data.second->Release();
	}
	GetInstance()->m_pBGMSource.clear();
	GetInstance()->m_p3DBGMSource.clear();
	GetInstance()->m_pSESource.clear();
	GetInstance()->m_p3DSESource.clear();


}
//=============================
// マスター音量セット.
//=============================
void CSoundManager::SetMasterVolume(float& MasterVolume)
{
	if (MasterVolume >= 1.0f) MasterVolume = 1.0f;
	if (MasterVolume <= 0.0f) MasterVolume = 0.0f;
	GetInstance()->m_stSound.MasterVolume = MasterVolume;
}
//=============================
// BGM,SEの音量を変更するためのスレッドを立ち上げる.
//=============================
void CSoundManager::CreateChangeSoundVolumeThread()
{
	// スレッド作成のフラグが立っていればリターン.
	if (GetInstance()->m_isCreateThread == true) return;
	SetChangeVolumeThread();	// スレッド作成.
	GetInstance()->m_isCreateThread = true;
}
//=============================
// 音量を変更するスレッドを立てる.
//=============================
void CSoundManager::SetChangeVolumeThread()
{
	DWORD ThreadExitCode = -1;
	// スレッド状態を取得.
	GetExitCodeThread(GetInstance()->m_SoundSourceVolume.native_handle(), &ThreadExitCode);
	// スレッドが動いていればリターン.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveSoundVolumeThread = true;
	auto SetSoundVolume = [&]()
	{
		while (GetInstance()->m_bMoveSoundVolumeThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_SoundVolumemtx); // mutex.
			// スレッドをm_bMoveSoundVolumeThreadがfalseの間ここでサスペンド(一切動かさない)、trueで再開.
			GetInstance()->m_SoundVolumeCv.wait(lk, [&] { return GetInstance()->m_bResumeSoundVolumeThread; });


			for (auto& Data : GetInstance()->m_pBGMSource) {
				Data.second->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
			}
			for (auto& Data : GetInstance()->m_p3DBGMSource) {
				Data.second->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
			}
			for (auto& Data : GetInstance()->m_pSESource) {
				Data.second->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);
			}
			for (auto& Data : GetInstance()->m_p3DSESource) {
				Data.second->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);
			}
		}
	};
	GetInstance()->m_SoundSourceVolume = std::thread(SetSoundVolume);
}
//=============================
// 音量初期設定関数.
//=============================
void CSoundManager::VolumeInit()
{
	HRESULT hr;
	// バイナリデータから音量を読み込む.
	hr = XAudio2File::LoadBinary(BINARY_FILE_PATH, GetInstance()->m_stSound);
	// バイナリデータが存在しない場合.
	if (hr == E_FAIL) {
		// バイナリデータ新規作成.
		XAudio2File::CreateBinary(BINARY_FILE_PATH, INIT_VOLUME);
		GetInstance()->m_stSound = INIT_VOLUME;
	}
	//---------------------
	// 音量を適応する.
	CSoundManager::GetInstance()->m_stSound = GetInstance()->m_stSound;

	for (auto& Data : GetInstance()->m_pBGMSource) {
		Data.second->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
	}
	for (auto& Data : GetInstance()->m_p3DBGMSource) {
		Data.second->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
	}
	for (auto& Data : GetInstance()->m_pSESource) {
		Data.second->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);
	}
	for (auto& Data : GetInstance()->m_p3DSESource) {
		Data.second->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);
	}

	SetMasterVolume(GetInstance()->m_stSound.MasterVolume);

}

//=============================
// BGM,SEの音量を変更するためのスレッドを解放する.
//=============================
bool CSoundManager::ReleaseChangeSoundVolumeThread()
{
	GetInstance()->m_bResumeSoundVolumeThread = true;
	// スレッドを起床させる.
	GetInstance()->m_SoundVolumeCv.notify_one();
	// スレッドが解放できる状態(スレッドが動いている). 
	if (GetInstance()->m_SoundSourceVolume.joinable() == true) {
		GetInstance()->m_bMoveSoundVolumeThread = false;
		GetInstance()->m_SoundSourceVolume.join();
		while (1)
		{
			if (GetInstance()->m_SoundSourceVolume.joinable() != true) {
				break;
			}
		}
	}
	GetInstance()->m_isCreateThread = false;
	return true;
}