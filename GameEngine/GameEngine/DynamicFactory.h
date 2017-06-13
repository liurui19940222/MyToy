#ifndef __DYNAMIC_FACTORY_H__
#define __DYNAMIC_FACTORY_H__

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <typeinfo>

// 动态对象基类
class DynamicObject
{
public:
	DynamicObject() {}
	virtual ~DynamicObject() {}
};

// 动态对象创建工厂
class DynamicFactory
{
public:

	typedef DynamicObject* (*CreateFunction)();

	static DynamicFactory & Instance()
	{
		static DynamicFactory fac;
		return fac;
	}

	bool Regist(const char * name, CreateFunction func)
	{
		if (!func)
		{
			return false;
		}
		std::string type_name = name;
		return _create_function_map.insert(std::make_pair(type_name, func)).second;
	}

	DynamicObject * Create(const std::string & type_name)
	{
		if (type_name.empty())
		{
			return NULL;
		}

		std::map<std::string, CreateFunction>::iterator it = _create_function_map.find(type_name);
		if (it == _create_function_map.end())
		{
			return NULL;
		}

		return it->second();
	}

	template<typename T>
	T * Create()
	{
		DynamicObject * obj = Create(typeid(T).name());
		if (!obj)
		{
			return NULL;
		}
		T * real_obj = dynamic_cast<T*>(obj);
		if (!real_obj)
		{
			delete obj;
			return NULL;
		}
		return real_obj;
	}

public:

	std::map<std::string, CreateFunction> _create_function_map;
};

// 动态对象创建器
template<typename T>
class DynamicCreate : public DynamicObject
{
public:
	static DynamicObject * CreateObject()
	{
		return new T();
	}

	struct Registor
	{
		Registor()
		{
			if (!DynamicFactory::Instance().Regist(typeid(T).name(), CreateObject))
			{
				assert(false);
			}
		}

		inline void do_nothing()const { }
	};

	static Registor s_registor;

public:
	DynamicCreate()
	{
		s_registor.do_nothing();
	}

	virtual ~DynamicCreate()
	{
		s_registor.do_nothing();
	}
};

template <typename T> 
typename DynamicCreate<T>::Registor DynamicCreate<T>::s_registor;

#endif