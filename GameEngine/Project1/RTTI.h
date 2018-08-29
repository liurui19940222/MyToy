#pragma once

#include<string>
#include<vector>
#include<map>
#include<functional>

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
	typedef long int			Int64;
	typedef unsigned long int	UInt64;
	typedef float				Float;
	typedef double				Double;
	typedef string				String;
	typedef wstring				WString;

	class Metadata;
	class Property;
	typedef function<void(Metadata&)>	StaticInitHandler;
	typedef function<void*()>			ClassCreateHandler;
	typedef function<void(Property& prop, void* obj, UInt32 pos, void* value)> SetAtFunc;
	typedef function<void*(Property& prop, void* obj, UInt32 pos)> GetAtFunc;
	typedef function<UInt32(Property& prop, void* obj)> GetSizeFunc;
	typedef function<void(Property& prop, void* obj, UInt32 size)> ResizeFunc;

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
		Enum,
		Pointer,
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
	};

	class Property
	{
	private:
		UInt32		m_OffsetAddr;
		UInt32		m_Attitude;
		UInt32		m_RepeatCount;	//大于0时为数组, vector
		EType	m_DataType;

		string		m_FieldName;
		string		m_TypeName;
		SetAtFunc	m_SetAtFunc;
		GetAtFunc	m_GetAtFunc;
		GetSizeFunc m_GetSizeFunc;
		ResizeFunc	m_ResizeFunc;

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
		EType	GetDataType() const;
		UInt32		GetRepeatCount() const;
		void		SetSetAtFunc(SetAtFunc func);
		void		SetGetAtFunc(GetAtFunc func);
		void		SetResizeFunc(ResizeFunc func);
		void		SetGetSizeFunc(GetSizeFunc func);
		void		SetAt(void* obj, UInt32 pos, void* value);
		template<typename T> T GetAt(void* obj, UInt32 pos);
		void*		GetAt(void* obj, UInt32 pos);
		void		Resize(void* obj, UInt32 size);
		UInt32		GetSize(void* obj);
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
		Property				GetProperty(const string& fieldName) const;
		void					AddProperty(Property prop);
	};

	class RTTI {
	private:
		static map<string, ClassCreateHandler> m_Creators;

	public:
		template<typename T>
		static void RegisterCreatetor(ClassCreateHandler handler);

		template<typename T>
		static T* Instantiate(const string& className);

		static bool HasRTTIInfo(const string& className) { return m_Creators.find(className) != m_Creators.end(); }
	};
}


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
meta.AddProperty(fieldName##Prop);\

#define PROP_VEC_CLS(CLASS, TMPCLASS, fieldName) 	\
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata(), 1, DEFAUTL_ATTITUDE);\
SET_VECTOR_FUNC(CLASS, TMPCLASS, fieldName);

#define PROP_VEC(CLASS, TMPCLASS, fieldName, TYPE) 	\
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, TYPE, offsetof(CLASS, fieldName), NULL, 1, DEFAUTL_ATTITUDE);\
SET_VECTOR_FUNC(CLASS, TMPCLASS, fieldName);

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
meta.AddProperty(fieldName##Prop);\

#define PROP_ARR_CLS(CLASS, TMPCLASS, fieldName) \
Property fieldName##Prop = Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, colors), TMPCLASS::GetMetadata(), /*_countof(CLASS::fieldName)*/sizeof(CLASS::fieldName) / sizeof(TMPCLASS), DEFAUTL_ATTITUDE);\
SET_ARRAY_FUNC(CLASS, TMPCLASS, fieldName);

#define PROP_ARR(CLASS, TMPCLASS, fieldName, TYPE) \
Property fieldName##Prop = Property("", #fieldName, TYPE, offsetof(CLASS, fieldName), NULL, /*_countof(CLASS::fieldName)*/sizeof(CLASS::fieldName) / sizeof(TMPCLASS), DEFAUTL_ATTITUDE);\
SET_ARRAY_FUNC(CLASS, TMPCLASS, fieldName);

#define PROP(CLASS, fieldName, TYPE) \
meta.AddProperty(Property("", #fieldName, TYPE, offsetof(CLASS, fieldName)));

#define PROP_CLS(CLASS, TMPCLASS, fieldName) \
meta.AddProperty(Property(#TMPCLASS, #fieldName, EType::Class, offsetof(CLASS, fieldName), TMPCLASS::GetMetadata()));\

#include"RTTI.inl"