#include "Object.h"


Object::Object()
{
	static int id;
	instanceId = ++id;
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