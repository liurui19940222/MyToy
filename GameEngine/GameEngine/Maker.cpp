#include"Maker.h"
#include"Texture2D.h"
#include"Material.h"

void CMaker::OnInitialize()
{

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

	ForeachGameObject(go, [](CGameObject* go, int depth, Matrix4x4& mat) {
		go->OnDestroy();
		go->OnRelease();
	});
}

void CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObjectR(go, callback, 0, go->GetModelToWorldMat());
}

void CMaker::ForeachGameObjectR(ForeachGoCallbackR callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (!ForeachGameObjectR(*it, callback, 0, (*it)->GetModelToWorldMat()))
			break;
		it++;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0, go->GetModelToWorldMat());
}

void CMaker::ForeachGameObject(ForeachGoCallback callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it, callback, 0, (*it)->GetModelToWorldMat());
		it++;
	}
}

bool CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth, Matrix4x4& modelMatrix)
{
	if (!callback(go, depth, modelMatrix)) return false;
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		if (!ForeachGameObjectR(*it, callback, depth + 1, modelMatrix * (*it)->GetModelToWorldMat()))
			return false;
	}
	return true;
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth, Matrix4x4& modelMatrix)
{
	callback(go, depth, modelMatrix);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1, modelMatrix * (*it)->GetModelToWorldMat());
	}
}