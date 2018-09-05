#include "Serialize.h"

IMPL_RTTI_ROOT(SerializableObject, NULL, {})

void SerializableObject::OnSerialize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
{
	SerilizeHelper::AsJsonMember(depth, this, meta, value, allocator);
}

void SerializableObject::OnDeserialize(int depth, const Metadata* meta, Value& value)
{
	SerilizeHelper::FromJsonMember(depth, this, meta, value);
}

void SerilizeHelper::DeserClsFromJsonArray(int depth, SerializableObject* obj, Property& prop, Value& member, const string& fieldName)
{
	if (RTTI::HasRTTIInfo(prop.GetTypeName()))
	{
		if (!member[fieldName.c_str()].IsArray())
			return;
		auto& v = member[fieldName.c_str()].GetArray();
		prop.Resize(obj, v.Size());
		for (UInt32 i = 0; i < prop.GetSize(obj); i++)
		{
			Value& objValue = v[i];
			SerializableObject* o = ((SerializableObject*)prop.GetAt(obj, i));
			o->OnDeserialize(depth, o->metadata(), objValue);
		}
	}
}

void SerilizeHelper::SerClsToJsonArray(int depth, SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator)
{
	if (RTTI::HasRTTIInfo(prop.GetTypeName()))
	{
		member[fieldName.c_str()].SetArray();
		for (UInt32 i = 0; i < prop.GetSize(obj); i++)
		{
			Value v(kObjectType);
			SerializableObject* o = ((SerializableObject*)prop.GetAt(obj, i));
			o->OnSerialize(depth, o->metadata(), v, allocator);
			member[fieldName.c_str()].PushBack(v, allocator);
		}
	}
}

void SerilizeHelper::SerStrToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator)
{
	member[fieldName.c_str()].SetArray();
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)
	{
		Value v(kStringType);
		v.SetString(prop.GetAt<string>(obj, i).c_str(), allocator);
		member[fieldName.c_str()].PushBack(v, allocator);
	}
}

void SerilizeHelper::SerWStrToJsonArray(SerializableObject* obj, Property& prop, Value& member, const string& fieldName, MemoryPoolAllocator<>& allocator)
{
	member[fieldName.c_str()].SetArray();
	for (UInt32 i = 0; i < prop.GetSize(obj); i++)
	{
		Value v(kStringType);
		v.SetString(CConverter::WStringToString(prop.GetAt<wstring>(obj, i)).c_str(), allocator);
		member[fieldName.c_str()].PushBack(v, allocator);
	}
}

void SerilizeHelper::AsJsonMember(int depth, SerializableObject* obj, const Metadata* meta, Value& member, MemoryPoolAllocator<>& allocator)
{
	vector<Property> props = meta->GetAllProperties();
	string fieldName;
	EType dataType;
	for each (auto prop in props)
	{
		if (!prop.HasAttitude(rtti::Serializable))
			continue;
		fieldName = prop.GetFieldName();
		dataType = prop.GetDataType();
		CheckValueMember(member, allocator, fieldName.c_str());

		//如果是数组
		if (prop.GetRepeatCount() > 0)
		{
			switch (dataType)
			{
			case rtti::EType::Bool:
				SerToJsonArray<Bool>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Char:
				SerToJsonArray<Char>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Byte:
				SerToJsonArray<Byte>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Short:
				SerToJsonArray<Short>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::UShort:
				SerToJsonArray<UShort>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Int32:
				SerToJsonArray<Int32>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::UInt32:
				SerToJsonArray<UInt32>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Int64:
				SerToJsonArray<Int64>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::UInt64:
				SerToJsonArray<UInt64>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Float:
				SerToJsonArray<Float>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Double:
				SerToJsonArray<Double>(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::Class:
			case rtti::EType::Struct:
				SerClsToJsonArray(depth + 1, obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::String:
				SerStrToJsonArray(obj, prop, member, fieldName, allocator);
				break;
			case rtti::EType::WString:
				SerWStrToJsonArray(obj, prop, member, fieldName, allocator);
				break;
			}
			continue;
		}

		//如果是类和结构体类型，递归进行序列化
		if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
		{
			prop.InitPtr(prop.GetAddress(obj));
			Value& classMember = member[fieldName.c_str()];
			classMember.SetObject();
			SerializableObject* o = ((SerializableObject*)prop.GetPtr(obj));
			o->OnSerialize(depth + 1, o->metadata(), classMember, allocator);
			continue;
		}

		//序列化基础类型成员
		switch (dataType)
		{
		case rtti::EType::Bool:
			member[fieldName.c_str()].SetBool(prop.GetValue<Bool>(obj));
			break;
		case rtti::EType::Char:
			member[fieldName.c_str()].SetInt(prop.GetValue<Char>(obj));
			break;
		case rtti::EType::Byte:
			member[fieldName.c_str()].SetUint(prop.GetValue<Byte>(obj));
			break;
		case rtti::EType::Short:
			member[fieldName.c_str()].SetInt(prop.GetValue<Short>(obj));
			break;
		case rtti::EType::UShort:
			member[fieldName.c_str()].SetUint(prop.GetValue<UShort>(obj));
			break;
		case rtti::EType::Int32:
			member[fieldName.c_str()].SetInt(prop.GetValue<Int32>(obj));
			break;
		case rtti::EType::UInt32:
			member[fieldName.c_str()].SetUint(prop.GetValue<UInt32>(obj));
			break;
		case rtti::EType::Int64:
			member[fieldName.c_str()].SetInt64(prop.GetValue<Int64>(obj));
			break;
		case rtti::EType::UInt64:
			member[fieldName.c_str()].SetUint64(prop.GetValue<UInt64>(obj));
			break;
		case rtti::EType::Float:
			member[fieldName.c_str()].SetFloat(prop.GetValue<Float>(obj));
			break;
		case rtti::EType::Double:
			member[fieldName.c_str()].SetDouble(prop.GetValue<Double>(obj));
			break;
		case rtti::EType::String:
			member[fieldName.c_str()].SetString(prop.GetValue<String>(obj).c_str(), allocator);
			break;
		case rtti::EType::WString:
			member[fieldName.c_str()].SetString(CConverter::WStringToString(prop.GetValue<WString>(obj)).c_str(), allocator);
			break;
		}
	}
}

void SerilizeHelper::FromJsonMember(int depth, SerializableObject* obj, const Metadata* meta, Value& member)
{
	vector<Property> props = meta->GetAllProperties();
	string fieldName;
	EType dataType;
	for each (auto prop in props)
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
			switch (dataType)
			{
			case rtti::EType::Bool:
				DeserFromJsonArray<Bool>(obj, prop, member, fieldName, [](Bool* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetBool(); });
				break;
			case rtti::EType::Char:
				DeserFromJsonArray<Char>(obj, prop, member, fieldName, [](Char* tempValue, Value& jsonValue) { *tempValue = (Char)jsonValue.GetInt(); });
				break;
			case rtti::EType::Byte:
				DeserFromJsonArray<Byte>(obj, prop, member, fieldName, [](Byte* tempValue, Value& jsonValue) { *tempValue = (Byte)jsonValue.GetUint(); });
				break;
			case rtti::EType::Short:
				DeserFromJsonArray<Short>(obj, prop, member, fieldName, [](Short* tempValue, Value& jsonValue) { *tempValue = (Short)jsonValue.GetInt(); });
				break;
			case rtti::EType::UShort:
				DeserFromJsonArray<UShort>(obj, prop, member, fieldName, [](UShort* tempValue, Value& jsonValue) { *tempValue = (UShort)jsonValue.GetUint(); });
				break;
			case rtti::EType::Int32:
				DeserFromJsonArray<Int32>(obj, prop, member, fieldName, [](Int32* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetInt(); });
				break;
			case rtti::EType::UInt32:
				DeserFromJsonArray<UInt32>(obj, prop, member, fieldName, [](UInt32* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetUint(); });
				break;
			case rtti::EType::Int64:
				DeserFromJsonArray<Int64>(obj, prop, member, fieldName, [](Int64* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetInt64(); });
				break;
			case rtti::EType::UInt64:
				DeserFromJsonArray<UInt64>(obj, prop, member, fieldName, [](UInt64* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetUint64(); });
				break;
			case rtti::EType::Float:
				DeserFromJsonArray<Float>(obj, prop, member, fieldName, [](Float* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetFloat(); });
				break;
			case rtti::EType::Double:
				DeserFromJsonArray<Double>(obj, prop, member, fieldName, [](Double* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetDouble(); });
				break;
			case rtti::EType::String:
				DeserFromJsonArray<String>(obj, prop, member, fieldName, [](String* tempValue, Value& jsonValue) { *tempValue = jsonValue.GetString(); });
				break;
			case rtti::EType::WString:
				DeserFromJsonArray<WString>(obj, prop, member, fieldName, [](WString* tempValue, Value& jsonValue) { *tempValue = CConverter::StringToWString(string(jsonValue.GetString())); });
				break;
			case rtti::EType::Class:
			case rtti::EType::Struct:
				DeserClsFromJsonArray(depth + 1, obj, prop, member, fieldName);
				break;
			}
			continue;
		}

		//如果是类和结构体类型，递归进行序列化
		if ((dataType == EType::Class || dataType == EType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
		{
			Value& classMember = member[fieldName.c_str()];
			prop.InitPtr(prop.GetAddress(obj));
			SerializableObject* o = ((SerializableObject*)prop.GetPtr(obj));
			o->OnDeserialize(depth + 1, o->metadata(), classMember);
			continue;
		}

		//序列化基础类型成员
		switch (dataType)
		{
		case rtti::EType::Bool:
			prop.SetValue(obj, member[fieldName.c_str()].GetBool());
			break;
		case rtti::EType::Char:
			prop.SetValue(obj, (Char)member[fieldName.c_str()].GetInt());
			break;
		case rtti::EType::Byte:
			prop.SetValue(obj, (Byte)member[fieldName.c_str()].GetUint());
			break;
		case rtti::EType::Short:
			prop.SetValue(obj, (Short)member[fieldName.c_str()].GetInt());
			break;
		case rtti::EType::UShort:
			prop.SetValue(obj, (UShort)member[fieldName.c_str()].GetUint());
			break;
		case rtti::EType::Int32:
			prop.SetValue(obj, member[fieldName.c_str()].GetInt());
			break;
		case rtti::EType::UInt32:
			prop.SetValue(obj, member[fieldName.c_str()].GetUint());
			break;
		case rtti::EType::Int64:
			prop.SetValue(obj, member[fieldName.c_str()].GetInt64());
			break;
		case rtti::EType::UInt64:
			prop.SetValue(obj, member[fieldName.c_str()].GetUint64());
			break;
		case rtti::EType::Float:
			prop.SetValue(obj, member[fieldName.c_str()].GetFloat());
			break;
		case rtti::EType::Double:
			prop.SetValue(obj, member[fieldName.c_str()].GetDouble());
			break;
		case rtti::EType::String:
			prop.SetValue(obj, string(member[fieldName.c_str()].GetString()));
			break;
		case rtti::EType::WString:
			prop.SetValue(obj, CConverter::StringToWString(string(member[fieldName.c_str()].GetString())));
			break;
		}
	}
}