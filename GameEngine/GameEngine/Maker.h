#ifndef _MAKER_H_
#define _MAKER_H_

#include<map>
#include<vector>
#include<functional>
#include"Object.h"
#include"GameObject.h"
#include"DynamicFactory.h"
#include"Singleton.h"

using namespace std;

typedef function<bool(CGameObject*, int)> ForeachGoCallbackR;
typedef function<void(CGameObject*, int)> ForeachGoCallback;

class CMaker : public CSingleton<CMaker>
{
private:
	friend class CGameObject;

	//所有的根节点(没有父物体的)GameObject
	vector<CGameObject*> m_gameObjects;

	//所有Object但不包括GameObject
	map<int, Object*> m_objects;

	void AddGameObject(CGameObject* go);
	void RemoveGameObject(CGameObject* go);
	void DestroyGameObject(CGameObject* go);
	bool ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth);
	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth);

public:
	template<typename T>
	T* Instantiate()
	{
		Object* obj = DynamicFactory::Instance().Create<T>();
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

	void Destroy(Object* obj);

	void ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback);

	void ForeachGameObjectR(ForeachGoCallbackR callback);

	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback);

	void ForeachGameObject(ForeachGoCallback callback);
};

#endif