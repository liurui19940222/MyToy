#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include<Windows.h>
#include"ApiDefine.h"

template<typename T>
class CSingleton
{
	static T* m_instance;

protected:
	CSingleton() {}
	virtual ~CSingleton() {}
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

template<typename T>T* CSingleton<T>::m_instance;

#endif