#include "MeshCube.h"

Vector3 CMeshCube::vertices[VERTEX_NUM] = {
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

Mesh* CMeshCube::Create()
{
	Mesh* mesh = new Mesh;
	mesh->m_vertexCount = VERTEX_NUM;
	mesh->m_vertices = (Vector3*)malloc(sizeof(Vector3) * VERTEX_NUM);
	mesh->m_texcoords = (Vector2*)malloc(sizeof(Vector2) * VERTEX_NUM);
	memcpy(mesh->m_vertices, vertices, sizeof(Vector3) * VERTEX_NUM);

	for (int i = 0; i < VERTEX_NUM;)
	{
		mesh->m_texcoords[i++] = Vector2(0.0f, 1.0f);
		mesh->m_texcoords[i++] = Vector2(0.0f, 0.0f);
		mesh->m_texcoords[i++] = Vector2(1.0f, 0.0f);
		mesh->m_texcoords[i++] = Vector2(0.0f, 1.0f);
		mesh->m_texcoords[i++] = Vector2(1.0f, 0.0f);
		mesh->m_texcoords[i++] = Vector2(1.0f, 1.0f);
	}

	return mesh;
}

CMeshBuffer* CMeshCube::CreateBuffer()
{
	CMeshBuffer* buffer = new CMeshBuffer;
}