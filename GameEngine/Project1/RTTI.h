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

	class Metadata;
	typedef function<void(Metadata&)>		StaticInitHandler;
	typedef function<void*(const string&)>	ClassCreateHandler;

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
		DataType	m_DataType;

		string		m_FieldName;
		string		m_TypeName;

	public:
		Property();
		Property(const string& typeName, const string& fieldName, DataType dataType, UInt32 offsetAddr, UInt32 attitude = DEFAUTL_ATTITUDE);

		template<typename T> T		GetValue(void* obj) const;
		template<typename T> void	SetValue(void* obj, T value);
		bool		HasAttitude(UInt32 atti) const;
		void		AddAttitude(UInt32 atti);
		void		SetAttitude(UInt32 atti);
		string		GetFieldName() const;
		string		GetTypeName() const;
		DataType	GetDataType() const;
	};

	class Metadata {
	private:
		string				m_ClassName;
		const Metadata*		m_Parent;
		vector<Property>	m_Properties;

	public:
		Metadata(const string& className);
		Metadata(const string& className, const Metadata* parentMeta);
		Metadata(const string& className, const Metadata* parentMeta, StaticInitHandler initHandler);
		const Metadata*			GetParentMetaData() const;
		string					GetClassName() const;
		const vector<Property>&	GetProperties() const;
		void					AddProperty(Property prop);
	};

	class RTTI {
	private:
		map<string, ClassCreateHandler> m_Creators;
	};
}

#include"RTTI.inl"