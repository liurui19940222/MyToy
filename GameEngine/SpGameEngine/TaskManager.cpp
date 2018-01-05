#include"TaskManager.h"
#include"Time.h"

STask::STask()
{

}

STask::STask(TaskCall call, void* param, float delayTime)
{
	m_callback = call;
	m_params = param;
	m_delayTime = delayTime;
}

int CTaskManager::m_identity = 1;
map<int, STask> CTaskManager::m_tasks;

int CTaskManager::AddTask(STask task)
{
	int id = m_identity++;
	m_tasks.insert(make_pair(id, task));
	return id;
}

void CTaskManager::OnUpdate()
{
	float deltaTime = CTime::deltaTime;
	for (auto it = m_tasks.begin(); it != m_tasks.end();)
	{
		it->second.m_timer += deltaTime;
		if (it->second.m_timer >= it->second.m_delayTime)
		{
			it->second.Call();
			it = m_tasks.erase(it);
		}
		else ++it;
	}
}