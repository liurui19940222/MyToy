#include "GameObject.h"
#include "DynamicFactory.h"
#include "Application.h"

CGameObject::CGameObject(string name) : Object(name)
{
	SetPosition(Vector3(0, 0, 0));
	SetLocalScale(Vector3(1, 1, 1));
	SetLocalPosition(Vector3(0, 0, 0));
	SetLocalEulerAngles(Vector3(0, 0, 0));
	parent = NULL;
	right = Vector3::Right();
	up = Vector3::Up();
	forward = Vector3::Forward();
}

CGameObject::~CGameObject() { }

void CGameObject::SetPosition(Vector3 pos)
{
	position = pos;

	moveMat = Matrix4x4(
		1, 0, 0, position.x,
		0, 1, 0, position.y,
		0, 0, 1, position.z,
		0, 0, 0, 1
	);

	if (childs.size() > 0)
	{
		for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			(*it)->UpdatePosition();
		}
	}

	ComputeModelToWorldMat();
}

void CGameObject::SetLocalScale(Vector3 s)
{
	this->localScale = s;

	localScaleMat = Matrix4x4(
		localScale.x, 0, 0, 0,
		0, localScale.y, 0, 0,
		0, 0, localScale.z, 0,
		0, 0, 0, 1
	);

	ComputeModelToWorldMat();
}

void CGameObject::SetLocalPosition(Vector3 pos)
{
	localPosition = pos;

	localMoveMat = Matrix4x4(
		1, 0, 0, localPosition.x,
		0, 1, 0, localPosition.y,
		0, 0, 1, localPosition.z,
		0, 0, 0, 1
	);

	ComputeModelToWorldMat();
}

void CGameObject::SetLocalEulerAngles(Vector3 euler)
{
	euler.x = fmod(euler.x, 360.0f);
	euler.y = fmod(euler.y, 360.0f);
	euler.z = fmod(euler.z, 360.0f);
	this->localEulerAngles = euler;
	this->localRotateMat = Matrix4x4::Rotate(euler.x, euler.y, euler.z);
	this->forward = Vector3::Forward() * localRotateMat.Transpose();
	this->right = Vector3::Cross(this->forward, Vector3(0, 1, 0));
	this->up = Vector3::Cross(this->right, this->forward);
	ComputeModelToWorldMat();
}

Vector3 CGameObject::GetLocalScale()
{
	return localScale;
}

Vector3 CGameObject::GetLocalPosition()
{
	return localPosition;
}

Vector3 CGameObject::GetLocalEulerAngles()
{
	return localEulerAngles;
}

Vector3 CGameObject::GetPosition()
{
	return position;
}

Vector3 CGameObject::GetRealPosition()
{
	return realPosition;
}

Vector3 CGameObject::GetUp()
{
	return this->up;
}

Vector3 CGameObject::GetRight()
{
	return this->right;
}

Vector3 CGameObject::GetForward()
{
	return this->forward;
}

CGameObject* CGameObject::GetParent()
{
	return this->parent;
}

Matrix4x4 CGameObject::ComputeModelToWorldMat()
{
	if (!parent)
		modelToWorldMat = moveMat * localRotateMat * localScaleMat;
	else
		modelToWorldMat = parent->GetModelToWorldMat() * localRotateMat * localScaleMat * localMoveMat;
	realPosition.x = modelToWorldMat[0][3];
	realPosition.x = modelToWorldMat[1][3];
	realPosition.x = modelToWorldMat[2][3];
	return modelToWorldMat;
}

Matrix4x4 CGameObject::GetModelToWorldMat()
{
	return modelToWorldMat;
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

void CGameObject::BeginRender()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glMultMatrixf((float*)&GetModelToWorldMat());
}

void CGameObject::EndRender()
{
	glPopMatrix();
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
	Engine->RemoveGameObject(child);
	childs.push_back(child);
}

void CGameObject::RemoveChild(CGameObject* child)
{
	for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == child)
		{
			(*it)->parent = NULL;
			childs.erase(it);
			Engine->AddGameObject(*it);
			return;
		}
	}
}

void CGameObject::RemoveAllChilds()
{
	for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->parent = NULL;
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
