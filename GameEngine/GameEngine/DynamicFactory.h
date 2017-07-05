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
#include <iostream>

using namespace std;

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

	typedef void* (*CreateFunction)();

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

	void * Create(const std::string & type_name)
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
		void * obj = Create(typeid(T).name());
		if (!obj)
		{
			return NULL;
		}
		T * real_obj = (T*)(obj);
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
#define REFLECT_CLASS(class_name)\
public:\
	static void * CreateObject()\
	{\
		return new class_name();\
	}\
	struct Registor\
	{\
		Registor()\
		{\
			DynamicFactory::Instance().Regist(#class_name, CreateObject);\
			DynamicFactory::Instance().Regist(typeid(##class_name##).name(), CreateObject);\
			cout << #class_name"注册成功" << endl;\
		}\
	};\
	static Registor s_registor;\
	virtual string GetClassName();\

#define IMPL_CLASS(class_name)\
class_name::Registor class_name::s_registor;\
string class_name::GetClassName() { return typeid(##class_name##).name(); }\

#define IS_TYPE(T, p) dynamic_cast<T*>(p) != NULL

#endif