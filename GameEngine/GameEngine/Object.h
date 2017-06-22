#ifndef _OBJECT_
#define _OBJECT_

#include<string>

class Object
{
protected:
	int instanceId;
	std::string name;

public:
	Object();

	Object(std::string name);

	~Object();

	int GetInstanceId();

	virtual void OnInitialize();

	virtual void OnRelease();
};

#endif