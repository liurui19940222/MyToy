#include "Object.h"

Object::Object()
{
	static int id = 0;
	instanceId = ++id;
}

Object::Object(std::wstring& name) : Object()
{
	this->name = name;
}

Object::~Object()
{
}

int Object::GetInstanceId()
{
	return instanceId;
}

void Object::OnRelease()
{

}

void Object::OnInitialize()
{

}

void Object::SetName(const wstring& name)
{
	this->name = name;
}

wstring& Object::GetName()
{
	return name;
}