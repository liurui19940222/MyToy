#ifndef _CTIME_
#define _CTIME_

#include<Windows.h>
#include<windowsx.h>
#include"SpCommon\ApiDefine.h"

class ENGINE_API CTime
{
private:
	static float fpsUpdateInterval; // ˢ��fps�ļ��ʱ��
	static int curSecondFrameCount; //����һ�μ���fps�������0�ٴ��ۼӵ���֡��
	static int targetFrameCount; //Ŀ��ÿ���֡��
	static float lastFpsUpdateTime;
	static DWORD startTick; //��ʼ��Ϸʱ��tick
	static DWORD frameStartTick; //��ǰ֡��ʼʱ��tick
public:
	static float deltaTime; //�����һ֡�����ѵ�ʱ��
	static float time; //�ӳ�������ʱ����������ʱ��
	static float lastFrameTime; //�ӳ�������ʱ����������ʱ��--��һ֡
	static float fps; //֡����
	static double oneFrameTime; //һ֡����Ҫ��ʱ��

	static void SetTargetFrameCount(int count);
	static int GetTargetFrameCount();
	static void InitTime();
	static void UpdateTime();
	static void Lock();
};

#endif