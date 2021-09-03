#ifndef XAUDIO2_PLAYBGM_H
#define XAUDIO2_PLAYBGM_H


#include <vector>
#include <mutex>
#include "..\XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include "..\OggLoad\OggLoad.h"
#include "..\XAudio2CallBack\VoiceCallBack.h"

#include "..\X3DAudio\X3DAudio.h"
struct IXAudio2SourceVoice;

/**
 * @file XAudio2PlayBGM.h
 * @brief XAudio2でのBGMストリーミング再生を目的としたクラス.
 * @author OTakotiri.
 */

class CXAudio2PlayBGM
{
public: //---------------- 関数 -----------------
	CXAudio2PlayBGM();
	~CXAudio2PlayBGM();
	// 初期設定.
	HRESULT InitSourceVoice(const char* filename);
	// 再生してるかどうか取得関数.
	const bool IsPlaying();
	// スレッドを作ってBGMを再生する.
	void PlayBGM(const bool LoopFlag = true);
	// BGMスレッドを停止する.
	bool StopBGMThread();
	// BGMサウンドを完全停止.(位置を保持しない).
	bool Stop();
	// サウンド一時停止.
	bool Pause();
	// サウンド再生を再開.
	bool PlayStart();
	// ピッチ設定関数.
	bool SetPitch(float value);
	// ピッチを取得関数.
	const float GetPitch();
	// 最大ピッチを設定します.
	void SetMaxPitch(float value);
	// ピッチを設定.
	void SetPitchValue(const float value) { m_fPitch = value; }
	// パン値設定.
	void SetAudioPan(float value);
	// Oggからソースボイスを作成する.
	HRESULT CreateOggSound();
	// リバーブを設定.
	HRESULT SetReverb(const XAudio2Reverb::enReverbPreset Preset);
	//=======================================================================
	// 基本音量操作系.
	//====
	// BGMの音量を取得します.
	const float GetVolume();
	// BGMの音量を設定します.
	bool SetBGMVolume(float value);
	// 現在の音量から加算または減算.
	bool  AdjustVolume(float value);
	// SoundSourceを破壊する関数.
	void DestoroySource();
	// フェードアウトフラグのセッター.
	inline void SetFadeOutFlag(bool BGMFadeOutFlag)				{ m_bFadeOutStart = BGMFadeOutFlag; }
	// フェードアウトフラグのゲッター.
	inline const bool GetFadeOutFlag()							{ return m_bFadeOutStart; }
	// フェードインフラグのセッター.
	inline void SetFadeInFlag(bool BGMFadeInFlag)				{ m_bFadeInStart = BGMFadeInFlag; }
	// フェードインフラグのゲッター.
	inline const bool GetFadeInFlag()							{ return m_bFadeInStart; }
	// 解放処理.
	void Release();
protected:
	// BGMをフェードアウトする関数.
	bool FadeOutBGM(float value, const bool& isGameEnd);
	// BGMをフェードインする関数.
	bool FadeInBGM(float value, const bool& isGameEnd);
	// 音源データをストリームに流し込む.
	HRESULT OggSubmit();
private:
	// サウンド再生、前に一時停止されていた場合は記憶位置から再開.
	bool Play(bool& isEnd, const bool IsLoop = true);
	// フェードアウト用のスレッドを起動する.
	void CallFadeOutBGM(bool& isEnd);
	// フェードイン用のスレッドを起動する.
	void CallFadeInBGM(bool& isEnd);
protected: //--------------- 変数 ---------------.
		// Oggデータクラスの箱
	std::shared_ptr<COggLoad>	m_pOggWavData;
	// BGMストリーミング再生用スレッド.
	std::thread					m_pBGMThread;
	// このクラスで再生するファイル名.
	std::string					m_SourceFileName;
	// 再生終了(スレッド終了)フラグ.
	bool						m_IsEndPlay;


	std::mutex			m_mtx;
	VoiceCallback		m_Callback;		// コールバック.
	XAUDIO2_BUFFER		m_Buffer;		// オーディオバッファー
	int					m_Buff_len;		// バッファレンジ.
	BYTE**				m_Buff;			// バッファサイズ.
	int					m_Len;			// レンジ.
	int					m_Buff_cnt;		// バッファカウント.
	int					m_Size;

	IXAudio2SourceVoice*	m_pSourceVoice; //  サウンドを再生するストリーム.
	IXAudio2SubmixVoice*	m_pSubmixVoice;	// サブミックスボイス.

	std::thread	m_FadeInThread;			// ボリュームフェードイン用スレッド.
	std::thread	m_FadeOutThread;		// ボリュームフェードアウト用スレッド.
	bool	m_isMoveFadeInThread;		// フェードインスレッドが動いているか.
	bool	m_isMoveFadeOutThread;		// フェードアウトスレッドが動いているか.
	float	m_fMaxPitch;				// 最大ピッチ.
	float	m_fPitch;					// ピッチ.
	bool	m_bFadeOutStart;			// フェードアウト開始フラグ.
	bool	m_bFadeInStart;				// フェードイン開始フラグ.
	float	m_fFadeInValue;				// フェードインの値.
	float	m_fFadeOutValue;			// フェードアウトの値.
	bool	m_bFirstPlay;				// 初回再生かどうか.
};

#endif // !C_XAUDIO2_PLAYSOUND_H