#include "FPSCounter.h"
#include <crtdbg.h>

float CFPSCounter::m_fFrameTime = 0.0f;

CFPSCounter::CFPSCounter()
	: m_fFPS		( 0.0f )
	, m_TimeStart	()
	, m_TimeEnd		()
	, m_TimeFreq	()
{

	Init();
}

CFPSCounter::~CFPSCounter()
{
}

bool CFPSCounter::UpdateFPS()
{
	// 今の時間を取得
	QueryPerformanceCounter(&m_TimeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	m_fFrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	// デバッガーなどで、処理が一回止まった場合、値が吹っ飛ぶのを防止.
	// ※加算などに使用している場合困るため.
	if (m_fFrameTime > 0.05f) {
		m_fFrameTime = 0.05f;
	}

	if (m_fFrameTime < MIN_FRAME_TIME) { // 時間に余裕がある
		// ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_fFrameTime) * 1000);

		timeBeginPeriod(1); // 分解能を上げる(こうしないとSleepの精度はガタガタ)
		Sleep(sleepTime);   // 寝る
		timeEndPeriod(1);   // 戻す

		// 次週に持ち越し(こうしないとfpsが変になる?)
		return false;
	}

	if (m_fFrameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		m_fFPS = (m_fFPS * 0.99f) + (0.01f / m_fFrameTime); // 平均fpsを計算
	}

	m_TimeStart = m_TimeEnd; // 入れ替え
	return true;
}

void CFPSCounter::Init()
{
	// メインループに入る前に精度を取得しておく
	if (QueryPerformanceFrequency(&m_TimeFreq) == FALSE) { // この関数で0(FALSE)が帰る時は未対応
		_ASSERT_EXPR(false, "FPSCounterの初期設定失敗");
	}
	// 1度取得しておく(初回計算用)
	QueryPerformanceCounter(&m_TimeStart);
}
