#include "MeshBuffer.h"

MeshBuffer::MeshBuffer() : m_vaoHandle(0), m_vboColorHandle(0), 
	m_vboNormalHandle(0), m_vboUVHandle(0), m_vboVertexHandle(0),
	m_vboJointIndexHandle(0), m_vboJointWeightHandle(0), m_vertexNum(0)
{

}

MeshBuffer::MeshBuffer(PMesh mesh) : MeshBuffer()
{
	MakeBuffer(mesh);
}

MeshBuffer::MeshBuffer(PMesh mesh, const vector<Vector4>& weights, const vector<BVector4>& indices) : MeshBuffer()
{
	MakeBuffer(mesh);
	MakeJointBuffer(weights, indices);
}

void MeshBuffer::MakeBuffer(const Vector3* vertices, const Color* colors, const Vector3* normals, const Vector2* uvs, int size)
{
	MakeVertexBuffer(vertices, size);
	MakeColorBuffer(colors, size);
	MakeNormalBuffer(normals, size);
	MakeUVBuffer(uvs, size);
}

MeshBuffer::~MeshBuffer()
{
	ReleaseBuffer();
}

void MeshBuffer::MakeVertexBuffer(const Vector3* vertices, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!vertices) return;
	m_vertexNum = size;
	if (!m_vboVertexHandle) glGenBuffers(1, &m_vboVertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertexHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * size, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTEX_POS);
	glVertexAttribPointer(VERTEX_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeColorBuffer(const Color* colors, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!colors) return;
	if (!m_vboColorHandle) glGenBuffers(1, &m_vboColorHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboColorHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * size, colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(COLOR_POS);
	glVertexAttribPointer(COLOR_POS, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeUVBuffer(const Vector2* uvs, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!uvs) return;
	if (!m_vboUVHandle) glGenBuffers(1, &m_vboUVHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboUVHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2) * size, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(UV_POS);
	glVertexAttribPointer(UV_POS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeNormalBuffer(const Vector3* normals, int size)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!normals) return;
	if (!m_vboNormalHandle) glGenBuffers(1, &m_vboNormalHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormalHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * size, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_POS);
	glVertexAttribPointer(NORMAL_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeJointBuffer(const vector<Vector4>& weights, const vector<BVector4>& indices)
{
	if (!m_vaoHandle) glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	if (!weights.size() || !indices.size()) return;
	if (!m_vboJointIndexHandle) glGenBuffers(1, &m_vboJointIndexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboJointIndexHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BVector4) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(JOINT_INDEX_POS);
	glVertexAttribIPointer(JOINT_INDEX_POS, 4, GL_UNSIGNED_BYTE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!m_vboJointWeightHandle) glGenBuffers(1, &m_vboJointWeightHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboJointWeightHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * weights.size(), &weights[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(JOINT_WEIGHT_POS);
	glVertexAttribPointer(JOINT_WEIGHT_POS, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::MakeBuffer(PMesh mesh)
{
	MakeBuffer(mesh->m_vertices, mesh->m_colors, mesh->m_normals, mesh->m_texcoords, mesh->m_vertexCount);
}

void MeshBuffer::UpdateVertices(const Vector3* vertices, int offset, int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertexHandle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size * sizeof(Vector3), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshBuffer::BindBuffer()
{
	glBindVertexArray(m_vaoHandle);
}

void MeshBuffer::ReleaseBuffer()
{
	if (m_vboColorHandle) glDeleteBuffers(1, &m_vboColorHandle);
	if (m_vboUVHandle) glDeleteBuffers(1, &m_vboUVHandle);
	if (m_vboNormalHandle) glDeleteBuffers(1, &m_vboNormalHandle);
	if (m_vboVertexHandle) glDeleteBuffers(1, &m_vboVertexHandle);
	if (m_vaoHandle) glDeleteVertexArrays(1, &m_vaoHandle);
	if (m_vboJointIndexHandle) glDeleteBuffers(1, &m_vboJointIndexHandle);
	if (m_vboJointWeightHandle) glDeleteVertexArrays(1, &m_vboJointWeightHandle);
}

int MeshBuffer::GetVertexNum() const
{
	return m_vertexNum;
}