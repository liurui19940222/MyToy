#include"RTTI.h"
#include<iostream>
#include"..\SpCommon\Debug.h"
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

#define PROCESS_ARR(Type, Func) (dataType == DataType::Type)\
{\
	Type* pointer = (Type*)prop.GetAddress(obj);\
	member[fieldName.c_str()].SetArray();\
	for (int i = 0; i < prop.GetRepeatCount(); i++)\
	{\
		Value obj(kObjectType);\
		obj.##Func##(pointer[i]);\
		member[fieldName.c_str()].PushBack(obj, allocator);\
	}\
}\

class SerilizeHelper {
public:
	inline static void CheckValueMember(Value& value, MemoryPoolAllocator<>& allocator, const char* name)
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
		DataType dataType;
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
				if PROCESS_ARR(Int32, SetInt)
				else if PROCESS_ARR(UInt32, SetUint)
				else if PROCESS_ARR(Int64, SetInt64)
				else if PROCESS_ARR(UInt64, SetUint64)
				else if PROCESS_ARR(Short, SetInt)
				else if PROCESS_ARR(UShort, SetUint)
				else if PROCESS_ARR(Byte, SetUint)
				else if PROCESS_ARR(Char, SetInt)
				else if PROCESS_ARR(Float, SetFloat)
				else if PROCESS_ARR(Double, SetDouble)
				else if PROCESS_ARR(Bool, SetBool)
				else if (dataType == DataType::String)
				{
					String* pointer = (String*)prop.GetAddress(obj);
					member[fieldName.c_str()].SetArray();
					for (int i = 0; i < prop.GetRepeatCount(); i++)
					{
						Value obj(kStringType);
						obj.SetString(pointer[i].c_str(), allocator);
						member[fieldName.c_str()].PushBack(obj, allocator);
					}
				}
				else if (dataType == DataType::WString)
				{
					WString* pointer = (WString*)prop.GetAddress(obj);
					member[fieldName.c_str()].SetArray();
					for (int i = 0; i < prop.GetRepeatCount(); i++)
					{
						Value obj(kStringType);
						obj.SetString(CConverter::WStringToString(pointer[i]).c_str(), allocator);
						member[fieldName.c_str()].PushBack(obj, allocator);
					}
				}
				else if ((dataType == DataType::Class || dataType == DataType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
				{
					char* pointer = (char*)prop.GetAddress(obj);
					member[fieldName.c_str()].SetArray();
					for (int i = 0; i < prop.GetRepeatCount(); i++)
					{
						Value obj(kObjectType);
						AsJsonMember(pointer + i * prop.m_Metadata->GetSize(), prop.m_Metadata, obj, allocator);
						member[fieldName.c_str()].PushBack(obj, allocator);
					}
				}
				continue;
			}

			//如果是类和结构体类型，递归进行序列化
			if ((dataType == DataType::Class || dataType == DataType::Struct) && RTTI::HasRTTIInfo(prop.GetTypeName()))
			{
				Value& classMember = member[fieldName.c_str()];
				classMember.SetObject();
				AsJsonMember(prop.GetAddress(obj), prop.m_Metadata, classMember, allocator);
				continue;
			}

			//序列化基础类型成员
			if (dataType == DataType::Int32)
				member[fieldName.c_str()].SetInt(prop.GetValue<Int32>(obj));
			else if (dataType == DataType::UInt32)
				member[fieldName.c_str()].SetUint(prop.GetValue<UInt32>(obj));
			else if (dataType == DataType::Int64)
				member[fieldName.c_str()].SetInt64(prop.GetValue<Int64>(obj));
			else if (dataType == DataType::UInt64)
				member[fieldName.c_str()].SetUint64(prop.GetValue<UInt64>(obj));
			else if (dataType == DataType::Short)
				member[fieldName.c_str()].SetInt(prop.GetValue<Short>(obj));
			else if (dataType == DataType::UShort)
				member[fieldName.c_str()].SetUint(prop.GetValue<UShort>(obj));
			else if (dataType == DataType::Byte)
				member[fieldName.c_str()].SetUint(prop.GetValue<Byte>(obj));
			else if (dataType == DataType::Char)
				member[fieldName.c_str()].SetInt(prop.GetValue<Char>(obj));
			else if (dataType == DataType::Float)
				member[fieldName.c_str()].SetFloat(prop.GetValue<Float>(obj));
			else if (dataType == DataType::Double)
				member[fieldName.c_str()].SetDouble(prop.GetValue<Double>(obj));
			else if (dataType == DataType::Bool)
				member[fieldName.c_str()].SetBool(prop.GetValue<Bool>(obj));
			else if (dataType == DataType::WString)
				member[fieldName.c_str()].SetString(CConverter::WStringToString(prop.GetValue<WString>(obj)).c_str(), allocator);
			else if (dataType == DataType::String)
				member[fieldName.c_str()].SetString(prop.GetValue<String>(obj).c_str(), allocator);
		}
	}

	template<typename T>
	static void AsJsonMember(T* obj, Value& value, MemoryPoolAllocator<>& allocator)
	{
		AsJsonMember(obj, obj->GetMetadata(), value, allocator);
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
	static void Deserilize(T& obj, Value& value)
	{

	}
};

#pragma region ColorB

class ColorB {
public:
	float r, g, b;

private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata ColorB::_MetaData("ColorB", sizeof(ColorB), NULL, ColorB::Register);

void ColorB::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor<ColorB>([]() { return new ColorB(); });
	meta.AddProperty(Property("", "r", DataType::Float, offsetof(ColorB, r)));
	meta.AddProperty(Property("", "g", DataType::Float, offsetof(ColorB, g)));
	meta.AddProperty(Property("", "b", DataType::Float, offsetof(ColorB, b)));
}

const Metadata* ColorB::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion

#pragma region Person

class Person {
public:
	int id;
	int age;
	float height;
	bool intelligent;
	Int64 identity;
	wstring name;
	string en_name;
	ColorB color = { 0.0f, 0.0f, 0.0f };
	double coord[5] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
	string flags[4] = { "cheers", "hello", "welcome", "happy" };
	wstring frieds[4] = { L"官全红", L"官全绿", L"官全黄", L"官全蓝" };
	ColorB colors[2] = { ColorB{ 1.0f, 1.0f, 1.0f }, ColorB{ 0.5f, 0.5f, 0.5f } };

	Person() : id(9), age(55), height(1.55f), name(L"刘睿"), en_name("Ray"), intelligent(true), identity(800) { }
private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata Person::_MetaData("Person", sizeof(Person), NULL, Person::Register);

void Person::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor<Person>([]() { return new Person(); });
	meta.AddProperty(Property("ColorB", "color", DataType::Class, offsetof(Person, color), ColorB::GetMetadata()));
	meta.AddProperty(Property("", "id", DataType::Int32, offsetof(Person, id)));
	meta.AddProperty(Property("", "age", DataType::Int32, offsetof(Person, age), rtti::OnlyRead));
	meta.AddProperty(Property("", "height", DataType::Float, offsetof(Person, height)));
	meta.AddProperty(Property("", "identity", DataType::Int64, offsetof(Person, identity)));
	meta.AddProperty(Property("", "intelligent", DataType::Bool, offsetof(Person, intelligent)));
	meta.AddProperty(Property("", "name", DataType::WString, offsetof(Person, name)));
	meta.AddProperty(Property("", "en_name", DataType::String, offsetof(Person, en_name)));
	meta.AddProperty(Property("", "coord", DataType::Double, offsetof(Person, coord), NULL, _countof(Person::coord), DEFAUTL_ATTITUDE));
	meta.AddProperty(Property("", "flags", DataType::String, offsetof(Person, flags), NULL, _countof(Person::flags), DEFAUTL_ATTITUDE));
	meta.AddProperty(Property("", "frieds", DataType::WString, offsetof(Person, frieds), NULL, _countof(Person::frieds), DEFAUTL_ATTITUDE));
	meta.AddProperty(Property("ColorB", "colors", DataType::Class, offsetof(Person, colors), ColorB::GetMetadata(), _countof(Person::colors), DEFAUTL_ATTITUDE));
}

const Metadata* Person::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion

void main()
{
	Person* p = RTTI::Instantiate<Person>("Person");

	// 反射遍历所有属性
	const vector<Property>* props = p->GetMetadata()->GetProperties();
	for each (Property prop in *props)
	{
		cout << prop.GetFieldName() << endl;
	}

	//string json = SerilizeHelper::Serilize(p);
	//cout << json.c_str() << endl;

	system("pause");
}