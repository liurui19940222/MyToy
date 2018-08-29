#pragma once

#include<iostream>
#include<list>
#include<algorithm>
#include"RTTI.h"
#include"..\SpCommon\Converter.h"
#include"..\ExternInclude\rapidjson\rapidjson.h"
#include"..\ExternInclude\rapidjson\document.h"
#include"..\ExternInclude\rapidjson\writer.h"
#include"..\ExternInclude\rapidjson\prettywriter.h"
#include"..\ExternInclude\rapidjson\stringbuffer.h"

using namespace rapidjson;
using namespace std;
USING_NAMESPACE_ENGINE;

#ifdef GetClassName
#undef GetClassName
#endif

#define SER_BASE_TYPE(Type, Func) (dataType == EType::Type)\
{\
	member[fieldName.c_str()].SetArray();\
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)\
	{\
		Value v(kObjectType);\
		v.##Func##(prop.GetAt<Type>(obj, i));\
		member[fieldName.c_str()].PushBack(v, allocator);\
	}\
}\

#define DESER_BASE_TYPE(Type, Func) (dataType == EType::Type)\
{\
	if (!member[fieldName.c_str()].IsArray())\
		continue;\
	auto& v = member[fieldName.c_str()].GetArray();\
	prop.Resize(obj, v.Size());\
	Type temp;\
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)\
	{\
		temp = v[i].Func();\
		prop.SetAt(obj, i, &temp);\
	}\
}\

class SerilizeHelper {
public:
	inline static void CheckValueMember(rapidjson::Value& value, MemoryPoolAllocator<>& allocator, const char* name)
	{
		if (!value.HasMember(name))
			value.AddMember(Value(name, allocator), "", allocator);
	}

	template<typename T>
	static void AsJsonMember(T* obj, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
	{
		//序列化类名
		CheckValueMember(value, allocator, "type");
		value["type"].SetString(meta->GetClassName().c_str(), allocator);

		//序列化成员变量
		CheckValueMember(value, allocator, "member");
		value["member"].SetObject();
		Value& member = value["member"];

		const vector<Property>* props = meta->GetProperties();
		string fieldName;
		EType dataType;
		for each (auto prop in *props)
		{
			if (!prop.HasAttitude(rtti::Serializable))
				continue;
			fieldName = prop.GetFieldName();
			dataType = prop.GetDataType();
			CheckValueMember(member, allocator, fieldName.c_str());

			//如果是数组
			if (prop.GetRepeatCount() > 0)
			{
				if SER_BASE_TYPE(Int32, SetInt)
				else if SER_BASE_TYPE(UInt32, SetUint)
				else if SER_BASE_TYPE(Int64, SetInt64)
				else if SER_BASE_TYPE(UInt64, SetUint64)
				else if SER_BASE_TYPE(Short, SetInt)
				else if SER_BASE_TYPE(UShort, SetUint)
				else if SER_BASE_TYPE(Byte, SetUint)
				else if SER_BASE_TYPE(Char, SetInt)
				else if SER_BASE_TYPE(Float, SetFloat)
				else if SER_BASE_TYPE(Double, SetDouble)
				else if SER_BASE_TYPE(Bool, SetBool)
				else if (dataType == EType::String)
				{
					member[fieldName.c_str()].SetArray();
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						Value v(kStringType);
						v.SetString(prop.GetAt<string>(obj, i).c_str(), allocator);
						member[fieldName.c_str()].PushBack(v, allocator);
					}
				}
				else if (dataType == EType::WString)
				{
					member[fieldName.c_str()].SetArray();
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						Value v(kStringType);
						v.SetString(CConverter::WStringToString(prop.GetAt<wstring>(obj, i)).c_str(), allocator);
						member[fieldName.c_str()].PushBack(v, allocator);
					}
				}
				else if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
				{
					member[fieldName.c_str()].SetArray();
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						Value v(kObjectType);
						AsJsonMember(prop.GetAt(obj, i), prop.m_Metadata, v, allocator);
						member[fieldName.c_str()].PushBack(v, allocator);
					}
				}
				continue;
			}

			//如果是类和结构体类型，递归进行序列化
			if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
			{
				Value& classMember = member[fieldName.c_str()];
				classMember.SetObject();
				AsJsonMember(prop.GetAddress(obj), prop.m_Metadata, classMember, allocator);
				continue;
			}

			//序列化基础类型成员
			if (dataType == EType::Int32)
				member[fieldName.c_str()].SetInt(prop.GetValue<Int32>(obj));
			else if (dataType == EType::UInt32)
				member[fieldName.c_str()].SetUint(prop.GetValue<UInt32>(obj));
			else if (dataType == EType::Int64)
				member[fieldName.c_str()].SetInt64(prop.GetValue<Int64>(obj));
			else if (dataType == EType::UInt64)
				member[fieldName.c_str()].SetUint64(prop.GetValue<UInt64>(obj));
			else if (dataType == EType::Short)
				member[fieldName.c_str()].SetInt(prop.GetValue<Short>(obj));
			else if (dataType == EType::UShort)
				member[fieldName.c_str()].SetUint(prop.GetValue<UShort>(obj));
			else if (dataType == EType::Byte)
				member[fieldName.c_str()].SetUint(prop.GetValue<Byte>(obj));
			else if (dataType == EType::Char)
				member[fieldName.c_str()].SetInt(prop.GetValue<Char>(obj));
			else if (dataType == EType::Float)
				member[fieldName.c_str()].SetFloat(prop.GetValue<Float>(obj));
			else if (dataType == EType::Double)
				member[fieldName.c_str()].SetDouble(prop.GetValue<Double>(obj));
			else if (dataType == EType::Bool)
				member[fieldName.c_str()].SetBool(prop.GetValue<Bool>(obj));
			else if (dataType == EType::WString)
				member[fieldName.c_str()].SetString(CConverter::WStringToString(prop.GetValue<WString>(obj)).c_str(), allocator);
			else if (dataType == EType::String)
				member[fieldName.c_str()].SetString(prop.GetValue<String>(obj).c_str(), allocator);
		}
	}

	template<typename T>
	static void AsJsonMember(T* obj, Value& value, MemoryPoolAllocator<>& allocator)
	{
		list<const Metadata*> metas;
		const Metadata* meta = obj->GetMetadata();
		while (meta)
		{
			metas.push_front(meta);
			meta = meta->GetParentMetaData();
		}

		std::for_each(metas.begin(), metas.end(), [&](const Metadata* m) {
			CheckValueMember(value, allocator, m->GetClassName().c_str());
			value[m->GetClassName().c_str()].SetObject();
			Value& member = value[m->GetClassName().c_str()];
			AsJsonMember(obj, m, member, allocator);
		});
	}

	template<typename T>
	static void FromJsonMember(T* obj, const Metadata* meta, Value& value)
	{
		//反序列化成员变量
		if (!value.HasMember("member"))
			return;

		const vector<Property>* props = meta->GetProperties();
		string fieldName;
		EType dataType;
		Value& member = value["member"];
		for each (auto prop in *props)
		{
			if (!prop.HasAttitude(rtti::Serializable))
				continue;

			fieldName = prop.GetFieldName();
			if (!member.HasMember(fieldName.c_str()))
				continue;

			dataType = prop.GetDataType();

			//如果是数组
			if (prop.GetRepeatCount() > 0)
			{
				//if (dataType == EType::Float)
				//{
				//	if (!member[fieldName.c_str()].IsArray())
				//		continue;
				//	auto& v = member[fieldName.c_str()].GetArray();
				//	prop.Resize(obj, v.Size());
				//	Float temp;
				//	for (UInt32 i = 0; i < prop.GetSize(obj); i++)
				//	{
				//		temp = v[i].GetFloat();
				//		prop.SetAt(obj, i, &temp);
				//	}
				//}
				if DESER_BASE_TYPE(Int32, GetInt)
				else if DESER_BASE_TYPE(UInt32, GetUint)
				else if DESER_BASE_TYPE(Int64, GetInt64)
				else if DESER_BASE_TYPE(UInt64, GetUint64)
				else if DESER_BASE_TYPE(Short, GetInt)
				else if DESER_BASE_TYPE(UShort, GetUint)
				else if DESER_BASE_TYPE(Byte, GetUint)
				else if DESER_BASE_TYPE(Char, GetInt)
				else if DESER_BASE_TYPE(Float, GetFloat)
				else if DESER_BASE_TYPE(Double, GetDouble)
				else if DESER_BASE_TYPE(Bool, GetBool)
				else if (dataType == EType::String)
				{
					if (!member[fieldName.c_str()].IsArray())
						continue;
					auto& v = member[fieldName.c_str()].GetArray();
					prop.Resize(obj, v.Size());
					string temp;
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						temp = v[i].GetString();
						prop.SetAt(obj, i, &temp);
					}
				}
				else if (dataType == EType::WString)
				{
					if (!member[fieldName.c_str()].IsArray())
						continue;
					auto& v = member[fieldName.c_str()].GetArray();
					prop.Resize(obj, v.Size());
					wstring temp;
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						temp = CConverter::StringToWString(string(v[i].GetString()));
						prop.SetAt(obj, i, &temp);
					}
				}
				else if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
				{
					if (!member[fieldName.c_str()].IsArray())
						continue;
					auto& v = member[fieldName.c_str()].GetArray();
					prop.Resize(obj, v.Size());
					UInt32 size = prop.GetSize(obj);
					for (UInt32 i = 0; i < prop.GetSize(obj); i++)
					{
						Value& objValue = v[i];
						FromJsonMember(prop.GetAt(obj, i), prop.m_Metadata, objValue);
					}
				}
				continue;
			}

			//如果是类和结构体类型，递归进行序列化
			if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
			{
				Value& classMember = member[fieldName.c_str()];
				FromJsonMember(prop.GetAddress(obj), prop.m_Metadata, classMember);
				continue;
			}

			//序列化基础类型成员
			if (dataType == EType::Int32)
				prop.SetValue(obj, member[fieldName.c_str()].GetInt());
			else if (dataType == EType::UInt32)
				prop.SetValue(obj, member[fieldName.c_str()].GetUint());
			else if (dataType == EType::Int64)
				prop.SetValue(obj, member[fieldName.c_str()].GetInt64());
			else if (dataType == EType::UInt64)
				prop.SetValue(obj, member[fieldName.c_str()].GetUint64());
			else if (dataType == EType::Short)
				prop.SetValue(obj, (Short)member[fieldName.c_str()].GetInt());
			else if (dataType == EType::UShort)
				prop.SetValue(obj, (UShort)member[fieldName.c_str()].GetUint());
			else if (dataType == EType::Byte)
				prop.SetValue(obj, (Byte)member[fieldName.c_str()].GetUint());
			else if (dataType == EType::Char)
				prop.SetValue(obj, (Char)member[fieldName.c_str()].GetInt());
			else if (dataType == EType::Float)
				prop.SetValue(obj, member[fieldName.c_str()].GetFloat());
			else if (dataType == EType::Double)
				prop.SetValue(obj, member[fieldName.c_str()].GetDouble());
			else if (dataType == EType::Bool)
				prop.SetValue(obj, member[fieldName.c_str()].GetBool());
			else if (dataType == EType::WString)
				prop.SetValue(obj, CConverter::StringToWString(string(member[fieldName.c_str()].GetString())));
			else if (dataType == EType::String)
				prop.SetValue(obj, string(member[fieldName.c_str()].GetString()));
		}
	}

	template<typename T>
	static void FromJsonMember(T* obj, Value& value)
	{
		list<const Metadata*> metas;
		const Metadata* meta = obj->GetMetadata();
		while (meta)
		{
			metas.push_front(meta);
			meta = meta->GetParentMetaData();
		}

		std::for_each(metas.begin(), metas.end(), [&](const Metadata* m) {
			if (value.HasMember(m->GetClassName().c_str()))
			{
				FromJsonMember(obj, m, value[m->GetClassName().c_str()]);
			}
		});
	}

	template<typename T>
	static string Serilize(T* obj)
	{
		rapidjson::Document document;
		document.SetObject();

		AsJsonMember(obj, document, document.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		document.Accept(writer);
		return buffer.GetString();
	}

	template<typename T>
	static void Deserilize(T* obj, const string& json)
	{
		rapidjson::Document document;
		document.Parse(json.c_str());

		FromJsonMember(obj, document);
	}
};