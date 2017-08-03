#include"MeshProvider.h"

STriangle* MeshProvider::GetTriangleArray() { return NULL; }

Vector3* MeshProvider::GetNormalArray() { return NULL; }

Vector3* MeshProvider::GetTangentArray() { return NULL; }

Vector2* MeshProvider::GetUVArray() { return NULL; }

Color* MeshProvider::GetColorArray() { return NULL; }

int MeshProvider::GetGLMode() { return GL_TRIANGLES; }

void MeshProvider::ReleaseMesh() { m_buffer.ReleaseBuffer(); }

CMeshBuffer* MeshProvider::GetBuffer() { return &m_buffer; }