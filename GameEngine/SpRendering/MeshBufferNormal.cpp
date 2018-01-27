#include "MeshBufferNormal.h"

USING_NAMESPACE_ENGINE;

MeshBufferNormal::MeshBufferNormal() : MeshBufferTexcoord(), m_VboNormalHandle(0)
{
}

MeshBufferNormal::MeshBufferNormal(PMesh mesh) : MeshBufferTexcoord(mesh), m_VboNormalHandle(0)
{
	MakeNormalBuffer(mesh->m_normals, mesh->m_vertexCount);
}

void MeshBufferNormal::MakeNormalBuffer(const Vector3* normals, int size)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboNormalHandle, sizeof(Vector3) * size, 3, normals, ATTR_POS_NORMAL, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferNormal::MakeBuffer(PMesh mesh)
{
	MeshBufferTexcoord::MakeBuffer(mesh);
	MakeNormalBuffer(mesh->m_normals, mesh->m_vertexCount);
}

void MeshBufferNormal::ReleaseBuffer()
{
	DeleteBufer(&m_VboNormalHandle);
	MeshBufferTexcoord::ReleaseBuffer();
}