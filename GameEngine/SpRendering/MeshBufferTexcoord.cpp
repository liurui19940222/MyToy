#include "MeshBufferTexcoord.h"

USING_NAMESPACE_ENGINE;

MeshBufferTexcoord::MeshBufferTexcoord() : MeshBuffer(), m_VboTexcoordHandle(0)
{
}

MeshBufferTexcoord::MeshBufferTexcoord(PMesh mesh) : MeshBuffer(mesh), m_VboTexcoordHandle(0)
{
	MakeTexcoordBuffer(mesh->m_texcoords, mesh->m_vertexCount);
}

void MeshBufferTexcoord::MakeTexcoordBuffer(const Vector2* uvs, int size)
{
	BindBuffer();
	MakeVertexBuffer(&m_VboTexcoordHandle, sizeof(Vector2) * size, 2, uvs, ATTR_POS_TEXCOORD, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferTexcoord::MakeBuffer(PMesh mesh)
{
	MeshBuffer::MakeBuffer(mesh);
	MakeTexcoordBuffer(mesh->m_texcoords, mesh->m_vertexCount);
}

void MeshBufferTexcoord::ReleaseBuffer()
{
	DeleteBufer(&m_VboTexcoordHandle);
	MeshBuffer::ReleaseBuffer();
}