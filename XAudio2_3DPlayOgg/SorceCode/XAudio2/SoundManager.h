#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <map>
#include <iostream>
#include <thread>
#include "XAudio2Play\XAudio2PlayBGM.h"
#include "XAudio2Play\XAudio2PlaySE.h"
#include "X3DAudio/X3DAudioPlay/X3DAudioPlayBGM.h"
#include "X3DAudio/X3DAudioPlay/X3DAudioPlaySE.h"
#include "OggLoad/OggLoad.h"

/**
 * @file SoundManager.h
 * @brief XAudio2のサウンド再生機能をまとめたシングルトンクラス.
 * @author OTakotiri.
 */

class CSoundManager
{
public:
//=============================
// 構造体宣言.
//=============================
	struct SoundVolume
	{
		float MasterVolume = 1.0f;
		float BGMVolume = 1.0f;
		float SEVolume = 1.0f;
	};
public:
	CSoundManager();
	~CSoundManager();
	// サウンドデータ作成.
	static void CreateSoundData();
	// インスタンスの取得.
	static CSoundManager* GetInstance();
	// BGMクラスの取得.
	static std::shared_ptr<CXAudio2PlayBGM>		GetBGM(const std::string& filename);
	// SEクラスの取得.
	static std::shared_ptr<CXAudio2PlaySE>		GetSE(const std::string& filename);
	// 3DBGMクラスの取得.
	static std::shared_ptr<CX3DAudioPlayBGM>	Get3DBGM(const std::string& filename);
	// 3DSEクラスの取得.
	static std::shared_ptr<CX3DAudioPlaySE>		Get3DSE(const std::string& filename);
	// ゲーム全体のBGMの最大音量を取得.
	inline static const float GetGameBGMVolume()		{ return GetInstance()->m_stSound.BGMVolume * GetInstance()->m_stSound.MasterVolume; }
	// 最大マスター音量取得
	inline static const float GetMasterVolume()			{ return GetInstance()->m_stSound.MasterVolume; }
	// 最大BGM音量取得
	inline static const float GetBGMVolume()			{ return GetInstance()->m_stSound.BGMVolume; }
	// ゲーム全体のSEの最大音量を取得.
	inline static const float GetGameSEVolume()			{ return GetInstance()->m_stSound.SEVolume * GetInstance()->m_stSound.MasterVolume; }
	// 解放処理関数.
	static void Release();
	//========================================================================================
	// オプション画面用全ソースのサウンド調整.
	// マスター音量セット.
	static void SetMasterVolume(float& MasterVolume);
	// BGM,SEの音量を変更するためのスレッドを立ち上げる.
	static void CreateChangeSoundVolumeThread();
	// BGM,SEの音量を変更するためのスレッドを解放する.
	static bool ReleaseChangeSoundVolumeThread();
	//===========================================================
	// オプション用関数.
	// 音量を変更できるスレッドのステート変更.
	static void StateChangeVolumeThread(const bool& bFlag);
	// オプション用スレッド動作済みかどうかのフラグ取得関数.
	inline static const bool GetCreateOptionThread()	{ return GetInstance()->m_isCreateThread; }
	// アプリ終了フラグ取得.
	inline const bool GetEndAppFlag()					{ return m_IsEndApp; }

public:
	SoundVolume m_stSound;
private:
	// 音量を変更するスレッドを立てる.
	static void SetChangeVolumeThread();
	// 音量初期設定関数.
	static void VolumeInit();
private:

	std::unordered_map <std::string, std::shared_ptr<CX3DAudioPlayBGM>> m_p3DBGMSource;
	std::unordered_map <std::string, std::shared_ptr<CX3DAudioPlaySE>>	m_p3DSESource;
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlayBGM>>	m_pBGMSource;
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlaySE>>	m_pSESource;

	// 終了用フラグ.
	bool	m_IsEndApp;
	// BGM,SEの作成が終わった後かどうか.
	bool	m_bEndCreate;
//=================================================
// 以下オプション画面用.
	// 以下二つはスレッドで回さないとストリーミングの入れ込みを待ってからの変更になるため
	// スレッドに回す.
	std::thread m_SoundSourceVolume;	// オプション画面で、音量を変更するためのスレッド.
	bool	m_bMoveSoundVolumeThread;	// オプション画面で、BGMを設定と同時に変更するためのスレッド用変数.
	bool	m_isCreateThread;	// 音量変更のスレッドが動いているかどうか.
//------------------------------------------
// スレッドロック用.
	bool	m_bResumeSoundVolumeThread;	// 条件変数.
	std::mutex m_SoundVolumemtx;
	std::condition_variable m_SoundVolumeCv;
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSoundManager(const CSoundManager &) = delete;
	CSoundManager& operator = (const CSoundManager &) = delete;
	CSoundManager(CSoundManager &&) = delete;
	CSoundManager& operator = (CSoundManager &&) = delete;
};

#endif // #ifndef SOUNDMANAGER_H.
