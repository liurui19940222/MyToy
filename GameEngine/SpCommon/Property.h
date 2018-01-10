#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include<functional>
#include"..\SpCommon\ApiDefine.h"

using namespace std;

#define setter(variableType)[&](variableType value) 
#define getter(variableType)[&]()
#define _prop(type, set, get) property<type>(setter(type) {\
	set\
}, getter(type) {\
	get\
})\

#define _prop_r(type, get) property_r<type>(getter(type) { get })
#define _prop_w(type, set) property_w<type>(setter(type) { set })

template<typename T> class ENGINE_API_TEMPLATE property
{
	typedef function<void(T)> Setter;
	typedef function<T()> Getter;

public:
	property(Setter setter, Getter getter) : m_setter(setter), m_getter(getter) {}

	property& operator=(const property& pro)
	{
		m_setter(pro.m_getter());
		return *this;
	}

	property& operator=(const T& v)
	{
		m_setter(v);
		return *this;
	}

	operator T() const
	{
		return m_getter();
	}

	const T& value() const
	{
		return m_getter();
	}

private:
	Setter m_setter;
	Getter m_getter;
};

template<typename T> class ENGINE_API_TEMPLATE property_r
{
	typedef function<T()> Getter;

public:
	property_r(Getter getter) : m_getter(getter) {}

	operator T() const
	{
		return m_getter();
	}

	const T& value() const
	{
		return m_getter();
	}

private:
	Getter m_getter;
};

template<typename T> class ENGINE_API_TEMPLATE property_w
{
	typedef function<void(T)> Setter;

public:
	property_w(Setter setter) : m_setter(setter) {}

	property_w& operator=(const property_w& pro)
	{
		m_setter(pro.m_setter());
		return *this;
	}

	property_w& operator=(const T& v)
	{
		m_setter(v);
		return *this;
	}

private:
	Setter m_setter;
};

#endif