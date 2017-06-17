#ifndef _CGAMEOBJECT_
#define _CGAMEOBJECT_

#include<vector>
#include<map>
#include"GLHead.h"
#include"Object.h"
#include"Math.h"
#include "Component.h"

using namespace std;

class CGameObject : Object
{
protected:
	CGameObject();
	~CGameObject();

	vector<CComponent*> components;
	vector<CGameObject*> childs;
	CGameObject* parent;

	int instanceId;
	Vector3 position;
	Vector3 eulerAngles;
	Vector3 scale;
	Vector3 localScale;
	Vector3 localPosition;
	Vector3 localEulerAngles;
	Vector3 right;
	Vector3 up;
	Vector3 forward;

	Matrix4x4 scaleMat;
	Matrix4x4 moveMat;
	Matrix4x4 localMoveMat;
	Matrix4x4 rotMat;

	void UpdatePosition();
	void UpdateEulerAngles();
	void UpdateScale();
	void SetEulerAngles(Vector3 euler, bool updateLocal);
	void AddChild(CGameObject* child);
	void RemoveChild(CGameObject* child);
	void RemoveAllChilds();

public:
	friend class CEngine;

	void SetPosition(Vector3 pos);
	Vector3 GetPosition();
	void SetEulerAngles(Vector3 euler);
	Vector3 GetEulerAngles();
	void SetLocalScale(Vector3 scale);
	Vector3 GetLocalScale();
	void SetLocalPosition(Vector3 pos);
	Vector3 GetLocalPosition();
	void SetLocalEulerAngles(Vector3 euler);
	Vector3 GetLocalEulerAngles();
	void LookAt(Vector3 targetPos);
	void SetParent(CGameObject* parent);
	Vector3 GetUp();
	Vector3 GetRight();
	Vector3 GetForward();
	Matrix4x4 GetModelToWorldMat();

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDrawDebug();
	virtual void OnDestroy();

	template<typename T>
	T* AddComponent()
	{
		CComponent* t = DynamicFactory::Instance().Create<T>();
		components.push_back(t);
		t->gameObject = this;
		t->OnStart();
		return (T*)t;
	}

	template<typename T>
	T* GetComponent()
	{
		vector<CComponent*>::iterator it = components.begin();
		while (it != components.end())
		{
			if (typeid(**it) == typeid(T))
				return static_cast<T*>(*it);
			++it;
		}
		return NULL;
	}

	template<typename T>
	bool DestroyComponent()
	{
		vector<CComponent*>::iterator it = components.begin();
		while (it != components.end())
		{
			if (typeid(**it) == typeid(T))
			{
				components.erase(it);
				return true;
			}
			++it;
		}
		return false;
	}
};

#endif