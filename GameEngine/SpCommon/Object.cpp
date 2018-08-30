#include"Object.h"

IMPL_RTTI_ROOT(Object, NULL, {
	PROP(Object, m_InstanceId, EType::Int32)
	PROP(Object, m_Name, EType::WString)
})

string Object::Serialize() { return string(); }

void Object::Deserialize(const string& json) {}