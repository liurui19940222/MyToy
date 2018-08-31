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

class SerializableObject {
	DECLARE_RTTI_ROOT()
	friend class SerilizeHelper;
protected:
	virtual void OnSerilize(const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator);
	virtual void OnDeserialize(const Metadata* meta, Value& value);
};

class SerilizeHelper {
private:
	static void DeserClsFromJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName);

	static void SerClsToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator);

	static void SerStrToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator);

	static void SerWStrToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator);

	inline static void CheckValueMember(rapidjson::Value& value, MemoryPoolAllocator<>& allocator, const char* name);

	template<typename T>
	static void SerilizeAll(T* obj, Value& value, MemoryPoolAllocator<>& allocator);

	template<typename T>
	static void DeserilizeAll(T* obj, Value& value);

	template<typename T>
	static void SerToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator);

	template<typename T>
	static void DeserFromJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, function<void(T* tempValue, Value& jsonValue)> func);

public:
	template<typename T>
	static string Serilize(T* obj);

	template<typename T>
	static void Deserilize(T* obj, const string& json);

	static void AsJsonMember(SerializableObject* obj, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator);

	static void FromJsonMember(SerializableObject* obj, const Metadata* meta, Value& value);
};


class RapidJsonHelper {
public:
	inline static void SetValue(Value& jsonValue, Bool value)
	{
		jsonValue.SetBool(value);
	}

	inline static void SetValue(Value& jsonValue, Char value)
	{
		jsonValue.SetInt(value);
	}

	inline static void SetValue(Value& jsonValue, Byte value)
	{
		jsonValue.SetUint(value);
	}

	inline static void SetValue(Value& jsonValue, Short value)
	{
		jsonValue.SetInt(value);
	}

	inline static void SetValue(Value& jsonValue, UShort value)
	{
		jsonValue.SetUint(value);
	}

	inline static void SetValue(Value& jsonValue, Int32 value)
	{
		jsonValue.SetInt(value);
	}

	inline static void SetValue(Value& jsonValue, UInt32 value)
	{
		jsonValue.SetUint(value);
	}

	inline static void SetValue(Value& jsonValue, Int64 value)
	{
		jsonValue.SetInt64(value);
	}

	inline static void SetValue(Value& jsonValue, UInt64 value)
	{
		jsonValue.SetUint64(value);
	}

	inline static void SetValue(Value& jsonValue, Float value)
	{
		jsonValue.SetFloat(value);
	}

	inline static void SetValue(Value& jsonValue, Double value)
	{
		jsonValue.SetDouble(value);
	}
};

#include"Serialize.inl"