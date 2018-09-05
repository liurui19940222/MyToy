#include "RTTI.h"
#include<list>
#include<algorithm>

using namespace rtti;

inline Property::Property() :
	m_DataType(EType::UnkownType),
	m_OffsetAddr(0),
	m_Attitude(0),
	m_RepeatCount(0)
{ }

inline Property::Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, UInt32 attitude) :
	m_TypeName(typeName),
	m_FieldName(fieldName),
	m_DataType(dataType),
	m_OffsetAddr(offsetAddr),
	m_Attitude(attitude),
	m_RepeatCount(0)
{ }

inline Property::Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, const Metadata* metadata, UInt32 attitude) :
	m_TypeName(typeName),
	m_FieldName(fieldName),
	m_DataType(dataType),
	m_OffsetAddr(offsetAddr),
	m_Metadata(metadata),
	m_Attitude(attitude),
	m_RepeatCount(0)
{ }

inline Property::Property(const string& typeName, const string& fieldName, EType dataType, UInt32 offsetAddr, const Metadata* metadata, UInt32 repeatCount, UInt32 attitude) :
	m_TypeName(typeName),
	m_FieldName(fieldName),
	m_DataType(dataType),
	m_OffsetAddr(offsetAddr),
	m_Metadata(metadata),
	m_Attitude(attitude),
	m_RepeatCount(repeatCount)
{ }

template<typename T> T Property::GetValue(void* obj) const { return *((T*)(((char*)obj) + m_OffsetAddr)); }

template<typename T> void Property::SetValue(void* obj, T value) { *((T*)(((char*)obj) + m_OffsetAddr)) = value; }

inline void* Property::GetAddress(void* obj) const { return (char*)obj + m_OffsetAddr; }

inline bool	Property::HasAttitude(UInt32 atti) const { return (m_Attitude & atti) > 0; }

inline void Property::AddAttitude(UInt32 atti) { m_Attitude |= atti; }

inline void Property::SetAttitude(UInt32 atti) { m_Attitude = atti; }

inline string Property::GetFieldName() const { return m_FieldName; }

inline string Property::GetTypeName() const { return m_TypeName; }

inline EType Property::GetDataType() const { return m_DataType; }

inline UInt32 Property::GetRepeatCount() const { return m_RepeatCount; }

inline void	Property::SetSetAtFunc(SetAtFunc func) { m_SetAtFunc = func; }

inline void	Property::SetGetAtFunc(GetAtFunc func) { m_GetAtFunc = func; }

inline void	Property::SetResizeFunc(ResizeFunc func) { m_ResizeFunc = func; }

inline void	Property::SetGetSizeFunc(GetSizeFunc func) { m_GetSizeFunc = func; }

inline void	Property::SetInitPtrFunc(InitPtrFunc func) { m_InitPtrFunc = func; }

inline void	Property::SetGetPtrFunc(GetPtrFunc func) { m_GetPtrFunc = func; }

inline void	Property::SetAt(void* obj, UInt32 pos, void* value) { m_SetAtFunc(*this, obj, pos, value); }

template<typename T> T Property::GetAt(void* obj, UInt32 pos) { return *(T*)GetAt(obj, pos); }

inline void* Property::GetAt(void* obj, UInt32 pos) { 
	if (m_GetPtrFunc)
	{
		return  m_GetPtrFunc(*this, (m_GetAtFunc(*this, obj, pos)));
	}
	return m_GetAtFunc(*this, obj, pos); 
}

inline void	Property::Resize(void* obj, UInt32 size) { if (m_ResizeFunc) m_ResizeFunc(*this, obj, size); }

inline UInt32 Property::GetSize(void* obj) { if (m_GetSizeFunc) return m_GetSizeFunc(*this, obj); else return m_RepeatCount; }

inline void Property::InitPtr(void* obj) { 
	if (m_InitPtrFunc) m_InitPtrFunc(*this, obj); 
}

inline void* Property::GetPtr(void* obj) { if (m_GetPtrFunc) return m_GetPtrFunc(*this, GetAddress(obj)); return GetAddress(obj); }

inline Metadata::Metadata(const string& className, UInt32 size) : m_Parent(NULL), m_Size(size) {}
inline Metadata::Metadata(const string& className, UInt32 size, const Metadata* parentMeta) : m_ClassName(className), m_Size(size), m_Parent(parentMeta) {}
inline Metadata::Metadata(const string& className, UInt32 size, const Metadata* parentMeta, StaticInitHandler initHandler) : m_ClassName(className), m_Size(size), m_Parent(parentMeta)
{
	if (initHandler)
		initHandler(*this);
}
inline UInt32					Metadata::GetSize() const { return m_Size; }
inline const Metadata*			Metadata::GetParentMetaData() const { return m_Parent; }
inline string					Metadata::GetClassName() const { return m_ClassName; }
inline const vector<Property>*	Metadata::GetProperties() const { return &m_Properties; }
inline void						Metadata::AddProperty(Property prop) { m_Properties.push_back(prop); }
inline Property					Metadata::GetProperty(const string& fieldName) const 
{
	for(auto it = m_Properties.begin(); it != m_Properties.end(); ++it)
	{
		if (it->GetFieldName() == fieldName)
			return (*it);
	}
	return Property{};
}

inline map<string, ClassCreateHandler>& RTTI::getMap() { static map<string, ClassCreateHandler> _map; return _map; }

template<typename T>
T* RTTI::Instantiate(const string& className)
{
	auto it = getMap().find(className);
	if (it != getMap().end())
		return (T*)(it->second());
	return NULL;
}

inline bool RTTI::HasRTTIInfo(const string& className) { return getMap().find(className) != getMap().end(); }