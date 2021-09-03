#include "SoundManager.h"
#include <crtdbg.h>
#include <filesystem>	// C++17 �K�{.
#include "..\Common\FileManager\FileManager.h"

namespace fs = std::filesystem;
//=============================
// �萔�錾.
//=============================
namespace {
	constexpr char FILE_PATH[128] = "Data\\Sound";					// Sound�f�[�^�������Ă���f�B���N�g���p�X.
	constexpr char BINARY_FILE_PATH[128] = "Data\\Sound\\Data.bin";	// �o�C�i���f�[�^�������Ă�p�X.
	constexpr CSoundManager::SoundVolume INIT_VOLUME = { 0.5f,0.5f,0.5f };	// �o�C�i���f�[�^�����݂��Ȃ��ۂɐݒ肷�鏉������.
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
// �C���X�^���X�̍쐬.
//=============================
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance =
		std::make_unique<CSoundManager>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}
//=============================
// BGM�N���X�̎擾.
//=============================
std::shared_ptr<CXAudio2PlayBGM> CSoundManager::GetBGM(const std::string& filename)
{
	if (GetInstance()->m_pBGMSource.count(filename) == 0) return nullptr;
	return GetInstance()->m_pBGMSource[filename];
}
//=============================
// SE�N���X�̎擾.
//=============================
std::shared_ptr<CXAudio2PlaySE> CSoundManager::GetSE(const std::string& filename)
{
	if (GetInstance()->m_pSESource.count(filename) == 0) return nullptr;
	return GetInstance()->m_pSESource[filename];
}
//=============================
// 3DBGM�N���X�̎擾.
//=============================
std::shared_ptr<CX3DAudioPlayBGM> CSoundManager::Get3DBGM(const std::string& filename)
{
	if (GetInstance()->m_p3DBGMSource.count(filename) == 0) return nullptr;
	return GetInstance()->m_p3DBGMSource[filename];
}
//=============================
// 3DSE�N���X�̎擾.
//=============================
std::shared_ptr<CX3DAudioPlaySE> CSoundManager::Get3DSE(const std::string& filename)
{
	if (GetInstance()->m_p3DSESource.count(filename) == 0) return nullptr;
	return GetInstance()->m_p3DSESource[filename];
}

//=============================
// �T�E���h�f�[�^�쐬.
//=============================
void CSoundManager::CreateSoundData()
{
	if (GetInstance()->m_bEndCreate == true) return;
	auto eachLoad = [&](const fs::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// �g���q.
		const std::string filePath = entry.path().string();					// �t�@�C���p�X.
		const std::string fileName = entry.path().stem().string();			// �t�@�C����.
		// �g���q��Ogg�łȂ���ΏI��.
		if (extension != ".ogg" && extension != ".OGG") return;

		//�t�@�C���p�X����BGM���Ȃ����SE�̍쐬������.
		if (filePath.find("BGM") == std::string::npos) {
			if (filePath.find("3D") == std::string::npos) {
				// 2D�Đ���SE�N���X�쐬.
				GetInstance()->m_pSESource[fileName] = std::make_shared<CXAudio2PlaySE>();
				if (GetInstance()->m_pSESource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create SESource");
			}
			else {
				// 3D�Đ���SE�N���X�쐬.
				GetInstance()->m_p3DSESource[fileName] = std::make_shared<CX3DAudioPlaySE>();
				if (GetInstance()->m_p3DSESource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create 3DSESource");
			}
		}
		else {
			if (filePath.find("3D") == std::string::npos) {
				// 2D�Đ���BGM�N���X�쐬.
				GetInstance()->m_pBGMSource[fileName] = std::make_shared<CXAudio2PlayBGM>();
				if (GetInstance()->m_pBGMSource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false, L"Failed Create BGMSource");
			}
			else {
				// 3D�Đ���BGM�N���X�쐬.
				GetInstance()->m_p3DBGMSource[fileName] = std::make_shared<CX3DAudioPlayBGM>();
				if( GetInstance()->m_p3DBGMSource[fileName]->InitSourceVoice(fileName.c_str()) == E_FAIL) _ASSERT_EXPR(false,L"Failed Create 3DBGMSource");
			}
		}
	};

	try {
		//�w��p�X(FILE_PATH)�f�B���N�g�������ċA�I�ɑ���
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		// eachLoad�֐����s��.
		std::for_each(dir_itr, end_itr, eachLoad);
	}
	// �G���[�L���b�`.
	catch (const fs::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"�T�E���h�f�[�^�쐬���s");
		MessageBox(nullptr, errorMessage, "�T�E���h�f�[�^�쐬���s", MB_OK);
	}
	// ���ʐݒ�.
	VolumeInit();
	// �T�E���h�쐬�I��.
	GetInstance()->m_bEndCreate = true;
}
//=============================
// ���ʂ�ύX�ł���X���b�h�̃X�e�[�g�ύX.
//=============================
void CSoundManager::StateChangeVolumeThread(const bool& bFlag)
{
	GetInstance()->m_bResumeSoundVolumeThread = bFlag;
	// ���ꂽ�t���O��true�̎��A����false�̏ꍇ�A�X���b�h���T�X�y���h���Ă��邽��.
	// ���ʂ�ύX����X���b�h���N��������.
	if (bFlag) GetInstance()->m_SoundVolumeCv.notify_one();
}
//=============================
// ����֐�.
//=============================
void CSoundManager::Release()
{
	// �I���t���O�𗧂Ă�.
	GetInstance()->m_IsEndApp = true;
	// ���ʂ�ύX����X���b�h�̉���҂�.
	while (ReleaseChangeSoundVolumeThread() == false);
	// ���ʂ��o�C�i���f�[�^�ɃZ�[�u.
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
// �}�X�^�[���ʃZ�b�g.
//=============================
void CSoundManager::SetMasterVolume(float& MasterVolume)
{
	if (MasterVolume >= 1.0f) MasterVolume = 1.0f;
	if (MasterVolume <= 0.0f) MasterVolume = 0.0f;
	GetInstance()->m_stSound.MasterVolume = MasterVolume;
}
//=============================
// BGM,SE�̉��ʂ�ύX���邽�߂̃X���b�h�𗧂��グ��.
//=============================
void CSoundManager::CreateChangeSoundVolumeThread()
{
	// �X���b�h�쐬�̃t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_isCreateThread == true) return;
	SetChangeVolumeThread();	// �X���b�h�쐬.
	GetInstance()->m_isCreateThread = true;
}
//=============================
// ���ʂ�ύX����X���b�h�𗧂Ă�.
//=============================
void CSoundManager::SetChangeVolumeThread()
{
	DWORD ThreadExitCode = -1;
	// �X���b�h��Ԃ��擾.
	GetExitCodeThread(GetInstance()->m_SoundSourceVolume.native_handle(), &ThreadExitCode);
	// �X���b�h�������Ă���΃��^�[��.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveSoundVolumeThread = true;
	auto SetSoundVolume = [&]()
	{
		while (GetInstance()->m_bMoveSoundVolumeThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_SoundVolumemtx); // mutex.
			// �X���b�h��m_bMoveSoundVolumeThread��false�̊Ԃ����ŃT�X�y���h(��ؓ������Ȃ�)�Atrue�ōĊJ.
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
// ���ʏ����ݒ�֐�.
//=============================
void CSoundManager::VolumeInit()
{
	HRESULT hr;
	// �o�C�i���f�[�^���特�ʂ�ǂݍ���.
	hr = XAudio2File::LoadBinary(BINARY_FILE_PATH, GetInstance()->m_stSound);
	// �o�C�i���f�[�^�����݂��Ȃ��ꍇ.
	if (hr == E_FAIL) {
		// �o�C�i���f�[�^�V�K�쐬.
		XAudio2File::CreateBinary(BINARY_FILE_PATH, INIT_VOLUME);
		GetInstance()->m_stSound = INIT_VOLUME;
	}
	//---------------------
	// ���ʂ�K������.
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
// BGM,SE�̉��ʂ�ύX���邽�߂̃X���b�h���������.
//=============================
bool CSoundManager::ReleaseChangeSoundVolumeThread()
{
	GetInstance()->m_bResumeSoundVolumeThread = true;
	// �X���b�h���N��������.
	GetInstance()->m_SoundVolumeCv.notify_one();
	// �X���b�h������ł�����(�X���b�h�������Ă���). 
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