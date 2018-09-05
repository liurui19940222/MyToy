#include "MeshBufferTexcoord.h"

USING_NAMESPACE_ENGINE;

MeshBufferTexcoord::MeshBufferTexcoord() : MeshBuffer(), m_VboTexcoordHandle(0)
{
}

MeshBufferTexcoord::MeshBufferTexcoord(MeshPtr mesh) : MeshBuffer(mesh), m_VboTexcoordHandle(0)
{
	MakeTexcoordBuffer(mesh->m_Texcoords, mesh->m_VertexCount);
}

MeshBufferTexcoord::~MeshBufferTexcoord()
{
	DeleteBufer(&m_VboTexcoordHandle);
}

void MeshBufferTexcoord::MakeTexcoordBuffer(const Vector2* uvs, int size)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboTexcoordHandle, sizeof(Vector2) * size, 2, uvs, ATTR_POS_TEXCOORD, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferTexcoord::MakeBuffer(MeshPtr mesh)
{
	MeshBuffer::MakeBuffer(mesh);
	MakeTexcoordBuffer(mesh->m_Texcoords, mesh->m_VertexCount);
}
