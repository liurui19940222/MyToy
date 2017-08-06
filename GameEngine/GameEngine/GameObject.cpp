#include "GameObject.h"
#include "DynamicFactory.h"
#include "Application.h"

IMPL_CLASS(CGameObject)

CGameObject::CGameObject() : CGameObject("NewGameObject")
{
	
}

CGameObject::CGameObject(string name) : Object(name)
{
	SetPosition(Vector3(0, 0, 0));
	SetLocalScale(Vector3(1, 1, 1));
	SetLocalPosition(Vector3(0, 0, 0));
	SetLocalEulerAngles(Vector3(0, 0, 0));
	parent = NULL;
	right = Vector3::right;
	up = Vector3::up;
	forward = Vector3::forward;
}

CGameObject::~CGameObject() { }

void CGameObject::SetPosition(const Vector3& pos)
{
	position = pos;

	moveMat.MakeTranslate(pos);

	if (childs.size() > 0)
	{
		for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			(*it)->UpdatePosition();
		}
	}

	ComputeModelToWorldMat();
	//SetLocalPosition(pos);
}

void CGameObject::SetLocalScale(const Vector3& s)
{
	this->localScale = s;

	localScaleMat.MakeScale(s);

	ComputeModelToWorldMat();
}

void CGameObject::SetLocalPosition(const Vector3& pos)
{
	localPosition = pos;

	localMoveMat.MakeTranslate(pos);

	ComputeModelToWorldMat();
}

void CGameObject::SetLocalEulerAngles(const Vector3& euler)
{
	localEulerAngles.x = fmod(euler.x, 360.0f);
	localEulerAngles.y = fmod(euler.y, 360.0f);
	localEulerAngles.z = fmod(euler.z, 360.0f);

	localRotateMat.MakeRotate(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
	Matrix4x4::GetUVN(localRotateMat, &right, &up, &forward);
	ComputeModelToWorldMat();
}

const Vector3& CGameObject::GetLocalScale() const
{
	return localScale;
}

const Vector3& CGameObject::GetLocalPosition() const
{
	return localPosition;
}

const Vector3& CGameObject::GetLocalEulerAngles() const
{
	return localEulerAngles;
}

const Vector3& CGameObject::GetPosition() const
{
	return position;
}

const Vector3& CGameObject::GetRealPosition() const
{
	return realPosition;
}

const Vector3& CGameObject::GetUp() const
{
	return this->up;
}

const Vector3& CGameObject::GetRight() const
{
	return this->right;
}

const Vector3& CGameObject::GetForward() const
{
	return this->forward;
}

CGameObject* CGameObject::GetParent() const
{
	return this->parent;
}

Matrix4x4 CGameObject::ComputeModelToWorldMat()
{
	if (!parent)
		modelToWorldMat = moveMat * localRotateMat * localScaleMat;
	else
		modelToWorldMat = parent->GetModelToWorldMat()  * localRotateMat * localScaleMat* localMoveMat;
	realPosition.x = modelToWorldMat[0][3];
	realPosition.x = modelToWorldMat[1][3];
	realPosition.x = modelToWorldMat[2][3];
	return modelToWorldMat;
}

Matrix4x4 CGameObject::GetModelToWorldMat()
{
	return ComputeModelToWorldMat();
}

void CGameObject::OnStart()
{
	vector<CComponent*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it++)->OnStart();
	}
}

void CGameObject::OnUpdate()
{
	vector<CComponent*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it++)->OnUpdate();
	}
}

void CGameObject::OnRender()
{
	vector<CComponent*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it++)->OnRender();
	}
}

void CGameObject::OnDrawDebug()
{
	vector<CComponent*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it++)->OnDrawDebug();
	}
}

void CGameObject::OnDestroy()
{
	vector<CComponent*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it++)->OnDestroy();
	}
}

void CGameObject::UpdatePosition()
{
	if (parent)
	{
		SetPosition(parent->GetPosition());
	}
}

void CGameObject::UpdateEulerAngles()
{
	if (parent)
	{
		SetLocalEulerAngles(localEulerAngles);
	}
}

void CGameObject::AddChild(CGameObject* child)
{
	child->parent = this;
	for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == child)
		{
			return;
		}
	}
	Maker->RemoveGameObject(child);
	childs.push_back(child);
}

void CGameObject::RemoveChild(CGameObject* child)
{
	for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == child)
		{
			Maker->AddGameObject(*it);
			(*it)->parent = NULL;
			childs.erase(it);
			return;
		}
	}
}

void CGameObject::RemoveAllChilds()
{
	for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->parent = NULL;
		Maker->AddGameObject(*it);
	}
	childs.clear();
}

void CGameObject::SetParent(CGameObject* parent)
{
	if (this->parent)
	{
		this->parent->RemoveChild(this);
	}
	if (parent)
	{
		parent->AddChild(this);
		SetPosition(parent->position);
		SetLocalPosition(localPosition);
		SetLocalEulerAngles(localEulerAngles);
		SetLocalScale(localScale);
	}
	else
	{
		this->parent = NULL;
	}
}
