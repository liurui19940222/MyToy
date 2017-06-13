#include "3DSFile.h"


C3DSFile::C3DSFile()
{
}


C3DSFile::~C3DSFile()
{
}

void C3DSFile::LoadFromFile(const char* filename)
{
	lib3dsfile = lib3ds_file_load(filename);
	triangleNum = lib3dsfile->meshes->faces;
	vertexNum = triangleNum * 3;
	triangleArray = (STriangle*)malloc(triangleNum * sizeof(STriangle));
	normalArray = (Vector3*)malloc(triangleNum * 3 * sizeof(Vector3));
	uvArray = (Vector2*)malloc(vertexNum * sizeof(Vector2));
	Lib3dsPoint* points = lib3dsfile->meshes->pointL;
	Lib3dsTexel* texels = lib3dsfile->meshes->texelL;
	Lib3dsFace* face = NULL;
	for (int i = 0, index = 0; i < triangleNum; i++)
	{
		face = &lib3dsfile->meshes->faceL[i];
		memcpy(&triangleArray[i].verties[0], &(points[face->points[0]].pos), sizeof(Vector3));
		memcpy(&triangleArray[i].verties[1], &(points[face->points[1]].pos), sizeof(Vector3));
		memcpy(&triangleArray[i].verties[2], &(points[face->points[2]].pos), sizeof(Vector3));
		memcpy(&uvArray[i * 3 + 0], &(texels[face->points[0]]), sizeof(Vector2));
		memcpy(&uvArray[i * 3 + 1], &(texels[face->points[1]]), sizeof(Vector2));
		memcpy(&uvArray[i * 3 + 2], &(texels[face->points[2]]), sizeof(Vector2));
		for (int j = 0; j < 3; j++)
		{
			float temp = triangleArray[i].verties[j].y;
			triangleArray[i].verties[j].y = triangleArray[i].verties[j].z;
			triangleArray[i].verties[j].z = temp;
		}

		memcpy(&normalArray[index++], &face->normal, sizeof(Vector3));
		memcpy(&normalArray[index++], &face->normal, sizeof(Vector3));
		memcpy(&normalArray[index++], &face->normal, sizeof(Vector3));
	}
	vertexArray = (Vector3*)triangleArray;
	lib3ds_file_free(lib3dsfile);
	lib3dsfile = NULL;
}
