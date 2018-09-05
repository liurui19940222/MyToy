#include "MeshBufferNormal.h"

USING_NAMESPACE_ENGINE;

MeshBufferNormal::MeshBufferNormal() : MeshBufferTexcoord(), m_VboNormalHandle(0)
{
}

MeshBufferNormal::MeshBufferNormal(MeshPtr mesh) : MeshBufferTexcoord(mesh), m_VboNormalHandle(0)
{
	MakeNormalBuffer(mesh->m_Normals, mesh->m_VertexCount);
}

MeshBufferNormal::~MeshBufferNormal() 
{
	DeleteBufer(&m_VboNormalHandle);
}

void MeshBufferNormal::MakeNormalBuffer(const Vector3* normals, int size)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboNormalHandle, sizeof(Vector3) * size, 3, normals, ATTR_POS_NORMAL, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferNormal::MakeBuffer(MeshPtr mesh)
{
	MeshBufferTexcoord::MakeBuffer(mesh);
	MakeNormalBuffer(mesh->m_Normals, mesh->m_VertexCount);
}
