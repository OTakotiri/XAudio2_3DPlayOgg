#include "XAudio2PlaySE.h"
#include "XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include <vector>
#include <process.h>
#include <thread>

#include "..\SoundManager.h"

CXAudio2PlaySE::CXAudio2PlaySE()
	: m_pOggWavData				( nullptr )
	, m_CheckNonePlaySeVoice	( 0 )
	, m_RepeatPlaySe			( 0 )
	, m_bisAnotherVolume		( false )
	, m_fSEVolume				( 1.0f )
	, m_fAnotherVolume			( 0.2f )
	, m_SourceFileName			( "" )
{
	m_pSourceVoice[0] = nullptr;
}

CXAudio2PlaySE::~CXAudio2PlaySE()
{
}
HRESULT CXAudio2PlaySE::InitSourceVoice(const char * filename)
{
	m_SourceFileName = filename;
	// WavLoadクラス作成.
	m_pOggWavData = std::make_shared<COggLoad>();
	// Wavフォーマット取得.
	if (FAILED(m_pOggWavData->CreateOggData(m_SourceFileName, true))) return E_FAIL;
	if (FAILED(CreateOggSound())) return E_FAIL;
	return S_OK;
}
//=============================
// 解放処理.
//=============================
void CXAudio2PlaySE::Release()
{
	m_pOggWavData->Close();
	DestoroySource();
}
//=============================
// 再生されていないソースボイスを探す.
//=============================
int CXAudio2PlaySE::CheckPlaySeBuffer()
{
	//XAUDIO2_VOICE_STATE state;
	// 再生していないSEVoiceを探す.
	for (int Array = 0; Array < SE_VOICE_SIZE; Array++) {
		//m_pSourceVoice[Array]->GetState(&state);
		if (!IsPlayingSE(Array)) return Array;	// 再生されていない配列の値を返す.
	}
	// すべてサウンド再生途中なので、リピート再生する.
	if (m_RepeatPlaySe < SE_VOICE_SIZE) m_RepeatPlaySe++;
	// 最大数と同じになったら0にする.
	if (m_RepeatPlaySe == SE_VOICE_SIZE) m_RepeatPlaySe = 0;


	return m_RepeatPlaySe;
}
//=============================
// 指定された配列番号のSEが再生されているか.
//=============================
const bool CXAudio2PlaySE::IsPlayingSE(size_t ArrayNum)
{
	// SoundSourceがない場合リターン 0(再生していないときと同じ).
	if (m_pSourceVoice[ArrayNum] == nullptr) return false;
	XAUDIO2_VOICE_STATE xState;
	// SoundSourceのステート取得.
	m_pSourceVoice[ArrayNum]->GetState(&xState);

	return xState.BuffersQueued != 0;
}
//=============================
// サウンド再生.
//=============================
bool CXAudio2PlaySE::Play()
{
	HRESULT hr;
	int Array = 0;
	if (m_pSourceVoice[0] == nullptr) return false;
	if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;

	Array = CheckPlaySeBuffer(); // 再生されていないソースボイスの配列番号を取得.
	// 個別にSEの音量をセットするフラグが立っている時.
	if (m_bisAnotherVolume == true) SetSEVolume(m_fAnotherVolume, Array);	// SEに全体とは別の音量セット.
	else SetSEVolume(m_fSEVolume, Array);	// SEの音量セット.

	m_pSourceVoice[Array]->Stop();			// ソースボイス停止.
	m_pSourceVoice[Array]->FlushSourceBuffers();// ソースバッファをクリア.
	m_pSourceVoice[Array]->Start();			// ソースボイス再生.

	// バッファーを生成する
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = m_pOggWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = m_pOggWavData->GetSEFileSize();

	// キューにバッファーを投入する
	if (FAILED(hr = m_pSourceVoice[Array]->SubmitSourceBuffer(&seBuffer)))
	{
		return false;
	}

	return true;
}
//=============================
// SEを多重再生しないで再生.
// #もとからなっているSEを止めて一つにするものではない.
//=============================
bool CXAudio2PlaySE::NoMultiplePlay()
{
	HRESULT hr;

	if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;

	// このSEが再生されていなければ中に入り再生する.
	if (IsPlayingSE(0) == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_pSourceVoice[0]->Start();	// ソースボイス再生.
	}
	if (m_bisAnotherVolume == true) SetSEVolume(m_fAnotherVolume, 0);	// SEに全体とは別の音量セット.
	else SetSEVolume(m_fSEVolume, 0);		// SEの音量セット.

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// バッファが0より大きい.
	{
		m_pSourceVoice[0]->Stop();		// ソースボイス停止.
		m_pSourceVoice[0]->FlushSourceBuffers();// ソースバッファをクリア.
		m_pSourceVoice[0]->Start();		// ソースボイス再生.
	}

	// バッファーを生成する
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = m_pOggWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = m_pOggWavData->GetSEFileSize();

	// キューにバッファーを投入する
	if (FAILED(hr = m_pSourceVoice[0]->SubmitSourceBuffer(&seBuffer)))
	{
		return false;
	}

	return true;
}
//=============================
// SE停止.
//=============================
bool CXAudio2PlaySE::SeStop(const size_t ArrayNum)
{
	if (m_pSourceVoice[ArrayNum] == nullptr) return true;

	m_pSourceVoice[ArrayNum]->Stop(0);
	m_pSourceVoice[ArrayNum]->FlushSourceBuffers();
	return true;
}
//=============================
// 作成してる予備含む全てのSE停止.
//=============================
bool CXAudio2PlaySE::AllSeStop()
{
	if (m_pSourceVoice[0] == nullptr) return true;

	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array]->Stop(0);
		m_pSourceVoice[Array]->FlushSourceBuffers();
	}

	return true;
}
//=============================
// Oggデータ作成.
//=============================
HRESULT CXAudio2PlaySE::CreateOggSound()
{
	//一回データが作られていたらリターン.
	if (m_pOggWavData == nullptr) return E_FAIL;
	HRESULT hr;
	// XAudio2マスターボイスのインスタンス取得.
	CXAudio2MasterVoice& xAudioMaster = CXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();
	// リバーブ用のIUnknown用意.
	IUnknown* pXAPO;
	// リバーブ作成.
	XAudio2CreateReverb(&pXAPO);

	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.pEffect = pXAPO;
	desc.InitialState = TRUE;			// エフェクトを有効状態で初期化するかどうか(EnableEffect(0)を呼ばなくてもいい).
	desc.OutputChannels = 1;			// エフェクトが生成する出力チャンネルの数.
	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;				// ボイスのエフェクトチェーン内のエフェクトの数今回はリバーブのみ.
	chain.pEffectDescriptors = &desc;	// XAUDIO2_EFFECT_DESCRIPTOR.

	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {

		m_pSubmixVoice[Array] = nullptr;
		m_pSubmixVoice[Array] = { 0 };
		// サブミックスボイスの作成.
		if (FAILED(hr = pHandle->CreateSubmixVoice(&m_pSubmixVoice[Array], 1,
			xAudioMaster.m_SampleRate, 0, 0,
			nullptr, &chain)))
		{
			_ASSERT_EXPR(false, L"Failed Create SubMixVoice");
			return hr;
		}
		// デフォルトエフェクトパラメータをセットする.
		XAUDIO2FX_REVERB_PARAMETERS native;
		XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
		ReverbConvertI3DL2ToNative(&PRESET_PARAMS, &native);
		m_pSubmixVoice[Array]->SetEffectParameters(0, &native, sizeof(native));
		// IUnknownの破棄.
		// ここで破棄することで、IUnknownの参照カウンタが1になり、所有権をソースボイスが持つ？.

		// 別のボイスからの送信のターゲットである宛先ボイスをソースにセットする.
		XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
		// 下で指定した送信先にフィルターを使用する 0 or XAUDIO2_SEND_USEFILTER.
		sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path.
		sendDescriptors[0].pOutputVoice = xAudioMaster.GetMasteringVoice();
		// 下で指定した送信先にフィルターを使用する 0 or XAUDIO2_SEND_USEFILTER.
		sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path 省略することで、よりリアルなオクルージョンを犠牲にしてパフォーマンスを向上させることができます.
		sendDescriptors[1].pOutputVoice = m_pSubmixVoice[Array];
		// リストを用意、SEND_DESCRIPTORの数,変数.
		const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };


		if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice[Array], m_pOggWavData->GetFormat())))
		{
			// 失敗した.
			_ASSERT_EXPR("error %#X creating se source voice\n", hr);
			return E_FAIL;
		}
		m_pX3DAudio[Array] = std::make_unique<CX3DAudio>(m_pSourceVoice[Array], m_pSubmixVoice[Array], 2);
		if (FAILED(hr))
			return E_FAIL;
	}
	pXAPO->Release();

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}
//=============================
// 聞き手に設定する構造体ポインターをセットする.
//=============================
void CXAudio2PlaySE::SetListenerInfo(CX3DAudio::AUDIO_LISTENER& stListenerData)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetListenerData(stListenerData);
	}
}
//=============================
// 音源に設定する構造体ポインターをセットする.
//=============================
void CXAudio2PlaySE::SetEmitterInfo(CX3DAudio::AUDIO_EMITTER& stEmitterData)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetEmitterData(stEmitterData);
	}
}
//=============================
// リバーブを設定.
//=============================
HRESULT CXAudio2PlaySE::SetReverb(const XAudio2Reverb::enReverbPreset Preset)
{
	if (m_pX3DAudio[0] == nullptr) return E_FAIL;
	HRESULT hr;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		hr = m_pX3DAudio[Array]->SetReverb(Preset);
	}
	return hr;
}
//=============================
// リスナーのコーンを使用するかのフラグをセットする.
//=============================
void CXAudio2PlaySE::SetUseListenerConeFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pX3DAudio[Array]->SetUseListenerConeFlag(isFlag);
	}
}
//=============================
// InnerRadiusを使用するかのフラグをセットする.
//=============================
void CXAudio2PlaySE::SetUseInnerRadiusFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		if (m_pX3DAudio != nullptr) m_pX3DAudio[Array]->SetUseInnerRadiusFlag(isFlag);
	}
}
//=============================
// ドップラー効果を使用するかのフラグをセットする.
//=============================
void CXAudio2PlaySE::SetUseDopplerFlag(const bool isFlag)
{
	if (m_pX3DAudio[0] == nullptr) return;
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		if (m_pX3DAudio != nullptr) m_pX3DAudio[Array]->SetUseDopplerFlag(isFlag);
	}
}

//=============================
// 音量取得.
//=============================
const float CXAudio2PlaySE::GetSEVolume(size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return 0.0f;
	float Volume = 0.0f;

	m_pSourceVoice[Array]->GetVolume(&Volume);

	return Volume;
}
//=============================
// SE音量設定.
//=============================
bool CXAudio2PlaySE::SetSEVolume(float value, size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return false;

	m_pSourceVoice[Array]->SetVolume(value * CSoundManager::GetInstance()->m_stSound.MasterVolume, 0);
	return true;
}
//=============================
// ソースボイス解放.
//=============================
void CXAudio2PlaySE::DestoroySource()
{
	if (m_pSourceVoice[0] != nullptr) {
		for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
			m_pSourceVoice[Array]->DestroyVoice();
			m_pSourceVoice[Array] = nullptr;
		}
	}
}
