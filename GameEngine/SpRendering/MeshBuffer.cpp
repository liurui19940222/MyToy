#include "MeshBuffer.h"

USING_NAMESPACE_ENGINE;

MeshBuffer::MeshBuffer() : m_VaoHandle(0), m_VboPositionHandle(0), m_VertexNum(0)
{
	glGenVertexArrays(1, &m_VaoHandle);
}

MeshBuffer::MeshBuffer(PMesh mesh) : MeshBuffer()
{
	MakePositionBuffer(mesh->m_vertices, mesh->m_vertexCount);
}

MeshBuffer::~MeshBuffer()
{
	ReleaseBuffer();
	DeleteBufer(&m_VaoHandle);
}

void MeshBuffer::MakePositionBuffer(const Vector3* vertices, int size)
{
	m_VertexNum = size;
	BindBuffer();
	MakeVertexBuffer(&m_VboPositionHandle, sizeof(Vector3) * size, 3, vertices, 0, EBufferUsage::DynamicDraw);
	UnbindBuffer();
}

void MeshBuffer::UpdateVertices(const Vector3* vertices, int offset, int size)
{
	UpdateVertexBuffer(m_VboPositionHandle, offset, size * sizeof(Vector3), vertices);
}

void MeshBuffer::MakeBuffer(PMesh mesh)
{
	MakePositionBuffer(mesh->m_vertices, mesh->m_vertexCount);
}

void MeshBuffer::BindBuffer()
{
	glBindVertexArray(m_VaoHandle);
}

void MeshBuffer::ReleaseBuffer()
{
	DeleteBufer(&m_VboPositionHandle);
}

void MeshBuffer::MakeVertexBuffer(GLuint *bufferId, int size, int componentCount, const void* pointer, int attrPos, EBufferUsage usage, EDataType dataType)
{
	if (!(*bufferId)) 
		glGenBuffers(1, bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, *bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, pointer, (GLenum)usage);
	glEnableVertexAttribArray(attrPos);
	glVertexAttribPointer(attrPos, componentCount, (GLenum)dataType, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeIntegerVertexBuffer(GLuint *bufferId, int size, int componentCount, const void* pointer, int attrPos, EBufferUsage usage, EDataType dataType)
{
	if (!(*bufferId))
		glGenBuffers(1, bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, *bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, pointer, (GLenum)usage);
	glEnableVertexAttribArray(attrPos);
	glVertexAttribIPointer(attrPos, componentCount, (GLenum)dataType, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeIndexBuffer(GLuint* bufferId, int dataSize, const void* pointer)
{
	if (!(*bufferId))
		glGenBuffers(1, bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, pointer, GL_STATIC_DRAW);
}

void MeshBuffer::MakeInstanceVertexBuffer(GLuint* bufferId, int structSize, int componentCount, int instanceCount, const void* pointer, int attrPos, EBufferUsage usage, EDataType dataType)
{
	glGenBuffers(1, bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, *bufferId);
	glBufferData(GL_ARRAY_BUFFER, structSize * instanceCount, pointer, (GLenum)usage);
	int oneSize = structSize / componentCount;
	for (int i = 0; i < oneSize; ++i)
	{
		glVertexAttribPointer(attrPos + i, 4, (GLenum)dataType, GL_FALSE, structSize, (void*)(sizeof(Vector4) * i));
		glEnableVertexAttribArray(attrPos + i);
		glVertexAttribDivisor(attrPos + i, 1);
	}
}

void MeshBuffer::UpdateVertexBuffer(GLuint bufferId, int offset, int dataSize, const void* pointer)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, pointer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::DeleteBufer(GLuint* bufferId) 
{
	if (bufferId && *bufferId)
		glDeleteBuffers(1, bufferId);
	*bufferId = 0;
}