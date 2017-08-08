#ifndef _OBJECT_
#define _OBJECT_

#include<string>

using namespace std;

class Object
{
protected:
	int instanceId;
	string name;

public:
	Object();

	Object(std::string& name);

	~Object();

	int GetInstanceId();

	virtual void OnInitialize();

	virtual void OnRelease();

	void SetName(const string& name);

	string& GetName();
};

#endif