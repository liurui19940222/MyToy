#include "MeshQuad.h"

#ifndef GL_QUADS
#define GL_QUADS                          0x0007
#endif

IMPL_CLASS(CMeshQuad)

void CMeshQuad::OnInitialize()
{
	normals[0] = Vector3(0, 0, 1);
	normals[1] = Vector3(0, 0, 1);
	vertices[0] = Vector3(-0.5f, 0.5f, 0.0f);
	vertices[1] = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[2] = Vector3(0.5f, -0.5f, 0.0f);
	vertices[3] = Vector3(0.5f, -0.5f, 0.0f);
	vertices[4] = Vector3(0.5f, 0.5f, 0.0f);
	vertices[5] = Vector3(-0.5f, 0.5f, 0.0f);
	uvs[0] = Vector2(0.0f, 1.0f);
	uvs[1] = Vector2(0.0f, 0.0f);
	uvs[2] = Vector2(1.0f, 0.0f);
	uvs[3] = Vector2(1.0f, 0.0f);
	uvs[4] = Vector2(1.0f, 1.0f);
	uvs[5] = Vector2(0.0f, 1.0f);

	m_buffer.MakeBuffer(vertices, NULL, NULL, uvs, VERTEX_NUM);
}

Vector3* CMeshQuad::GetVertexArray() { return vertices; }

Vector3* CMeshQuad::GetNormalArray() { return normals; }

Vector2* CMeshQuad::GetUVArray() { return uvs; }

int CMeshQuad::GetVertexNum() { return VERTEX_NUM; }

int CMeshQuad::GetTriangleNum() { return VERTEX_NUM / 3; }