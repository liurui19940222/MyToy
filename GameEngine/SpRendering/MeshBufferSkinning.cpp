#include "MeshBufferSkinning.h"

USING_NAMESPACE_ENGINE;

MeshBufferSkinning::MeshBufferSkinning() : MeshBufferNormal(), m_VboJointIndexHandle(0), m_VboJointWeightHandle(0){}

MeshBufferSkinning::MeshBufferSkinning(PMesh mesh) : MeshBufferNormal(mesh), m_VboJointIndexHandle(0), m_VboJointWeightHandle(0) 
{
	MakeJointBuffer(mesh->m_JointWeights, mesh->m_JointIndices, mesh->m_VertexCount);
}

MeshBufferSkinning::~MeshBufferSkinning()
{
	DeleteBufer(&m_VboJointIndexHandle);
	DeleteBufer(&m_VboJointWeightHandle);
}

void MeshBufferSkinning::MakeJointBuffer(const Vector4* weights, const BVector4* indices, int count)
{
	BindBuffer();
	MakeIntegerVertexBuffer(&m_VboJointIndexHandle, sizeof(BVector4) * count, 4, indices, ATTR_POS_JOINT_INDEX, EBufferUsage::StaticDraw, EDataType::UNSIGNED_BYTE);
	MakeVertexBuffer(&m_VboJointWeightHandle, sizeof(Vector4) * count, 4, weights, ATTR_POS_JOINT_WEIGHT, EBufferUsage::StaticDraw);
	UnbindBuffer();
}
