#include "Time.h"

float CTime::fpsUpdateInterval = 0.5f; // 刷新fps的间隔时间
float CTime::lastFpsUpdateTime = 0;
int CTime::curSecondFrameCount = 0;
int CTime::targetFrameCount = 0;
double CTime::oneFrameTime = 0;
float CTime::deltaTime = 0;
float CTime::time = 0;
float CTime::fps = 0;
float CTime::lastFrameTime = 0;
DWORD CTime::startTick = 0;
DWORD CTime::frameStartTick = 0;

void CTime::SetTargetFrameCount(int count)
{
	targetFrameCount = count;
	//oneFrameTime = 1.0 / (float)count;
	oneFrameTime = 1000.0 / (double)count;
}

int CTime::GetTargetFrameCount()
{
	return targetFrameCount;
}

void CTime::InitTime()
{
	startTick = GetTickCount();
	lastFrameTime = time;
	lastFpsUpdateTime = time;
	frameStartTick = startTick;
}

void CTime::UpdateTime()
{
	lastFrameTime = time;
	time = (GetTickCount() - startTick) / 1000.0f;
	deltaTime = time - lastFrameTime;

	curSecondFrameCount++;
	if (time - lastFpsUpdateTime >= fpsUpdateInterval)
	{
		fps = curSecondFrameCount / (time - lastFpsUpdateTime);
		curSecondFrameCount = 0;
		lastFpsUpdateTime = time;
	}
}

void CTime::Lock()
{
	while ((GetTickCount() - frameStartTick) < oneFrameTime);
	frameStartTick += oneFrameTime;
}