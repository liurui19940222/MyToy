#include"3DSModelLoader.h"

void C3DSModelLoader::LoadFromFile(const char* filename)
{
	m_model = new Model;
	lib3dsfile = lib3ds_file_load(filename);
	int m_triangleNum = lib3dsfile->meshes->faces;
	int m_vertexNum = m_triangleNum * 3;
	STriangle* m_triangleArray = (STriangle*)malloc(m_triangleNum * sizeof(STriangle));
	Vector3* m_normalArray = (Vector3*)malloc(m_triangleNum * 3 * sizeof(Vector3));
	Vector2* m_uvArray = (Vector2*)malloc(m_vertexNum * sizeof(Vector2));
	Lib3dsPoint* points = lib3dsfile->meshes->pointL;
	Lib3dsTexel* texels = lib3dsfile->meshes->texelL;
	Lib3dsFace* face = NULL;
	Vector3* normalBuffer = (Vector3*)malloc(lib3dsfile->meshes->points * sizeof(Vector3));
	memset(normalBuffer, 0, lib3dsfile->meshes->points * sizeof(Vector3));
	for (int i = 0, index = 0; i < m_triangleNum; i++)
	{
		face = &lib3dsfile->meshes->faceL[i];
		memcpy(&m_triangleArray[i].verties[0], &(points[face->points[0]].pos), sizeof(Vector3));
		memcpy(&m_triangleArray[i].verties[1], &(points[face->points[1]].pos), sizeof(Vector3));
		memcpy(&m_triangleArray[i].verties[2], &(points[face->points[2]].pos), sizeof(Vector3));
		memcpy(&m_uvArray[i * 3 + 0], &(texels[face->points[0]]), sizeof(Vector2));
		memcpy(&m_uvArray[i * 3 + 1], &(texels[face->points[1]]), sizeof(Vector2));
		memcpy(&m_uvArray[i * 3 + 2], &(texels[face->points[2]]), sizeof(Vector2));
		for (int j = 0; j < 3; j++)
		{
			float temp = m_triangleArray[i].verties[j].y;
			m_triangleArray[i].verties[j].y = m_triangleArray[i].verties[j].z;
			m_triangleArray[i].verties[j].z = temp;
		}

		Vector3 normal = Vector3::Cross(m_triangleArray[i].verties[1] - m_triangleArray[i].verties[0], m_triangleArray[i].verties[2] - m_triangleArray[i].verties[0]).Normalize();

		normalBuffer[face->points[0]] += normal;
		normalBuffer[face->points[1]] += normal;
		normalBuffer[face->points[2]] += normal;
	}
	for (size_t i = 0; i < lib3dsfile->meshes->points; i++)
	{
		normalBuffer[i] = normalBuffer[i].Normalize();
	}
	for (int i = 0, index = 0; i < m_triangleNum; i++)
	{
		face = &lib3dsfile->meshes->faceL[i];

		memcpy(&m_normalArray[index++], &(normalBuffer[face->points[0]]), sizeof(Vector3));
		memcpy(&m_normalArray[index++], &(normalBuffer[face->points[1]]), sizeof(Vector3));
		memcpy(&m_normalArray[index++], &(normalBuffer[face->points[2]]), sizeof(Vector3));
	}
	m_model->m_meshCount = 1;
	m_model->m_meshes = new Mesh[1];
	m_model->m_meshes[0].m_vertices = (Vector3*)m_triangleArray;
	m_model->m_meshes[0].m_normals = m_normalArray;
	m_model->m_meshes[0].m_texcoords = m_uvArray;
	m_model->m_meshes[0].m_vertexCount = m_vertexNum;
}

void C3DSModelLoader::ReleaseSource()
{
	lib3ds_file_free(lib3dsfile);
	lib3dsfile = NULL;
}