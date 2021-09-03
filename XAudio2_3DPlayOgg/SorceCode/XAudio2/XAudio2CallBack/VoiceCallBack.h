#ifndef VOICE_CALLBACK
#define VOICE_CALLBACK


#include "..\XAudio2.h"
#include <stdio.h>

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE event; // イベント
	// コンストラクタ
	VoiceCallback() : event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	// デストラクタ
	~VoiceCallback() { CloseHandle(event); }

	//
	// 以下コールバック
	//
	// ボイスが連続オーディオストリームの再生を終了したときに呼び出される
	void STDMETHODCALLTYPE OnStreamEnd() { printf("%s\n", __func__); }
	// ボイスの終了のプロセッシングパスの直後に呼び出される
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
	// ボイス開始のプロセッシングパスの直前に呼び出される
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	// ボイスがバッファーの処理を終了したときに呼び出される
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) { SetEvent(event); }
	// ボイスが新しいオーディオバッファーの処理を開始しようとするときに呼び出される
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {}
	// ボイスがループの最後に到達したときに呼び出される
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) { printf("%s\n", __func__); }
	// ボイスの処理中に重大なエラーが発生したときに呼び出される
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) { printf("%s\n", __func__); }
};

#endif // #ifndef VOICE_CALLBACK