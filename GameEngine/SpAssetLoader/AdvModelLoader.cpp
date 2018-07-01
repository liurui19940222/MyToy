#include "AdvModelLoader.h"

USING_NAMESPACE_ENGINE;

void AdvModelLoader::LoadFromFile(const char* filename)
{
	const aiScene* scene = m_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (scene == NULL)
		return;
	//读取mesh
	if (scene->HasMeshes())
	{
		m_model->m_Meshes.resize(scene->mNumMeshes);
		Mesh* mesh = NULL;
		aiMesh* aiMesh = NULL;
		uint size = 0;
		uint vertexNum = 0;
		for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
		{
			m_model->m_Meshes[meshIdx] = make_shared<Mesh>();
			mesh = m_model->m_Meshes[meshIdx].get();
			aiMesh = scene->mMeshes[meshIdx];
			vertexNum = aiMesh->mNumVertices;

			//复制顶点
			if (aiMesh->HasPositions())
			{
				size = sizeof(Vector3) * vertexNum;
				mesh->m_Vertices = (Vector3*)malloc(size);
				memcpy_s(mesh->m_Vertices, size, aiMesh->mVertices, size);
				mesh->m_VertexCount = vertexNum;
			}

			//复制索引
			if (aiMesh->HasFaces())
			{
				mesh->m_Indices = (ushort*)malloc(sizeof(ushort) * aiMesh->mNumFaces * 3);
				mesh->m_TriangleCount = aiMesh->mNumFaces;
				for (int i = 0; i < aiMesh->mNumFaces; ++i)
				{
					mesh->m_Indices[i * 3 + 0] = aiMesh->mFaces[i].mIndices[0];
					mesh->m_Indices[i * 3 + 1] = aiMesh->mFaces[i].mIndices[1];
					mesh->m_Indices[i * 3 + 2] = aiMesh->mFaces[i].mIndices[2];
				}
			}

			//复制UV
			if (aiMesh->HasTextureCoords(0))
			{
				mesh->m_Texcoords = (Vector2*)malloc(sizeof(Vector2) * vertexNum);
				for (int i = 0; i < aiMesh->mNumVertices; ++i)
				{
					mesh->m_Texcoords[i].x = aiMesh->mTextureCoords[0][i].x;
					mesh->m_Texcoords[i].y = aiMesh->mTextureCoords[0][i].y;
				}
			}

			//复制Normals
			if (aiMesh->HasNormals())
			{
				size = sizeof(Vector3) * vertexNum;
				mesh->m_Normals = (Vector3*)malloc(size);
				memcpy_s(mesh->m_Normals, size, aiMesh->mNormals, size);
			}
		}
	}
}

void AdvModelLoader::ReleaseSource()
{
	
}