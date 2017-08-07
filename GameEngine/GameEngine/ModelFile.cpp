#include "ModelFile.h"

#ifndef GL_TRIANGLES
#define GL_TRIANGLES                      0x0004
#endif

Vector3* CModelFile::GetVertexArray()
{
	return vertexArray;
}

STriangle* CModelFile::GetTriangleArray()
{
	return triangleArray;
}

Vector3* CModelFile::GetNormalArray()
{
	return normalArray;
}

Vector3* CModelFile::GetTangentArray()
{
	return tangentArray;
}

Vector2* CModelFile::GetUVArray()
{
	return uvArray;
}

int CModelFile::GetVertexNum()
{
	return vertexNum;
}

int CModelFile::GetTriangleNum()
{
	return triangleNum;
}

int CModelFile::GetGLMode()
{
	return GL_TRIANGLES;
}

void CModelFile::ReleaseSource()
{
	if (this->vertexArray)
		free(vertexArray);
	if (this->triangleArray)
		free(triangleArray);
	if (this->normalArray)
		free(normalArray);
	if (this->uvArray)
		free(uvArray);
	if (this->tangentArray)
		free(triangleArray);
}

void CModelFile::ReleaseMesh() 
{
	CMeshProvider::ReleaseMesh();
}
