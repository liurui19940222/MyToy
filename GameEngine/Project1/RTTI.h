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
	typedef function<void(Metadata&)>	StaticInitHandler;
	typedef function<void*()>			ClassCreateHandler;

	enum class DataType {
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
		UInt32		m_RepeatCount;	//大于0时为数组
		DataType	m_DataType;

		string		m_FieldName;
		string		m_TypeName;

	public:
		const Metadata*	m_Metadata;

		Property();
		Property(const string& typeName, const string& fieldName, DataType dataType, UInt32 offsetAddr, UInt32 attitude = DEFAUTL_ATTITUDE);
		Property(const string& typeName, const string& fieldName, DataType dataType, UInt32 offsetAddr, const Metadata*	metadata, UInt32 attitude = DEFAUTL_ATTITUDE);
		Property(const string& typeName, const string& fieldName, DataType dataType, UInt32 offsetAddr, const Metadata*	metadata, UInt32 repeatCount, UInt32 attitude = DEFAUTL_ATTITUDE);

		template<typename T> T		GetValue(void* obj) const;
		template<typename T> void	SetValue(void* obj, T value);
		void*						GetAddress(void* obj) const;
		bool		HasAttitude(UInt32 atti) const;
		void		AddAttitude(UInt32 atti);
		void		SetAttitude(UInt32 atti);
		string		GetFieldName() const;
		string		GetTypeName() const;
		DataType	GetDataType() const;
		UInt32		GetRepeatCount() const;
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

#include"RTTI.inl"