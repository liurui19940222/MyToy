#ifndef _MAKER_H_
#define _MAKER_H_

#include<map>
#include<vector>
#include<functional>
#include"Object.h"
#include"GameObject.h"
#include"DynamicFactory.h"
#include"Singleton.h"
#include"Math.h"

#define _Maker CMaker::GetInstance()

using namespace std;

typedef function<bool(CGameObject*, int)> ForeachGoCallbackR;
typedef function<void(CGameObject*, int)> ForeachGoCallback;
typedef function<void(CGameObject*, int, Matrix4x4&)> ForeachGoCallbackMatrix;

class ENGINE_API CMaker : public CSingleton<CMaker>
{
private:
	friend class CGameObject;
	friend class CSingleton<CMaker>;

	//所有的根节点(没有父物体的)GameObject
	vector<CGameObject*> m_gameObjects;

	//所有Object但不包括GameObject
	map<int, Object*> m_objects;

	void AddGameObject(CGameObject* go);
	void RemoveGameObject(CGameObject* go);
	void DestroyGameObject(CGameObject* go);
	bool ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth);
	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth);
	void ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback, int depth, Matrix4x4& modelMatrix);
	virtual void OnInitialize() override;
public:
	template<typename T>
	T* Instantiate()
	{
		Object* obj = new T();
		obj->OnInitialize();
		if (IS_TYPE(CGameObject, obj))
		{
			CGameObject* go = dynamic_cast<CGameObject*>(obj);
			AddGameObject(go);
			go->OnStart();
		}
		else
		{
			m_objects.insert(make_pair(obj->GetInstanceId(), obj));
		}
		return dynamic_cast<T*>(obj);
	}

	CGameObject* Instantiate();

	CGameObject* Instantiate(string name);

	template<typename T>
	void Destroy(T* obj)
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
				m_objects.erase(it);
			}
			obj->OnRelease();
			delete(obj);
		}
	}

	void ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback);

	void ForeachGameObjectR(ForeachGoCallbackR callback);

	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback);

	void ForeachGameObject(ForeachGoCallback callback);

	void ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback);

	void ForeachGameObject(ForeachGoCallbackMatrix callback);
};

#endif