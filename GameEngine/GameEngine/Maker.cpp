#include "Maker.h"

CMaker* CMaker::m_instance = NULL;

CMaker* CMaker::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new CMaker();
	return m_instance;
}

CGameObject* CMaker::Instantiate()
{
	return Instantiate("NewGameObject");
}

CGameObject* CMaker::Instantiate(string name)
{
	CGameObject* go = Instantiate<CGameObject>();
	go->SetName(name);
	return go;
}

void CMaker::Destroy(Object* obj)
{
	if (IS_TYPE(CGameObject, obj))
	{
		DestroyGameObject(dynamic_cast<CGameObject*>(obj));
	}
	else
	{
		auto it = m_objects.find(obj->GetInstanceId());
		if (it != m_objects.end())
		{
			Object* obj = it->second;
			m_objects.erase(it);
			obj->OnRelease();
			delete(obj);
		}
	}
}

void CMaker::AddGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			return;
		}
		it++;
	}
	m_gameObjects.push_back(go);
}

void CMaker::RemoveGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			m_gameObjects.erase(it);
			return;
		}
		it++;
	}
}

void CMaker::DestroyGameObject(CGameObject* go)
{
	if (go->parent)
		go->parent->RemoveChild(go);
	Maker->RemoveGameObject(go);

	ForeachGameObject(go, [](CGameObject* go, int depth) {
		go->OnDestroy();
		go->OnRelease();
	});
}

void CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObjectR(go, callback, 0);
}

void CMaker::ForeachGameObjectR(ForeachGoCallbackR callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (!ForeachGameObjectR(*it++, callback, 0))
			break;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0);
}

void CMaker::ForeachGameObject(ForeachGoCallback callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it++, callback, 0);
	}
}

bool CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth)
{
	if (!callback(go, depth)) return false;
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		if (!ForeachGameObjectR(*it, callback, depth + 1))
			return false;
	}
	return true;
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth)
{
	callback(go, depth);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1);
	}
}