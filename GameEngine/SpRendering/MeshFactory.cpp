#include "MeshFactory.h"
#include "MeshBufferNormal.h"

USING_NAMESPACE_ENGINE

PMesh MeshFactory::CreateCube()
{
	static const int VERTEX_NUM = 36;
	static const float vertices[] = {
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
	};
	static const float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	static const float texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	static const ushort indices[] = {
		0, 2, 3,
		0, 3, 1,
		8, 4, 5,
		8, 5, 9,
		10, 6, 7,
		10, 7, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};
	return CreateMesh((Vector3*)vertices, (Vector2*)texcoords, (Vector3*)normals, indices, VERTEX_NUM, 12);
}

PMesh MeshFactory::CreateQuad()
{
	static const int VERTEX_NUM = 4;
	static const Vector3 vertices[VERTEX_NUM] = {
		{-0.5f, 0.5f, 0.0f},
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
	};
	static const Vector2 texcoords[VERTEX_NUM] = {
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
	};
	static const Vector3 normals[VERTEX_NUM] = {
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	static const ushort indices[6] = 
	{
		0, 1, 2,
		0, 2, 3
	};
	return CreateMesh(vertices, texcoords, normals, indices, VERTEX_NUM, 2);
}

PMesh MeshFactory::CreateMesh(const Vector3* vertices, const Vector2* texcoords, const Vector3* normals, const ushort* indices, int vertexCount, int triangleCount)
{
	PMesh mesh(new Mesh);
	mesh->m_VertexCount = vertexCount;
	mesh->m_TriangleCount = triangleCount;
	size_t dataSize = 0;
	if (vertices)
	{
		dataSize = sizeof(Vector3) * vertexCount;
		mesh->m_Vertices = (Vector3*)malloc(dataSize);
		memcpy(mesh->m_Vertices, vertices, dataSize);
	}
	if (texcoords)
	{
		dataSize = sizeof(Vector2) * vertexCount;
		mesh->m_Texcoords = (Vector2*)malloc(dataSize);
		memcpy(mesh->m_Texcoords, texcoords, dataSize);
	}
	if (normals)
	{
		dataSize = sizeof(Vector3) * vertexCount;
		mesh->m_Normals = (Vector3*)malloc(dataSize);
		memcpy(mesh->m_Normals, normals, dataSize);
	}
	if (indices)
	{
		dataSize = sizeof(ushort) * triangleCount * 3;
		mesh->m_Indices = (ushort*)malloc(dataSize);
		memcpy(mesh->m_Indices, indices, dataSize);
	}
	return mesh;
}

PMesh MeshFactory::CreateRectMesh(float width, float height)
{
	float half_w = width * 0.5f;
	float half_h = height * 0.5f;
	static const int VERTEX_NUM = 6;
	Vector3 vertices[VERTEX_NUM] = {
		{ -half_w, half_h, 0.0f },
		{ -half_w, -half_h, 0.0f },
		{ half_w, -half_h, 0.0f },
		{ half_w, -half_h, 0.0f },
		{ half_w, half_h, 0.0f },
		{ -half_w, half_h, 0.0f },
	};
	static const Vector2 texcoords[VERTEX_NUM] = {
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};
	return CreateMesh(vertices, texcoords, NULL, NULL, VERTEX_NUM, 0);
}

PMesh MeshFactory::CreateMesh(EMeshType type)
{
	switch (type)
	{
	case EMeshType::Cube:
		return CreateCube();
	case EMeshType::Quad:
		return CreateQuad();
	}
	return NULL;
}

PMesh MeshFactory::SharedMesh(EMeshType type)
{
	auto it = m_Meshes.find(type);
	if (it == m_Meshes.end())
	{
		PMesh mesh = CreateMesh(type);
		m_Meshes.insert(make_pair(type, mesh));
		return mesh;
	}
	return it->second;
}

PMeshBuffer MeshFactory::SharedBuffer(EMeshType type)
{
	PMesh mesh = SharedMesh(type);
	auto it = m_buffers.find(type);
	if (it == m_buffers.end())
	{
		PMeshBufferNormal buffer(new MeshBufferNormal(mesh));
		m_buffers.insert(make_pair(type, buffer));
		return buffer;
	}
	return it->second;
}