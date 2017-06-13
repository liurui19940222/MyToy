#include "MeshQuad.h"

#ifndef GL_QUADS
#define GL_QUADS                          0x0007
#endif

void CMeshQuad::OnInitialize()
{
	normals[0] = Vector3(0, 0, 1);
	normals[1] = Vector3(0, 0, 1);
	vertices[0] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[1] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[2] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[3] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[4] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[5] = Vector3(-0.5f, 0.5f, 0.5f);
}

Vector3* CMeshQuad::GetVertexArray() { return vertices; }

STriangle* CMeshQuad::GetTriangleArray() { return 0; }

Vector3* CMeshQuad::GetNormalArray() { return normals; }

Vector3* CMeshQuad::GetTangentArray() { return 0; }

Vector2* CMeshQuad::GetUVArray() { return 0; }

int CMeshQuad::GetVertexNum() { return 6; }

int CMeshQuad::GetTriangleNum() { return 2; }

int CMeshQuad::GetGLMode() { return GL_QUADS; }

void CMeshQuad::ReleaseMesh() {}