#include"Object.h"

IMPL_RTTI(Object, NULL, {
	PROP(Object, m_Name, EType::WString)
})

void Object::OnSerilize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
{
	if (m_Reference.empty() || depth == 0)
	{
		SerializableObject::OnSerilize(depth, meta, value, allocator);
	}
	else
	{
		
	}
}

void Object::OnDeserialize(int depth, const Metadata* meta, Value& value)
{
	if (m_Reference.empty() || depth == 0)
	{
		SerializableObject::OnDeserialize(depth, meta, value);
	}
	else
	{

	}
}