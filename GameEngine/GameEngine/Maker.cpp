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
	_Maker->RemoveGameObject(go);

	vector<CGameObject*> list;
	ForeachGameObject(go, [&list](CGameObject* go, int depth) {
		go->OnDestroy();
		go->OnRelease();
		list.push_back(go);
	});
	for (int i = list.size() - 1; i >= 0; --i)
		delete(list[i]);
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
		if (!ForeachGameObjectR(*it, callback, 0))
			break;
		it++;
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
		ForeachGameObject(*it, callback, 0);
		it++;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0, go->GetModelToWorldMat());
}

void CMaker::ForeachGameObject(ForeachGoCallbackMatrix callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it, callback, 0, (*it)->GetModelToWorldMat());
		it++;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback, int depth, Matrix4x4& modelMatrix)
{
	callback(go, depth, modelMatrix);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1, modelMatrix * (*it)->GetModelToWorldMat());
	}
}