#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H
#pragma comment(lib,"winmm.lib")
#include <windows.h>
#include <mmsystem.h>

class CFPSCounter
{
private:
	static constexpr float MIN_FRAME_TIME = 1.0f / 60.0f;
public:
	CFPSCounter();
	~CFPSCounter();
	
	bool UpdateFPS();

	float GetFPS() { return m_fFPS; }
	static float GetFrameTime() { return m_fFrameTime; }
private:
	void Init();
private:
	static float	m_fFrameTime;
	LARGE_INTEGER	m_TimeStart;
	LARGE_INTEGER	m_TimeEnd;
	LARGE_INTEGER	m_TimeFreq;
	float			m_fFPS;
};

#endif // !FPSCOUNTER_H
