#include "ModelFile.h"

#ifndef GL_TRIANGLES
#define GL_TRIANGLES                      0x0004
#endif

Vector3* CModelFile::GetVertexArray()
{
	return m_vertexArray;
}

STriangle* CModelFile::GetTriangleArray()
{
	return m_triangleArray;
}

Vector3* CModelFile::GetNormalArray()
{
	return m_normalArray;
}

Vector3* CModelFile::GetTangentArray()
{
	return m_tangentArray;
}

Vector2* CModelFile::GetUVArray()
{
	return m_uvArray;
}

int CModelFile::GetVertexNum()
{
	return m_vertexNum;
}

int CModelFile::GetTriangleNum()
{
	return m_triangleNum;
}

int CModelFile::GetGLMode()
{
	return GL_TRIANGLES;
}

void CModelFile::ReleaseSource()
{
	if (this->m_vertexArray)
		free(m_vertexArray);
	if (this->m_triangleArray)
		free(m_triangleArray);
	if (this->m_normalArray)
		free(m_normalArray);
	if (this->m_uvArray)
		free(m_uvArray);
	if (this->m_tangentArray)
		free(m_triangleArray);
}

void CModelFile::ReleaseMesh() 
{
	CMeshProvider::ReleaseMesh();
}
