#include "MeshCube.h"

#ifndef GL_QUADS
#define GL_QUADS                          0x0007
#endif

IMPL_CLASS(CMeshCube)

void CMeshCube::OnInitialize()
{
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
