#ifndef _CGAMEOBJECT_
#define _CGAMEOBJECT_

#include<vector>
#include<map>
#include"GLHead.h"
#include"Object.h"
#include"Math.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"Renderer.h"

using namespace std;

class CGameObject : public Object
{
	REFLECT_CLASS(CGameObject)
protected:
	CGameObject();
	CGameObject(string name);
	~CGameObject();

	vector<CComponent*> components;
	vector<CGameObject*> childs;
	CGameObject* parent;

	int instanceId;
	Vector3 localScale;
	Vector3 localPosition;
	Vector3 localEulerAngles;
	Vector3 right;
	Vector3 up;
	Vector3 forward;

	Matrix4x4 localScaleMat;
	Matrix4x4 localMoveMat;
	Matrix4x4 localRotateMat;
	Matrix4x4 modelToWorldMat;

	void UpdateEulerAngles();
	void AddChild(CGameObject* child);
	void RemoveChild(CGameObject* child);
	void RemoveAllChilds();

public:
	friend class CEngine;
	friend class CMaker;

	void SetLocalScale(const Vector3& scale);
	const Vector3& GetLocalScale() const;
	void SetLocalPosition(const Vector3& pos);
	const Vector3& GetLocalPosition() const;
	void SetLocalEulerAngles(const Vector3& euler);
	const Vector3& GetLocalEulerAngles() const;
	void SetParent(CGameObject* parent);
	const Vector3& GetRealPosition() const;
	const Vector3& GetUp() const;
	const Vector3& GetRight() const;
	const Vector3& GetForward() const;
	CGameObject* GetParent() const;
	IRenderer* GetRenderer();
	Matrix4x4 ComputeModelToWorldMat();
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
			if (IS_TYPE(T, *it))
				return dynamic_cast<T*>(*it);
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
			if (IS_TYPE(T, *it))
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