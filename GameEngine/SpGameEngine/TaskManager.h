#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_

#include<functional>
#include<map>
#include"SpCommon\ApiDefine.h"

using namespace std;

struct ENGINE_API STask
{
	typedef function<void(void*)> TaskCall;
	TaskCall m_callback;
	void* m_params;
	float m_delayTime = 0;
	float m_timer = 0;

	inline void Call()
	{
		m_callback(m_params);
	}

	STask();
	STask(TaskCall call, void* param, float delayTime);
};

class ENGINE_API CTaskManager
{
	static int m_identity;
	static map<int, STask> m_tasks;
public:

	static int AddTask(STask task);
	static void OnUpdate();
};

#endif