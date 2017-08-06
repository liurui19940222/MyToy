#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include<Windows.h>

template<typename T>
class CSingleton
{
	static T* m_instance;

protected:
	CSingleton() {}
	virtual ~CSingleton() {}

public:
	static T* GetInstance()
	{
		if (m_instance == NULL)
			m_instance = new T();
		return m_instance;
	}
};

template<typename T>T* CSingleton<T>::m_instance;

#endif