#include"RTTI.h"

class Color {
public:
	float r, g, b;
};

class Person {
public:
	int id;
	int age;
	int* tex = (int*)0x88ff;
	Color color = {0.0f, 0.0f, 0.0f};

private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetaData();
};

Metadata Person::_MetaData("Person", NULL, Person::Register);

void Person::Register(Metadata& meta)
{
	meta.AddProperty(Property("Int", "id", DataType::Int32, offsetof(Person, id)));
	meta.AddProperty(Property("Int", "age", DataType::Int32, offsetof(Person, age)));
}

const Metadata* Person::GetMetaData()
{
	return &_MetaData;
}

void main()
{
	Person person;
	const Metadata* meta = Person::GetMetaData();
	system("pause");
}