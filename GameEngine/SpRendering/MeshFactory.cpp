#include "MeshFactory.h"

USING_NAMESPACE_ENGINE

PMesh MeshFactory::CreateCube()
{
	static const int VERTEX_NUM = 36;
	static const Vector3 vertices[VERTEX_NUM] = {
		//front
		{ -0.5f, 0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f },
		//back
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },
		//left
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, -0.5f },
		//right
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		//top
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, 0.5f, 0.5f },
		//bottom
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, -0.5f },
	};
	static const Vector3 normals[VERTEX_NUM] = {
		//front
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		//back
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		//left
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		//right
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		//top
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		//bottom
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
	};
	Vector2 texcoords[VERTEX_NUM];
	for (int i = 0; i < VERTEX_NUM;)
	{
		texcoords[i++] = Vector2(0.0f, 1.0f);
		texcoords[i++] = Vector2(0.0f, 0.0f);
		texcoords[i++] = Vector2(1.0f, 0.0f);
		texcoords[i++] = Vector2(0.0f, 1.0f);
		texcoords[i++] = Vector2(1.0f, 0.0f);
		texcoords[i++] = Vector2(1.0f, 1.0f);
	}
	return CreateMesh(vertices, texcoords, normals, VERTEX_NUM);
}

PMesh MeshFactory::CreateQuad()
{
	static const int VERTEX_NUM = 6;
	static const Vector3 vertices[VERTEX_NUM] = {
		{-0.5f, 0.5f, 0.0f},
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f},
	};
	static const Vector2 texcoords[VERTEX_NUM] = {
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
	};
	static const Vector3 normals[VERTEX_NUM] = {
		//front
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	return CreateMesh(vertices, texcoords, normals, VERTEX_NUM);
}

PMesh MeshFactory::CreateMesh(const Vector3* vertices, const Vector2* texcoords, const Vector3* normals, int count)
{
	PMesh mesh(new Mesh);
	mesh->m_vertexCount = count;
	if (vertices)
	{
		mesh->m_vertices = (Vector3*)malloc(sizeof(Vector3) * count);
		memcpy(mesh->m_vertices, vertices, sizeof(Vector3) * count);
	}
	if (texcoords)
	{
		mesh->m_texcoords = (Vector2*)malloc(sizeof(Vector2) * count);
		memcpy(mesh->m_texcoords, texcoords, sizeof(Vector2) * count);
	}
	if (normals)
	{
		mesh->m_normals = (Vector3*)malloc(sizeof(Vector3) * count);
		memcpy(mesh->m_normals, normals, sizeof(Vector3) * count);
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
	return CreateMesh(vertices, texcoords, NULL, VERTEX_NUM);
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
	auto it = m_meshes.find(type);
	if (it == m_meshes.end())
	{
		PMesh mesh = CreateMesh(type);
		m_meshes.insert(make_pair(type, mesh));
		return mesh;
	}
	return it->second;
}

PMeshBuffer MeshFactory::CreateBuffer(EMeshType type)
{
	PMesh mesh = CreateMesh(type);
	PMeshBuffer buffer(new MeshBuffer(CreateMesh(type)));
	return buffer;
}

PMeshBuffer MeshFactory::SharedBuffer(EMeshType type)
{
	PMesh mesh = SharedMesh(type);
	auto it = m_buffers.find(type);
	if (it == m_buffers.end())
	{
		PMeshBuffer buffer(new MeshBuffer(mesh));
		m_buffers.insert(make_pair(type, buffer));
		return buffer;
	}
	return it->second;
}