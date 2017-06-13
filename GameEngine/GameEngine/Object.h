#ifndef _OBJECT_
#define _OBJECT_

class Object
{
protected:
	int instanceId;

public:
	Object();
	~Object();

	int GetInstanceId();

	virtual void OnInitialize();

	virtual void OnRelease();
};

#endif