#ifndef _OBJECT_
#define _OBJECT_
#include<string>
#include<memory>
#include"ApiDefine.h"
#include"Json.h"
#include"Converter.h"
using namespace std;

#define SMART_CLASS(CLASS_NAME) class CLASS_NAME;  typedef std::shared_ptr<CLASS_NAME> P##CLASS_NAME;
#define SMART_STRUCT(CLASS_NAME) struct CLASS_NAME;  typedef std::shared_ptr<CLASS_NAME> P##CLASS_NAME;

BEGIN_NAMESPACE_ENGINE

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

	inline Object(wstring& name) : Object()
	{
		this->m_Name = name;
	}

	virtual ~Object() { }

	inline int GetInstanceId() { return m_InstanceId; }

	inline void SetName(const wstring& name) { this->m_Name = name; }

	inline wstring& GetName() { return this->m_Name; }

	virtual void OnInitialize() { }

	virtual void AsJsonMember(rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		if (value.IsObject())
		{
			if (!value.HasMember("name"))
			{
				value.AddMember("name", "", allocator);
			}
			value["name"].SetString(CConverter::WStringToString(m_Name).c_str(), allocator);
		}
	}

	virtual string ToJson()
	{
		rapidjson::Document document;
		document.SetObject();
		AsJsonMember(document, document.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		return buffer.GetString();
	}

	virtual void FromJson(const string& json)
	{
		
	}
};

END_NAMESPACE_ENGINE
#endif