#ifndef _CTIME_
#define _CTIME_

#include<Windows.h>
#include<windowsx.h>
#include"SpCommon\ApiDefine.h"

class ENGINE_API CTime
{
private:
	static float fpsUpdateInterval; // 刷新fps的间隔时间
	static int curSecondFrameCount; //从上一次计算fps结束后从0再次累加的总帧数
	static int targetFrameCount; //目标每秒的帧数
	static float lastFpsUpdateTime;
	static DWORD startTick; //开始游戏时的tick
	static DWORD frameStartTick; //当前帧开始时的tick
public:
	static float deltaTime; //完成上一帧所花费的时间
	static float time; //从程序运行时所经过的总时间
	static float lastFrameTime; //从程序运行时所经过的总时间--上一帧
	static float fps; //帧速率
	static double oneFrameTime; //一帧所需要的时间

	static void SetTargetFrameCount(int count);
	static int GetTargetFrameCount();
	static void InitTime();
	static void UpdateTime();
	static void Lock();
};

#endif