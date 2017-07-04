#include "Object.h"


Object::Object()
{
	static int id;
	instanceId = ++id;
}

Object::Object(std::string& name) : Object()
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

void Object::SetName(string& name)
{
	this->name = name;
}

string& Object::GetName()
{
	return name;
}