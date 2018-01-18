#ifndef _OBJECT_
#define _OBJECT_

#include<string>

using namespace std;

class Object
{
protected:
	int m_InstanceId;
	wstring m_Name;

public:
	inline Object()
	{
		static int id = 0;
		m_InstanceId = ++id;
		OnInitialize();
	}

	inline Object(std::wstring& name) : Object()
	{
		this->m_Name = name;
	}

	virtual ~Object() { }

	inline int GetInstanceId() { return m_InstanceId; }

	inline void SetName(const wstring& name) { this->m_Name = name; }

	inline wstring& GetName() { return this->m_Name; }

	virtual void OnInitialize() { }
};

#endif