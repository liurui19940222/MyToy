#include "MeshBufferSkinning.h"

USING_NAMESPACE_ENGINE;

MeshBufferSkinning::MeshBufferSkinning() : MeshBufferNormal(), m_VboJointIndexHandle(0), m_VboJointWeightHandle(0){}

MeshBufferSkinning::MeshBufferSkinning(PMesh mesh) : MeshBufferNormal(mesh), m_VboJointIndexHandle(0), m_VboJointWeightHandle(0) {}

MeshBufferSkinning::MeshBufferSkinning(PMesh mesh, const vector<Vector4>& weights, const vector<BVector4>& indices)
	: MeshBufferNormal(mesh), m_VboJointIndexHandle(0), m_VboJointWeightHandle(0)
{
	MakeJointBuffer(weights, indices);
}

void MeshBufferSkinning::MakeJointBuffer(const vector<Vector4>& weights, const vector<BVector4>& indices)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboJointIndexHandle, sizeof(BVector4) * indices.size(), 4, &indices[0], ATTR_POS_JOINT_INDEX, EBufferUsage::StaticDraw);
	MakeVertexBuffer(&m_VboJointWeightHandle, sizeof(Vector4) * weights.size(), 4, &weights[0], ATTR_POS_JOINT_WEIGHT, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferSkinning::ReleaseBuffer()
{
	DeleteBufer(&m_VboJointIndexHandle);
	DeleteBufer(&m_VboJointWeightHandle);
	MeshBufferNormal::ReleaseBuffer();
}