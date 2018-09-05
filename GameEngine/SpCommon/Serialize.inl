#include "Serialize.h"

template<typename T>
string SerilizeHelper::Serilize(T* obj)
{
	rapidjson::Document document;
	document.SetObject();

	SerilizeAll(obj, document, document.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<StringBuffer> writer(buffer);
	document.Accept(writer);
	return buffer.GetString();
}

template<typename T>
void SerilizeHelper::Deserilize(T* obj, const string& json)
{
	rapidjson::Document document;
	document.Parse(json.c_str());

	DeserilizeAll(obj, document);
}

template<typename T>
void SerilizeHelper::DeserilizeAll(T* obj, Value& value)
{
	((SerializableObject*)obj)->OnDeserialize(0, obj->metadata(), value);
}

template<typename T>
void SerilizeHelper::SerilizeAll(T* obj, Value& value, MemoryPoolAllocator<>& allocator)
{
	((SerializableObject*)obj)->OnSerialize(0, obj->metadata(), value, allocator);
}

template<typename T>
void SerilizeHelper::DeserFromJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, function<void(T* tempValue, Value& jsonValue)> func)
{
	if (!member[fieldName.c_str()].IsArray())
		return;
	auto& v = member[fieldName.c_str()].GetArray();
	prop.Resize(obj, v.Size());
	T temp;
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)
	{
		func(&temp, v[i]);
		prop.SetAt(obj, i, &temp);
	}
}

template<typename T>
void SerilizeHelper::SerToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator)
{
	member[fieldName.c_str()].SetArray();
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)
	{
		Value v(kObjectType);
		RapidJsonHelper::SetValue(v, (prop.GetAt<T>(obj, i)));
		member[fieldName.c_str()].PushBack(v, allocator);
	}
}

inline void SerilizeHelper::CheckValueMember(rapidjson::Value& value, MemoryPoolAllocator<>& allocator, const char* name)
{
	if (!value.HasMember(name))
		value.AddMember(Value(name, allocator), "", allocator);
}