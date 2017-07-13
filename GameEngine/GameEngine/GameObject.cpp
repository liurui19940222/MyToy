#include "GameObject.h"
#include "DynamicFactory.h"
#include "Application.h"

CGameObject::CGameObject(string name) : Object(name)
{
	SetPosition(Vector3(0, 0, 0));
	SetEulerAngles(Vector3(0, 0, 0));
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
}

void CGameObject::SetEulerAngles(Vector3 euler)
{
	SetEulerAngles(euler, true);
}

void CGameObject::SetEulerAngles(Vector3 euler, bool updateLocal)
{
	this->eulerAngles.x = fmod(euler.x, 360.0f);
	this->eulerAngles.y = fmod(euler.y, 360.0f);
	this->eulerAngles.z = fmod(euler.z, 360.0f);

	float thetaX = CMath::DegToRad(this->eulerAngles.x);
	float thetaY = CMath::DegToRad(this->eulerAngles.y);
	float thetaZ = CMath::DegToRad(this->eulerAngles.z);

	rotMat = Matrix4x4::Rotate(euler.x, euler.y, euler.z);

	if (updateLocal)
	{
		if (parent)
		{
			Matrix4x4 localMat = parent->rotMat.Transpose() * rotMat;
			this->localEulerAngles = localMat.EulerAngles();
		}
		else
		{
			this->localEulerAngles = euler;
		}
	}

	if (childs.size() > 0)
	{
		for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			(*it)->UpdateEulerAngles();
		}
	}

	this->forward = Vector3::Forward() * rotMat.Transpose();
	this->right = Vector3::Cross(this->forward, Vector3(0, 1, 0));
	this->up = Vector3::Cross(this->right, this->forward);

	GetModelToWorldMat();
}

void CGameObject::SetLocalScale(Vector3 s)
{
	if (parent)
	{
		this->scale.x = parent->scale.x * s.x;
		this->scale.y = parent->scale.y * s.y;
		this->scale.z = parent->scale.z * s.z;
		this->localScale = s;
	}
	else
	{
		this->scale = this->localScale = s;
	}

	scaleMat = Matrix4x4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	);

	if (childs.size() > 0)
	{
		for (vector<CGameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			(*it)->UpdateScale();
		}
	}
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

	GetModelToWorldMat();
}

void CGameObject::SetLocalEulerAngles(Vector3 euler)
{
	localEulerAngles = euler;
	if (parent)
	{
		Matrix4x4 rot = Matrix4x4::Rotate(euler.x, euler.y, euler.z);
		Matrix4x4 worldMat = parent->rotMat * rot;
		Vector3 e = worldMat.EulerAngles();
		SetEulerAngles(worldMat.EulerAngles(), false);
	}
	else
	{
		SetEulerAngles(euler, false);
	}
	GetModelToWorldMat();
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

Vector3 CGameObject::GetEulerAngles()
{
	return this->eulerAngles;
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

Matrix4x4 CGameObject::GetModelToWorldMat()
{
	modelToWorldMat = (moveMat * rotMat * scaleMat  * localMoveMat).Transpose();
	realPosition.x = modelToWorldMat.Get(3, 0);
	realPosition.y = modelToWorldMat.Get(3, 1);
	realPosition.z = modelToWorldMat.Get(3, 2);
	return modelToWorldMat;
}

void CGameObject::LookAt(Vector3 targetPos)
{
	this->forward = (targetPos - this->position).Normalization();
	this->right = Vector3::Cross(this->forward, Vector3(0, 1, 0)).Normalization();
	this->up = Vector3::Cross(this->right, this->forward);
	rotMat = Matrix4x4(
		right.x, right.y, right.z, 0,
		this->up.x, this->up.y, this->up.z, 0,
		this->forward.x, this->forward.y, this->forward.z, 0,
		0, 0, 0, 1
	);
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
	Matrix4x4 d = this->GetModelToWorldMat();
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

void CGameObject::UpdateScale()
{
	if (parent)
		SetLocalScale(this->localScale);
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
