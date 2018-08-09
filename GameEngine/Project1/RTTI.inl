#include "RTTI.h"

using namespace rtti;

inline Property::Property() :
	m_DataType(DataType::UnkownType),
	m_OffsetAddr(0),
	m_Attitude(0)
{ }

inline Property::Property(const string& typeName, const string& fieldName, DataType dataType, UInt32 offsetAddr, UInt32 attitude) :
	m_TypeName(typeName),
	m_FieldName(fieldName),
	m_DataType(dataType),
	m_OffsetAddr(offsetAddr),
	m_Attitude(attitude)
{ }

template<typename T> T Property::GetValue(void* obj) const { return *((T*)(((char*)obj) + m_OffsetAddr)); }

template<typename T> void Property::SetValue(void* obj, T value) { *((T*)(((char*)obj) + m_OffsetAddr)) = value; }

inline bool	Property::HasAttitude(UInt32 atti) const { return (m_Attitude & atti) > 0; }

inline void Property::AddAttitude(UInt32 atti) { m_Attitude |= atti; }

inline void Property::SetAttitude(UInt32 atti) { m_Attitude = atti; }

inline string Property::GetFieldName() const { return m_FieldName; }

inline string Property::GetTypeName() const { return m_TypeName; }

inline DataType Property::GetDataType() const { return m_DataType; }

inline Metadata::Metadata(const string& className) : m_Parent(NULL) {}
inline Metadata::Metadata(const string& className, const Metadata* parentMeta) : m_ClassName(className), m_Parent(NULL) {}
inline Metadata::Metadata(const string& className, const Metadata* parentMeta, StaticInitHandler initHandler) : m_ClassName(className), m_Parent(NULL) 
{
	if (initHandler)
		initHandler(*this);
}
inline const Metadata*			Metadata::GetParentMetaData() const { return m_Parent; }
inline string					Metadata::GetClassName() const { return m_ClassName; }
inline const vector<Property>&	Metadata::GetProperties() const { return m_Properties; }
inline void						Metadata::AddProperty(Property prop) { m_Properties.push_back(prop); }