#ifndef _OBJECT_
#define _OBJECT_

#include<string>

using namespace std;

class Object
{
protected:
	int instanceId;
	wstring name;

public:
	Object();

	Object(std::wstring& name);

	~Object();

	int GetInstanceId();

	virtual void OnInitialize();

	virtual void OnRelease();

	void SetName(const wstring& name);

	wstring& GetName();
};

#endif