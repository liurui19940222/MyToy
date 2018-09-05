#pragma once

#include<string>
#include<vector>
#include<map>
#include<functional>

#ifdef GetClassName
#undef GetClassName
#endif

#define DEFAUTL_ATTITUDE  (ReadAndWrite | Serializable | Editable)

using namespace std;

namespace rtti {
	typedef bool				Bool;
	typedef char				Char;
	typedef unsigned char		Byte;
	typedef short				Short;
	typedef unsigned short		UShort;
	typedef int					Int32;
	typedef unsigned int		UInt32;
	typedef long long			Int64;
	typedef unsigned long long	UInt64;
	typedef float				Float;
	typedef double				Double;
	typedef string				String;
	typedef wstring				WString;

	class Metadata;
	class Property;
	typedef function<void(Metadata&)>	StaticInitHandler;
	typedef function<void*()>			ClassCreateHandler;
	typedef function<void(Property& prop, void* obj, UInt32 pos, void* value)>	SetAtFunc;
	typedef function<void*(Property& prop, void* obj, UInt32 pos)>				GetAtFunc;
	typedef function<UInt32(Property& prop, void* obj)>							GetSizeFunc;
	typedef function<void(Property& prop, void* obj, UInt32 size)>				ResizeFunc;
	typedef function<void(Property& prop, void* obj)>							InitPtrFunc;
	typedef function<void*(Property& prop, void* obj)>							GetPtrFunc;

	enum class EType {
		Bool,
		Char,
		Byte,
		Short,
		UShort,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float,
		Double,
		Class,
		Struct,
		String,
		WString,
		UnkownType,
	};

	enum PropertyAttitude {
		OnlyRead = 1 << 0,
		OnlyWrite = 1 << 1,
		ReadAndWrite = 1 << 2,
		Serializable = 1 << 3,
		Editable = 1 << 4,
		Pointer = 1 << 5,
		SharedPtr = 1 << 6,
	};

	class Property
	{
	private:
		UInt32		m_OffsetAddr;
		UInt32		m_Attitude;
		UInt32		m_RepeatCount;	//����0ʱΪ����, vector
		EType		m_DataType;

		string		m_FieldName;
		string		m_TypeName;
		SetAtFunc	m_SetAtFunc;
		GetAtFunc	m_GetAtFunc;
		GetSizeFunc m_GetSizeFunc;
		ResizeFunc	m_ResizeFunc;
		InitPtrFunc	m_InitPtrFunc;
		GetPtrFunc  m_GetPtrFunc;

	public:
		const Metadata*	m_Metadata;

		Property();
		Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, UInt32 attitude = DEFAUTL_ATTITUDE);
		Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, const Metadata*	metadata, UInt32 attitude = DEFAUTL_ATTITUDE);
		Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, const Metadata*	metadata, UInt32 repeatCount, UInt32 attitude = DEFAUTL_ATTITUDE);

		template<typename T> T		GetValue(void* obj) const;
		template<typename T> void	SetValue(void* obj, T value);
		void*						GetAddress(void* obj) const;
		bool		HasAttitude(UInt32 atti) const;
		void		AddAttitude(UInt32 atti);
		void		SetAttitude(UInt32 atti);
		string		GetFieldName() const;
		string		GetTypeName() const;
		EType		GetDataType() const;
		UInt32		GetRepeatCount() const;
		void		SetSetAtFunc(SetAtFunc func);
		void		SetGetAtFunc(GetAtFunc func);
		void		SetResizeFunc(ResizeFunc func);
		void		SetGetSizeFunc(GetSizeFunc func);
		void		SetInitPtrFunc(InitPtrFunc func);
		void		SetGetPtrFunc(GetPtrFunc func);
		void		SetAt(void* obj, UInt32 pos, void* value);
		template<typename T> T GetAt(void* obj, UInt32 pos);
		void*		GetAt(void* obj, UInt32 pos);
		void		Resize(void* obj, UInt32 size);
		UInt32		GetSize(void* obj);
		void		InitPtr(void* obj);
		void*		GetPtr(void* obj);
	};

	class Metadata {
	private:
		UInt32				m_Size;
		string				m_ClassName;
		const Metadata*		m_Parent;
		vector<Property>	m_Properties;

	public:
		Metadata(const string& className, UInt32 size);
		Metadata(const string& className, UInt32 size, const Metadata* parentMeta);
		Metadata(const string& className, UInt32 size, const Metadata* parentMeta, StaticInitHandler initHandler);
		UInt32					GetSize() const;
		const Metadata*			GetParentMetaData() const;
		string					GetClassName() const;
		const vector<Property>*	GetProperties() const;
		vector<Property>		GetAllProperties() const;
		Property				GetProperty(const string& fieldName) const;
		void					AddProperty(Property prop);
	};

	class RTTI {
	private:
		static map<string, ClassCreateHandler>& getMap();

	public:
		static void RegisterCreatetor(const char* clsName, ClassCreateHandler handler);

		template<typename T>
		static T* Instantiate(const string& className);

		static bool HasRTTIInfo(const string& className);
	};
}

// ���÷���Vector�Ĵ���
#define SET_VECTOR_FUNC(CLASS, TMPCLASS, fieldName) \
fieldName##Prop.SetSetAtFunc([](Property& prop, void* obj, UInt32 pos, void* value) {\
	vector<TMPCLASS>& list = *(vector<TMPCLASS>*)prop.GetAddress(obj);\
	list[pos] = *(TMPCLASS*)value;\
});\
fieldName##Prop.SetGetAtFunc([](Property& prop, void* obj, UInt32 pos) {\
	vector<TMPCLASS>& list = *(vector<TMPCLASS>*)prop.GetAddress(obj);\
	return (void*)&list[pos];\
});\
fieldName##Prop.SetResizeFunc([](Property& prop, void* obj, UInt32 size) {\
	vector<TMPCLASS>& list = *(vector<TMPCLASS>*)prop.GetAddress(obj);\
	list.resize(size);\
});\
fieldName##Prop.SetGetSizeFunc([](Property& prop, void* obj) {\
	vector<TMPCLASS>& list = *(vector<TMPCLASS>*)prop.GetAddress(obj);\
	return list.size();\
});\


// ���÷�������Ĵ���
#define SET_ARRAY_FUNC(CLASS, TMPCLASS, fieldName)\
fieldName##Prop.SetSetAtFunc([](Property& prop, void* obj, UInt32 pos, void* value) {\
	TMPCLASS* arr = (TMPCLASS*)prop.GetAddress(obj);\
	arr[pos] = *(TMPCLASS*)value;\
});\
fieldName##Prop.SetGetAtFunc([](Property& prop, void* obj, UInt32 pos) {\
	TMPCLASS* arr = (TMPCLASS*)prop.GetAddress(obj);\
	return (void*)&arr[pos];\
});\
fieldName##Prop.SetGetSizeFunc([](Property& prop, void* obj) {\
	return prop.GetRepeatCount();\
});\

// ���ó�ʼ��shared_ptr<>�Ĵ���
#define SET_SHARED_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
fieldName##Prop.SetInitPtrFunc([](Property& prop, void* obj) {\
	shared_ptr<TMPCLASS>* ptr = (shared_ptr<TMPCLASS>*)obj;\
	if (!(*ptr).get()) \
		*ptr = make_shared<TMPCLASS>();\
});\
fieldName##Prop.SetGetPtrFunc([](Property& prop, void* obj) {\
	shared_ptr<TMPCLASS>* ptr = (shared_ptr<TMPCLASS>*)obj;\
	prop.InitPtr(obj);\
	return (*ptr).get(); \
});\

// ���ó�ʼ��ָ��Ĵ���
#define SET_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
fieldName##Prop.SetInitPtrFunc([](Property& prop, void* obj) {\
	TMPCLASS* p = (TMPCLASS*)*(int*)obj;\
	if (p == NULL)\
		*(int*)(int)obj = (int)new TMPCLASS;\
});\
fieldName##Prop.SetGetPtrFunc([](Property& prop, void* obj) {\
	TMPCLASS* p = (TMPCLASS*)*(int*)obj;\
	prop.InitPtr(obj);\
	return p;\
});\

// ע���������
#define PROP(CLASS, fieldName, TYPE) \
meta.AddProperty(Property("", #fieldName, TYPE, offsetof(CLASS, fieldName)));

// ע����ͽṹ������
#define PROP_CLS(CLASS, TMPCLASS, fieldName) \
meta.AddProperty(Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata()));\

// ע����ͽṹ������ptr (������ʹ��)
#define PROP_PTR_CLS(CLASS, TMPCLASS, fieldName) \
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata());\
SET_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
meta.AddProperty(fieldName##Prop);\

// ע����ͽṹ������shared_ptr
#define PROP_SHARED_PTR_CLS(CLASS, TMPCLASS, fieldName) \
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata());\
SET_SHARED_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
meta.AddProperty(fieldName##Prop);\

// ע��std::vector<base type>����
#define PROP_VEC(CLASS, TMPCLASS, fieldName, TYPE) 	\
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, TYPE, offsetof(CLASS, fieldName), NULL, 1, DEFAUTL_ATTITUDE);\
SET_VECTOR_FUNC(CLASS, TMPCLASS, fieldName); \
meta.AddProperty(fieldName##Prop);\

// ע��ջ��������
#define PROP_ARR(CLASS, TMPCLASS, fieldName, TYPE) \
Property fieldName##Prop = Property("", #fieldName, TYPE, offsetof(CLASS, fieldName), NULL, /*_countof(CLASS::fieldName)*/sizeof(CLASS::fieldName) / sizeof(TMPCLASS), DEFAUTL_ATTITUDE);\
SET_ARRAY_FUNC(CLASS, TMPCLASS, fieldName);\
meta.AddProperty(fieldName##Prop); \

// ע��std::vector<class type>����
#define PROP_VEC_CLS(CLASS, TMPCLASS, fieldName) 	\
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata(), 1, DEFAUTL_ATTITUDE);\
SET_VECTOR_FUNC(CLASS, TMPCLASS, fieldName); \
meta.AddProperty(fieldName##Prop);\

// ע��std::vector<shared_ptr<class type>>����
#define PROP_VEC_SHARED_PTR_CLS(CLASS, TMPCLASS, fieldName) 	\
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata(), 1, DEFAUTL_ATTITUDE);\
SET_SHARED_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
SET_VECTOR_FUNC(CLASS, shared_ptr<TMPCLASS>, fieldName); \
meta.AddProperty(fieldName##Prop);\

// ע�����ṹ����������
#define PROP_ARR_CLS(CLASS, TMPCLASS, fieldName) \
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata(), /*_countof(CLASS::fieldName)*/sizeof(CLASS::fieldName) / sizeof(TMPCLASS), DEFAUTL_ATTITUDE);\
SET_ARRAY_FUNC(CLASS, TMPCLASS, fieldName); \
meta.AddProperty(fieldName##Prop); \

// ע�����ṹ��shared_ptr<> arr[]��������
#define PROP_ARR_SHARED_PTR_CLS(CLASS, TMPCLASS, fieldName) \
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata(), /*_countof(CLASS::fieldName)*/sizeof(CLASS::fieldName) / sizeof(shared_ptr<TMPCLASS>), DEFAUTL_ATTITUDE);\
SET_SHARED_PTR_FUNC(CLASS, TMPCLASS, fieldName)\
SET_ARRAY_FUNC(CLASS, shared_ptr<TMPCLASS>, fieldName); \
meta.AddProperty(fieldName##Prop); \

// ����RTTI����(����)
#define DECLARE_RTTI_ROOT() \
private:\
static Metadata _MetaData;\
static void Register(Metadata& meta);\
public:\
	static const Metadata* GetMetadata();\
	virtual const Metadata* metadata() const;\
private:

// ʵ��RTTI����(����)
#define IMPL_RTTI_ROOT(CLASS, PARENT_META, RegCode) \
Metadata CLASS::_MetaData(#CLASS, sizeof(CLASS), PARENT_META, CLASS::Register);\
void CLASS::Register(Metadata& meta)\
{\
	RTTI::RegisterCreatetor(typeid(CLASS).name(), []() { return new CLASS(); });\
	RegCode\
}\
const Metadata* CLASS::GetMetadata()\
{\
	return &_MetaData;\
}\
const Metadata* CLASS::metadata() const { return &_MetaData; }

// ����RTTI����(����)
#define DECLARE_RTTI DECLARE_RTTI_ROOT

// ʵ��RTTI����(����)
#define IMPL_RTTI IMPL_RTTI_ROOT

#include"RTTI.inl"