#include"MeshProvider.h"

STriangle* CMeshProvider::GetTriangleArray() { return NULL; }

Vector3* CMeshProvider::GetNormalArray() { return NULL; }

Vector3* CMeshProvider::GetTangentArray() { return NULL; }

Vector2* CMeshProvider::GetUVArray() { return NULL; }

Color* CMeshProvider::GetColorArray() { return NULL; }

int CMeshProvider::GetGLMode() { return GL_TRIANGLES; }

void CMeshProvider::ReleaseMesh() { m_buffer.ReleaseBuffer(); }

CMeshBuffer* CMeshProvider::GetBuffer() { return &m_buffer; }