#include"Object.h"
#include"AssetUtility.h"

IMPL_RTTI(Object, NULL, {
	PROP(Object, m_Name, EType::WString)
})

void Object::OnSerialize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
{
	if (depth == 0 || m_Reference.empty() || !AssetUtility::CheckReference(this))
	{
		SerializableObject::OnSerialize(depth, meta, value, allocator);
	}
	else
	{
		value.SetString(m_Reference.c_str(), allocator);
	}
}

void Object::OnDeserialize(int depth, const Metadata* meta, Value& value)
{
	if (depth == 0 || m_Reference.empty() || !AssetUtility::CheckReference(this))
	{
		SerializableObject::OnDeserialize(depth, meta, value);
	}
	else
	{
		AssetUtility::Load(this, m_Reference);
	}
}