#include "MeshCube.h"

#ifndef GL_QUADS
#define GL_QUADS                          0x0007
#endif

IMPL_CLASS(CMeshCube)

void CMeshCube::OnInitialize()
{
	/*
	//front
	vertices[0] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[1] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[2] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[3] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[4] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[5] = Vector3(0.5f, 0.5f, -0.5f);

	//back
	vertices[6] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[7] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[8] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[9] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[10] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[11] = Vector3(-0.5f, 0.5f, 0.5f);

	//left
	vertices[12] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[13] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[14] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[15] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[16] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[17] = Vector3(-0.5f, 0.5f, -0.5f);

	//right
	vertices[18] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[19] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[20] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[21] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[22] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[23] = Vector3(0.5f, 0.5f, 0.5f);

	//top
	vertices[24] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[25] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[26] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[27] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[28] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[29] = Vector3(0.5f, 0.5f, 0.5f);

	//bottom
	vertices[30] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[31] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[32] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[33] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[34] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[35] = Vector3(0.5f, -0.5f, -0.5f);
	*/
	for (int i = 0; i < VERTEX_NUM;)
	{
		uvs[i++] = Vector2(0.0f, 1.0f);
		uvs[i++] = Vector2(0.0f, 0.0f);
		uvs[i++] = Vector2(1.0f, 0.0f);
		uvs[i++] = Vector2(0.0f, 1.0f);
		uvs[i++] = Vector2(1.0f, 0.0f);
		uvs[i++] = Vector2(1.0f, 1.0f);
	}
	m_buffer.MakeBuffer(vertices, NULL, NULL, uvs, VERTEX_NUM);
}

Vector3* CMeshCube::GetVertexArray() { return vertices; }

Vector3* CMeshCube::GetNormalArray() { return normals; }

Vector2* CMeshCube::GetUVArray() { return uvs; }

int CMeshCube::GetVertexNum() { return VERTEX_NUM; }

int CMeshCube::GetTriangleNum() { return VERTEX_NUM / 3; }
