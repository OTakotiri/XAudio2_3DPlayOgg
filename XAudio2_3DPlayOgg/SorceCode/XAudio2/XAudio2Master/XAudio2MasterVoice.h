#ifndef XAUDIO2_MASTERVOICE_H
#define XAUDIO2_MASTERVOICE_H

#include "XAudio2.h"
#include <x3daudio.h>
#define COM_SAFE_RELEASE( p ) { if(p) { (p)->Release(); (p) = nullptr; } }

/**
 * @file XAudio2MasterVoice.h
 * @brief XAduio2の再生に必要なマスターボイスクラス.
 * @author OTakotiri.
 */

class CXAudio2MasterVoice
{
public:
	CXAudio2MasterVoice();
	~CXAudio2MasterVoice();

	// インスタンスの取得.
	static CXAudio2MasterVoice* GetInstance();

	IXAudio2* GetInterface()const;
	// マスターボイス取得.
	IXAudio2MasteringVoice* GetMasteringVoice()const;
	static CXAudio2MasterVoice& GetGlobalSystem();
	// 解放関数.
	static void Release();
public:
	UINT					m_SampleRate;
	UINT					m_InputChannels;
	// 3Dサウンドハンドル.
	X3DAUDIO_HANDLE         m_x3DHandle;

protected:
	// 初期化関数.
	bool Initialize();


	IXAudio2*				m_pXAudioInterface;                //XAudio2インターフェース
	IXAudio2MasteringVoice* m_pMasterVoice;                    //マスターボイス.
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CXAudio2MasterVoice(const CXAudio2MasterVoice &) = delete;
	CXAudio2MasterVoice& operator = (const CXAudio2MasterVoice &) = delete;
	CXAudio2MasterVoice(CXAudio2MasterVoice &&) = delete;
	CXAudio2MasterVoice& operator = (CXAudio2MasterVoice &&) = delete;
};

#endif //#ifndef XAUDIO2_MASTERVOICE_H