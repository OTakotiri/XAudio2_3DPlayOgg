#ifndef X3DAUDIO_H
#define X3DAUDIO_H

#include "..\XAudio2Vector.h"
#include "..\XAudio2.h"
#include <x3daudio.h>	// XAudio2.hより手前でインクルードするとWindows.h 又は windef.hと競合する.
class CX3DAudio
{
public:
	static constexpr int MAXOUTPUT_CHANNEL	= 8;
	static constexpr int INPUTCHANNEL		= 1;
public:
	// 3Dサウンドリスナー(聞き手)データ.
	struct AUDIO_LISTENER
	{
		XAudio2Vec FrontVec;	// 正面ベクトル.
		XAudio2Vec Up;			// 上ベクトル(正面ベクトルと正規直交である必要あり).
		XAudio2Vec Pos;			// リスナーの座標.
		XAudio2Vec Velocity;	// ドッペルで使う加速度.
		AUDIO_LISTENER()
			: FrontVec	( 0.0f,0.0f,1.0f )
			, Up		( 0.0f,1.0f,0.0f )
			, Pos		( 0.0f,0.0f,0.0f )
			, Velocity	( 0.0f,0.0f,0.0f )
		{}
	};
	// 3Dサウンドエミッター(音源)データ.
	// リスナーデータ構造体も持つ.
	struct AUDIO_EMITTER : AUDIO_LISTENER
	{
		float InnerRadius;				// 内半径の計算に使用される値(0f ~ MAX_FLT)、この値が0でもInnerRadiusAngleは使用可能 .
		float InnerRadiusAngle;			// 内半径角度の計算に使用される値(0f ~ X3DAUDIO_PI /4.0f).
		unsigned int ChannelCount;		// X3DAUDIO_EMITTER構造体によって定義されたエミッターの数。0より大きくなければならない.
		float ChannelRadius;			// チャネルが配置される位置からの距離、行列計算のマルチチャネルエミッタでのみ使用される。0.0f以上である必要がある.
		float CurveDistanceScaler;		// 音の聞こえる範囲距離"など"に使用される.
										// 距離が0.0fとCurveDistanceScaler ×1.0fの間にある場合、減衰が適用されないように逆二乗の法則に準拠する特殊なデフォルト曲線を指定.
										// 距離がCurveDistanceScaler ×1.0fより大きい場合、増幅率は（CurveDistanceScaler ×1.0f）/距離.
										// CurveDistanceScaler ×2.0fの距離では、サウンドは半分のボリュームまたは-6 dBになり.
										// CurveDistanceScaler ×4.0fの距離では、サウンドは1/4ボリュームまたは-12dBになる.
		float DopplerScaler;			// ドップラーシフト効果を誇張するために使用されるドップラーシフトスケーラー(0f ~ FLT_MAX).
		AUDIO_EMITTER()
			: InnerRadius			( 2.0f )
			, InnerRadiusAngle		( 1.0f )
			, ChannelRadius			( 0.0f )
			, CurveDistanceScaler	( 14.0f )											
			, DopplerScaler			( 1.0f )
			, ChannelCount			( 2 )
		{}
	};
private:
	// このクラス内で使う3DAudioステート構造体.
	struct stAudioState
	{
		X3DAUDIO_DSP_SETTINGS	DspSettings;		// DSP取得用.
		X3DAUDIO_LISTENER		Listener;			// リスナー構造体.
		X3DAUDIO_EMITTER		Emitter;			// エミッター構造体.
		X3DAUDIO_CONE			EmitterCone;		// エミッターのコーン.
		FLOAT32					MatrixCoefficients[INPUTCHANNEL * MAXOUTPUT_CHANNEL];
		bool					IsInit;				// 初期化完了フラグ.
		bool					IsUseListenerCone;	// リスナーのコーンを使用するか.
		bool					IsUseInnerRadius;	// InnerRadiusを使用するか.
		bool					IsUseDoppler;		// ドッペル効果を使用するか.
		stAudioState()
			: IsInit				( false )
			, IsUseListenerCone		( true )
			, IsUseInnerRadius		( true )
			, IsUseDoppler			( false )
			, MatrixCoefficients	()
		{
			DspSettings		= { 0 };
			Listener		= { 0 };
			Emitter			= { 0 };
			EmitterCone		= { 0 };
		}
	};
public:
	CX3DAudio() {};

	CX3DAudio(IXAudio2SourceVoice* pSource, IXAudio2SubmixVoice* pSubMix,UINT SrcChannelCount);
	~CX3DAudio();

	// Listener構造体ポインターをセットする.
	void SetListenerData(AUDIO_LISTENER& stListenerData);
	// 聞き手の正面ベクトルを設定.
	void SetListenerFront(const XAudio2Vec vec);
	// 聞き手の上方向ベクトルを設定.
	// この値は正面ベクトルに直交していなければならない.
	void SetListenerUp(const XAudio2Vec vec);
	// 聞き手のワールド位置を設定.
	void SetListenerPosition(const XAudio2Vec vec);
	// 聞き手の加速度を設定.
	// 加速度はドップラー効果に利用される(必須ではない).
	void SetListenervelocity(const XAudio2Vec vec);
	// Emitter構造体ポインターをセットする.
	void SetEmitterData(AUDIO_EMITTER& stEmitterData);
	// 音源の正面ベクトルを設定.
	void SetEmitterFront(const XAudio2Vec vec);
	// 音源の上方向ベクトルを設定.
	// この値は正面ベクトルに直交していなければならない.
	void SetEmitterUp(const XAudio2Vec vec);
	// 音源のワールド座標を設定(3Dサウンド再生時).
	void SetEmitterPosition(const XAudio2Vec vec);
	// 音源の加速度を設定.
	// 加速度はドップラー効果に利用される(必須ではない).
	void SetEmittervelocity(const XAudio2Vec vec);
	// ドップラー効果の強さを指定.
	void SetEmitterDopplerLevel(const float value);
	// 音源の効果領域を指定.
	void SetEmitterRadius(const float value);
	// 3Dサウンドの情報を設定する.
	bool Apply3DSoundValue(const bool CallOnce = false);
	// リスナーのコーンを使用するかのフラグをセットする.
	inline void SetUseListenerConeFlag(const bool isFlag)	{ m_stAudioState.IsUseListenerCone	= isFlag; }
	// InnerRadiusを使用するかのフラグをセットする.
	inline void SetUseInnerRadiusFlag(const bool isFlag)	{ m_stAudioState.IsUseInnerRadius	= isFlag; }
	// ドップラー効果を使用するかのフラグをセットする.
	// 有効の場合、音のピッチの変更はできなくなる.
	inline void SetUseDopplerFlag(const bool isFlag)		{ m_stAudioState.IsUseDoppler		= isFlag; }
	// サウンドの計算を終了する.
	inline void StopCalc3DValue()							{ m_IsPlayEnd = true; }
	// すでに用意されているリバーブを設定する.
	HRESULT SetReverb(const XAudio2Reverb::enReverbPreset nReverb);
	// 解放.
	void Release();
private:
	HRESULT	Init();
private:
	stAudioState			m_stAudioState;
	AUDIO_EMITTER			m_InitEmitter;		// null防止初期設定用Emitter構造体.
	AUDIO_EMITTER*			m_Emitter;			// エミッターデータ構造体.
	AUDIO_EMITTER			m_InitListener;		// null防止初期設定用Listener構造体.
	AUDIO_LISTENER*			m_Listener;			// リスナーデータ構造体.
	IXAudio2SourceVoice*	m_pSourceVoice;		// サウンドを再生するストリーム.
	IXAudio2SubmixVoice*	m_pSubmixVoice;		// サブミックスボイス.
	XAUDIO2FX_REVERB_PARAMETERS m_ReverbParameters;	// リバーブのパラメータ.
	bool					m_IsPlayEnd;		// 再生終了.
	UINT					m_SrcChannelCount;	// 出力先チャンネルの数.
												// ここに不正な値を入れると、音源のパンが寄ったり、音割れ音量で出力されるので注意.
};



#endif // !X3DAUDIO2_H
