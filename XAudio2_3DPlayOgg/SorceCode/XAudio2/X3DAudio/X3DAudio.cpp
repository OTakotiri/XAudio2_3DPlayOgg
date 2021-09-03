#include "X3DAudio.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
//=================================
// 定数宣言.
namespace {
	// LisnerのCone設定用.
	constexpr X3DAUDIO_CONE LISTENER_DIRECTIONALCONE		= { X3DAUDIO_PI*5.0f / 6.0f, X3DAUDIO_PI*11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };
	// EmitterのLFEのカーブポイント設定用.
	constexpr X3DAUDIO_DISTANCE_CURVE_POINT EMITTER_LFE_CURVEP_POINTS[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	// EmitterのLFEのカーブ設定用.
	constexpr X3DAUDIO_DISTANCE_CURVE EMITTER_LFE_CURVE		= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&EMITTER_LFE_CURVEP_POINTS[0], 3 };
	// EmitterのReverbカーブポイント設定用.
	constexpr X3DAUDIO_DISTANCE_CURVE_POINT EMITTER_REVERB_CURVEPOINTS[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	// EmitterのReverbカーブ設定用.
	constexpr X3DAUDIO_DISTANCE_CURVE EMITTER_REVERB_CURVE	= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&EMITTER_REVERB_CURVEPOINTS[0], 3 };
}

CX3DAudio::CX3DAudio(IXAudio2SourceVoice* pSource, IXAudio2SubmixVoice* pSubMix, UINT SrcChannelCount)
	: m_pSourceVoice	( pSource )
	, m_pSubmixVoice	( pSubMix )
	, m_SrcChannelCount	( SrcChannelCount )
	, m_IsPlayEnd		( false )
	, m_Emitter			( &m_InitEmitter )
	, m_Listener		( &m_InitListener )
{
	Init();
}

CX3DAudio::~CX3DAudio()
{
	Release();
}
//=============================
// Listener構造体ポインターをセットする.
//=============================
void CX3DAudio::SetListenerData(AUDIO_LISTENER& stListenerData)
{
	m_Listener = &stListenerData;
}
//=============================
// 聞き手の正面ベクトルを設定.
//=============================
void CX3DAudio::SetListenerFront(const XAudio2Vec vec)
{
	m_Listener->FrontVec.x = vec.x;
	m_Listener->FrontVec.y = vec.y;
	m_Listener->FrontVec.z = vec.z;
}
//=============================
// 聞き手の上方向ベクトルを設定.
//=============================
void CX3DAudio::SetListenerUp(const XAudio2Vec vec)
{
	m_Listener->Up.x = vec.x;
	m_Listener->Up.y = vec.y;
	m_Listener->Up.z = vec.z;
}
//=============================
// 聞き手のワールド位置を設定.
//=============================
void CX3DAudio::SetListenerPosition(const XAudio2Vec vec)
{
	m_Listener->Pos.x = vec.x;
	m_Listener->Pos.y = vec.y;
	m_Listener->Pos.z = vec.z;
}
//=============================
// 聞き手の加速度を設定.
//=============================
void CX3DAudio::SetListenervelocity(const XAudio2Vec vec)
{
	m_Listener->Velocity.x = vec.x;
	m_Listener->Velocity.y = vec.y;
	m_Listener->Velocity.z = vec.z;
}
//=============================
// Emitter構造体ポインターをセットする.
//=============================
void CX3DAudio::SetEmitterData(AUDIO_EMITTER& stEmitterData)
{
	m_Emitter = &stEmitterData;
}
//=============================
// 音源の正面ベクトルを設定.
//=============================
void CX3DAudio::SetEmitterFront(const XAudio2Vec vec)
{
	m_Emitter->FrontVec.x = vec.x;
	m_Emitter->FrontVec.y = vec.y;
	m_Emitter->FrontVec.z = vec.z;
}
//=============================
// 音源の上方向ベクトルを設定.
//=============================
void CX3DAudio::SetEmitterUp(const XAudio2Vec vec)
{
	m_Emitter->Up.x = vec.x;
	m_Emitter->Up.y = vec.y;
	m_Emitter->Up.z = vec.z;
}
//=============================
// 音源のワールド座標を設定(3Dサウンド再生時).
//=============================
void CX3DAudio::SetEmitterPosition(const XAudio2Vec vec)
{
	m_Emitter->Pos.x = vec.x;
	m_Emitter->Pos.y = vec.y;
	m_Emitter->Pos.z = vec.z;
}
//=============================
// 音源の加速度を設定.
//=============================
void CX3DAudio::SetEmittervelocity(const XAudio2Vec vec)
{
	m_Emitter->Velocity.x = vec.x;
	m_Emitter->Velocity.y = vec.y;
	m_Emitter->Velocity.z = vec.z;
}
//=============================
// ドップラー効果の強さを指定.
//=============================
void CX3DAudio::SetEmitterDopplerLevel(const float value)
{
	m_Emitter->DopplerScaler = value;
}
//=============================
// 音源の効果領域を指定.
//=============================
void CX3DAudio::SetEmitterRadius(const float value)
{
	m_Emitter->InnerRadius = value;
}

HRESULT CX3DAudio::SetReverb(const XAudio2Reverb::enReverbPreset nReverb)
{
	if (!m_stAudioState.IsInit) // 初期化が終わっていなければ失敗.
		return S_FALSE;

	if (nReverb < XAudio2Reverb::enReverbPreset::en_PRESET_FOREST || nReverb > XAudio2Reverb::enReverbPreset::Max)
		return E_FAIL;

	if (m_pSubmixVoice)
	{
		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative(&XAudio2Reverb::PRESET_PARAMS[static_cast<int>(nReverb)], &native);
		m_pSubmixVoice->SetEffectParameters(0, &native, sizeof(native));
	}

	return S_OK;
}

void CX3DAudio::Release()
{
	m_pSourceVoice = nullptr;
	m_pSubmixVoice = nullptr;
	m_Listener = nullptr;
	m_Emitter = nullptr;
}

HRESULT CX3DAudio::Init()
{
	
	// コーン設定、別になくてもよい.
	// また、エミッターのコーンは初期化時だけでよさげ.
	m_stAudioState.Emitter.pCone = &m_stAudioState.EmitterCone;
	m_stAudioState.Emitter.pCone->InnerAngle	= 0.0f;
	// 外側のコーンの角度を0以外にすると.
	// 内側のコーンの設定のみで、ポイントエミッターのように動作します.
	m_stAudioState.Emitter.pCone->OuterAngle	= 0.0f;
	// 外側のコーンの角度を0にすると.
	// エミッターは、外円錐の設定のみを使用したポイントエミッターのように動作します.
	// 外側のコーンの設定のみで、ポイントエミッターのように動作します.
	m_stAudioState.Emitter.pCone->InnerVolume	= 0.0f;
	m_stAudioState.Emitter.pCone->OuterVolume	= 1.0f;
	m_stAudioState.Emitter.pCone->InnerLPF		= 0.0f;
	m_stAudioState.Emitter.pCone->OuterLPF		= 1.0f;
	m_stAudioState.Emitter.pCone->InnerReverb	= 0.0f;
	m_stAudioState.Emitter.pCone->OuterReverb	= 1.0f;
	// X3DAudioDefault_LinearCurveはX3DAudio.hに宣言されてる.
	m_stAudioState.Emitter.pVolumeCurve			= (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	// LFEレベルの距離曲線を、LFE以外のチャンネルよりも早くロールオフするように指定する.
	// サブウーファーをより効果的に使うことができます.
	m_stAudioState.Emitter.pLFECurve			= (X3DAUDIO_DISTANCE_CURVE*)&EMITTER_LFE_CURVE;
	m_stAudioState.Emitter.pLPFDirectCurve		= nullptr;	// デフォルトを使用.
	m_stAudioState.Emitter.pLPFReverbCurve		= nullptr;	// デフォルトを使用.
	// リバーブセンドレベルの距離曲線を指定し、リバーブセンドが.
	// 距離に応じてリバーブセンドがわずかに増加した後、無音になるようにします.
	// ダイレクトチャンネルは距離とともに減衰していきます.
	// これはリバーブとダイレクトの比率を高める効果がある.
	// これにより、リバーブと直接音の比率が高まり、距離感が強調されます.
	m_stAudioState.Emitter.pReverbCurve				= (X3DAUDIO_DISTANCE_CURVE*)&EMITTER_REVERB_CURVE;
	m_stAudioState.Emitter.ChannelCount				= 1;
	m_stAudioState.Emitter.ChannelRadius			= 1.0f;

	m_stAudioState.DspSettings.SrcChannelCount		= m_SrcChannelCount;		// 出力チャネル数.
	m_stAudioState.DspSettings.DstChannelCount		= CXAudio2MasterVoice::GetGlobalSystem().m_InputChannels;
	m_stAudioState.DspSettings.pMatrixCoefficients	= m_stAudioState.MatrixCoefficients;

	m_stAudioState.IsInit = true;

	return S_OK;
}

bool CX3DAudio::Apply3DSoundValue(const bool CallOnce)
{
	if (m_pSourceVoice == nullptr) return false;

	// XAudio2マスターボイスのインスタンス取得.
	CXAudio2MasterVoice& xAudioMaster = CXAudio2MasterVoice::GetGlobalSystem();

	
	do{
		// コーンを使用する場合適用.
		if (m_stAudioState.IsUseListenerCone)
		{
			m_stAudioState.Listener.pCone = (X3DAUDIO_CONE*)&LISTENER_DIRECTIONALCONE;
		}
		else
		{
			m_stAudioState.Listener.pCone = nullptr;
		}
		// インナーラディウスを使う場合適用.
		if (m_stAudioState.IsUseInnerRadius)
		{
			m_stAudioState.Emitter.InnerRadius = m_Emitter->InnerRadius;
			m_stAudioState.Emitter.InnerRadiusAngle = m_Emitter->InnerRadiusAngle;
		}
		else
		{
			m_stAudioState.Emitter.InnerRadius = 0.0f;
			m_stAudioState.Emitter.InnerRadiusAngle = 0.0f;
		}
		// 正面ベクトルセット.
		m_stAudioState.Listener.OrientFront.x = m_Listener->FrontVec.x;
		m_stAudioState.Listener.OrientFront.y = m_Listener->FrontVec.y;
		m_stAudioState.Listener.OrientFront.z = m_Listener->FrontVec.z;
		// 上ベクトルセット.
		m_stAudioState.Listener.OrientTop.x = m_Listener->Up.x;
		m_stAudioState.Listener.OrientTop.y = m_Listener->Up.y;
		m_stAudioState.Listener.OrientTop.z = m_Listener->Up.z;
		// 座標セット.
		m_stAudioState.Listener.Position.x = m_Listener->Pos.x;
		m_stAudioState.Listener.Position.y = m_Listener->Pos.y;
		m_stAudioState.Listener.Position.z = m_Listener->Pos.z;
		// 加速度計算.
		// XAudio2Vec Pos = { m_Listener.Pos.x ,m_Listener.Pos.y,m_Listener.Pos.z };
		// XAudio2Vec Pos2 = { listener.Position.x ,listener.Position.y,listener.Position.z };
		// m_Listener.Velocity = (Pos2 - Pos) / fElapsedTime;
		m_stAudioState.Listener.Velocity.x = m_Listener->Velocity.x;
		m_stAudioState.Listener.Velocity.y = m_Listener->Velocity.y;
		m_stAudioState.Listener.Velocity.z = m_Listener->Velocity.z;
		
		// エミッターに情報をセット.
		// 正面ベクトルセット.
		m_stAudioState.Emitter.OrientFront.x = m_Emitter->FrontVec.x;
		m_stAudioState.Emitter.OrientFront.y = m_Emitter->FrontVec.y;
		m_stAudioState.Emitter.OrientFront.z = m_Emitter->FrontVec.z;
		// 上ベクトルセット.
		m_stAudioState.Emitter.OrientTop.x = m_Emitter->Up.x;
		m_stAudioState.Emitter.OrientTop.y = m_Emitter->Up.y;
		m_stAudioState.Emitter.OrientTop.z = m_Emitter->Up.z;
		// 座標セット.
		m_stAudioState.Emitter.Position.x = m_Emitter->Pos.x;
		m_stAudioState.Emitter.Position.y = m_Emitter->Pos.y;
		m_stAudioState.Emitter.Position.z = m_Emitter->Pos.z;
			
		// 加速度計算.
		// XAudio2Vec Pos = { m_Emitter.Pos.x ,m_Emitter.Pos.y,m_Emitter.Pos.z };
		// XAudio2Vec Pos2 = { m_stAudioState.Emitter.Position.x ,m_stAudioState.Emitter.Position.y,m_stAudioState.Emitter.Position.z };
		// m_Emitter.Velocity = (Pos2 - Pos) / fElapsedTime;
		m_stAudioState.Emitter.Velocity.x = m_Emitter->Velocity.x;
		m_stAudioState.Emitter.Velocity.y = m_Emitter->Velocity.y;
		m_stAudioState.Emitter.Velocity.z = m_Emitter->Velocity.z;
		// 範囲.
		m_stAudioState.Emitter.CurveDistanceScaler = m_Emitter->CurveDistanceScaler;
		
		// 3D計算.
		X3DAudioCalculate(xAudioMaster.m_x3DHandle,
			&m_stAudioState.Listener,
			&m_stAudioState.Emitter,
			X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
			| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
			| X3DAUDIO_CALCULATE_REVERB,
			&m_stAudioState.DspSettings);	// out.

		if (m_pSourceVoice != nullptr && m_IsPlayEnd == false)
		{
			// Submixだけ呼ぶと、3Dが適応されてない鳴り方になる.

			// ドップラー効果を使用するなら適応.
			if (m_stAudioState.IsUseDoppler == true) m_pSourceVoice->SetFrequencyRatio(m_stAudioState.DspSettings.DopplerFactor);
			m_pSourceVoice->SetOutputMatrix(xAudioMaster.GetMasteringVoice(), 1, xAudioMaster.m_InputChannels, m_stAudioState.MatrixCoefficients);
			m_pSourceVoice->SetOutputMatrix(m_pSubmixVoice, 1, 1, &m_stAudioState.DspSettings.ReverbLevel);
			// XAUDIO2_FILTER_PARAMETERS.Frequencyで使用されているフィルタのカットオフ周波数を、ヘルツで表現されたものからラジアンに変換します.
			// XAUDIO2_FILTER_PARAMETERS.Frequencyで使用される周波数値、状態変数.
			// フィルタータイプのみ。 1 ポールのフィルタータイプには、XAudio2CutoffFrequencyToOnePoleCoefficient() を使用してください.
			// サポートされる最高のCutoffFrequencyはSampleRate/6であることに注意.
			// CutoffFrequencyの値が大きいと、XAUDIO2_MAX_FILTER_FREQUENCYが返される.
			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_stAudioState.DspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			m_pSourceVoice->SetOutputFilterParameters(xAudioMaster.GetMasteringVoice(), &FilterParametersDirect);
			// see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_stAudioState.DspSettings.LPFReverbCoefficient), 1.0f };
			m_pSourceVoice->SetOutputFilterParameters(m_pSubmixVoice, &FilterParametersReverb);
		}
		else {
			return false;
		}
	} while (CallOnce == false && !m_IsPlayEnd);

	return true;
}
