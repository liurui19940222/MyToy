#include "Timer.h"

using namespace dxgame;

#ifdef WIN32

#include<Windows.h>

Win32Timer::Win32Timer() noexcept :
m_OnFrameBeginningCounter(0),
m_BeginningCounter(0),
m_PrevCounter(0),
m_CurCounter(0),
m_TotalFrame(0),
m_PausedCounter(0),
m_RunningCounter(0),
m_FramePerSecond(0.0f),
m_CountPerSecond(0.0),
m_TargetFramePerSecond(1.0 / 60.0),
m_Paused(false)
{

}

bool Win32Timer::init() noexcept
{
	LARGE_INTEGER freq = { 0 };
	QueryPerformanceFrequency(&freq);
	if (freq.QuadPart == 0)
		return false;

	m_CountPerSecond = 1.0 / (double)freq.QuadPart;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_BeginningCounter);
	m_CurCounter = m_PrevCounter = m_BeginningCounter;
	start();
	return true;
}

void Win32Timer::beginFrame() noexcept
{
	m_TotalFrame++;
	m_PrevCounter = m_CurCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurCounter);
	int64 deltaCounter = m_CurCounter - m_PrevCounter;
	if (m_Paused)
	{
		m_PausedCounter += deltaCounter;
	}
	else
	{
		m_RunningCounter += deltaCounter;
	}
	calculateFps();
	m_OnFrameBeginningCounter = getRealCounter();
}

void Win32Timer::endFrame() noexcept
{
	while ((getRealCounter() - m_OnFrameBeginningCounter) * m_CountPerSecond < m_TargetFramePerSecond)
	{

	}
}

void Win32Timer::calculateFps() noexcept
{
	static int64 lastUpdateCounter = 0;
	static double frameCount = 0;
	frameCount++;
	if ((double)(m_CurCounter - lastUpdateCounter) * m_CountPerSecond > 1.0)
	{
		m_FramePerSecond = (float)(frameCount / ((m_CurCounter - lastUpdateCounter) * m_CountPerSecond));
		frameCount = 0;
		lastUpdateCounter = m_CurCounter;
	}
}

void Win32Timer::start() noexcept
{
	m_Paused = false;
}

void Win32Timer::pause() noexcept
{
	m_Paused = true;
}

void Win32Timer::stop() noexcept
{
	m_BeginningCounter = 0;
	m_PrevCounter = 0;
	m_CurCounter = 0;
	m_TotalFrame = 0;
	m_PausedCounter = 0;
	m_RunningCounter = 0;
	m_FramePerSecond = 0.0f;
	m_CountPerSecond = 0.0;
	m_Paused = true;
}

void Win32Timer::setTargetFps(float fps) noexcept
{
	m_TargetFramePerSecond = 1.0 / (double)fps;
}

float Win32Timer::getFps() const noexcept
{
	return m_FramePerSecond;
}

float Win32Timer::getTime() const noexcept
{
	return (float)(m_RunningCounter * m_CountPerSecond);
}

float Win32Timer::getRealTime() const noexcept
{
	static int64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	return (float)((time - m_BeginningCounter) * m_CountPerSecond);
}

int64 Win32Timer::getRealCounter() noexcept
{
	static int64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	return time - m_BeginningCounter;
}

float Win32Timer::getDeltaTime() const noexcept
{
	return m_Paused ? 0.0f : (float)((m_CurCounter - m_PrevCounter) * m_CountPerSecond);
}

#endif

std::shared_ptr<ITimer> TimerService::timer;

std::shared_ptr<ITimer> TimerService::getTimer()
{
	if (timer.get() == nullptr)
	{
#ifdef WIN32
		timer = std::make_shared<Win32Timer>();
		if (!timer->init())
		{
			throw std::exception("Win32Timer init failed.");
		}
#elif
		throw std::exception("the timer of other platform is not define.");
#endif
	}
	return timer;
}