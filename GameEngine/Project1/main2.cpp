#include"..\ExternInclude\rapidjson\rapidjson.h"
#include"..\ExternInclude\rapidjson\document.h"
#include"..\ExternInclude\rapidjson\writer.h"
#include"..\ExternInclude\rapidjson\prettywriter.h"
#include"..\ExternInclude\rapidjson\stringbuffer.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace rapidjson;
using namespace std;

class Color {
public:
	float r, g, b, a;

	void AsJsonMember(Value& value, MemoryPoolAllocator<>& allocator)
	{
		value.PushBack(r, allocator);
		value.PushBack(g, allocator);
		value.PushBack(b, allocator);
		value.PushBack(a, allocator);
	}
};

class Object {
public:
	int id;

	virtual void AsJsonMember(Value& value, MemoryPoolAllocator<>& allocator)
	{
		if (!value.HasMember("id"))
		{
			value.AddMember("id", 0, allocator);
		}
		value["id"].SetInt(id);
	}

	virtual string ToJson()
	{
		rapidjson::Document document;
		document.SetObject();
		AsJsonMember(document, document.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<StringBuffer> writer(buffer);
		document.Accept(writer);
		return buffer.GetString();
	}

	virtual void FromJson()
	{

	}
};

class Material : public Object {
public:
	string name;
	Color diffuse;

	virtual void AsJsonMember(Value& value, MemoryPoolAllocator<>& allocator)
	{
		Object::AsJsonMember(value, allocator);
		if (!value.HasMember("name"))
		{
			value.AddMember("name", "", allocator);
		}
		value["name"].SetString(name.c_str(), allocator);
		if (!value.HasMember("diffuse"))
		{
			value.AddMember("diffuse", "", allocator);
		}
		value["diffuse"].SetArray();
		diffuse.AsJsonMember(value["diffuse"], allocator);
	}

	virtual string ToJson()
	{
		rapidjson::Document document;
		document.SetObject();
		AsJsonMember(document, document.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		document.Accept(writer);
		return buffer.GetString();
	}
};

int main()
{
	Material mat;
	mat.id = 1000;
	mat.name = "jake";
	mat.diffuse.r = 1.0f;
	mat.diffuse.g = 0.5f;
	mat.diffuse.b = 0.6f;
	mat.diffuse.a = 0.0f;
	string json = mat.ToJson();
	cout << json << endl;
	system("pause");
	return 0;
}