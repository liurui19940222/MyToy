#include "MeshCube.h"

#ifndef GL_QUADS
#define GL_QUADS                          0x0007
#endif

IMPL_CLASS(CMeshCube)

void CMeshCube::OnInitialize()
{
	//front
	normals[0] = Vector3(0, 0, 1);
	vertices[0] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[1] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[2] = Vector3(0.5f, -0.5f, 0.5f);
	normals[1] = Vector3(0, 0, 1);
	vertices[3] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[4] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[5] = Vector3(-0.5f, 0.5f, 0.5f);

	//back
	normals[2] = Vector3(0, 0, -1);
	vertices[6] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[7] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[8] = Vector3(-0.5f, -0.5f, -0.5f);
	normals[3] = Vector3(0, 0, 1);
	vertices[9] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[10] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[11] = Vector3(0.5f, 0.5f, -0.5f);

	//left
	normals[4] = Vector3(-1, 0, 0);
	vertices[12] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[13] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[14] = Vector3(-0.5f, -0.5f, -0.5f);
	normals[5] = Vector3(-1, 0, 0);
	vertices[15] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[16] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[17] = Vector3(-0.5f, 0.5f, 0.5f);

	//right
	normals[6] = Vector3(1, 0, 0);
	vertices[18] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[19] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[20] = Vector3(0.5f, -0.5f, 0.5f);
	normals[7] = Vector3(1, 0, 0);
	vertices[21] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[22] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[23] = Vector3(0.5f, 0.5f, -0.5f);

	//top
	normals[8] = Vector3(0, 1, 0);
	vertices[24] = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[25] = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[26] = Vector3(0.5f, 0.5f, -0.5f);
	normals[9] = Vector3(0, 1, 0);
	vertices[27] = Vector3(0.5f, 0.5f, -0.5f);
	vertices[28] = Vector3(0.5f, 0.5f, 0.5f);
	vertices[29] = Vector3(-0.5f, 0.5f, 0.5f);

	//bottom
	normals[10] = Vector3(0, -1, 0);
	vertices[30] = Vector3(0.5f, -0.5f, 0.5f);
	vertices[31] = Vector3(0.5f, -0.5f, -0.5f);
	vertices[32] = Vector3(-0.5f, -0.5f, -0.5f);
	normals[11] = Vector3(0, -1, 0);
	vertices[33] = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[34] = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[35] = Vector3(0.5f, -0.5f, 0.5f);
}

Vector3* CMeshCube::GetVertexArray() { return vertices; }

STriangle* CMeshCube::GetTriangleArray() { return 0; }

Vector3* CMeshCube::GetNormalArray() { return normals; }

Vector3* CMeshCube::GetTangentArray() { return 0; }

Vector2* CMeshCube::GetUVArray() { return 0; }

int CMeshCube::GetVertexNum() { return 36; }

int CMeshCube::GetTriangleNum() { return 36; }

int CMeshCube::GetGLMode() { return GL_QUADS; }

void CMeshCube::ReleaseMesh() {}