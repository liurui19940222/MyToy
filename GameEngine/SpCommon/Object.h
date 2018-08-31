#ifndef _OBJECT_
#define _OBJECT_
#include<string>
#include<memory>
#include"ApiDefine.h"
#include"Converter.h"
#include"Serialize.h"
using namespace std;

#define SMART_CLASS(CLASS_NAME) class CLASS_NAME;  typedef std::shared_ptr<CLASS_NAME> P##CLASS_NAME;
#define SMART_STRUCT(CLASS_NAME) struct CLASS_NAME;  typedef std::shared_ptr<CLASS_NAME> P##CLASS_NAME;

BEGIN_NAMESPACE_ENGINE

class Object : public SerializableObject
{
	DECLARE_RTTI()
	friend class AssetUtility;
protected:
	int			m_InstanceId;
	wstring		m_Name;
	string		m_Reference;

public:
	inline Object()
	{
		static int id = 0;
		m_InstanceId = ++id;
		OnInitialize();
	}

	inline Object(wstring& name) : Object()
	{
		this->m_Name = name;
	}

	virtual ~Object() { }

	inline int GetInstanceId() { return m_InstanceId; }

	inline void SetName(const wstring& name) { this->m_Name = name; }

	inline wstring& GetName() { return this->m_Name; }

	virtual void OnInitialize() {}

protected:
	virtual void OnSerilize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator) override;

	virtual void OnDeserialize(int depth, const Metadata* meta, Value& value) override;
};

END_NAMESPACE_ENGINE
#endif