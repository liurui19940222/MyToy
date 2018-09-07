#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include<Windows.h>
#include"defs.h"

BEGIN_NAMESPACE_ENGINE

template<typename T>
class ENGINE_API_TEMPLATE Singleton
{
	static T* m_instance;

protected:
	Singleton() {}
	virtual ~Singleton() {}
	virtual void OnInitialize() {}

public:
	static T* GetInstance()
	{
		if (m_instance == NULL)
		{
			m_instance = new T();
			m_instance->OnInitialize();
		}
		return m_instance;
	}
};

template<typename T>T* Singleton<T>::m_instance;

END_NAMESPACE_ENGINE

#endif