#include "AdvModelLoader.h"

USING_NAMESPACE_ENGINE;

void copyMatrix(aiMatrix4x4& m1, Matrix4x4& m2)
{
	memcpy_s(&m2, sizeof(Matrix4x4), &m1, sizeof(aiMatrix4x4));
}

void AdvModelLoader::LoadFromFile(const char* filename)
{
	const aiScene* scene = m_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (scene == NULL)
		return;

	ReadMesh(scene);
	ReadSkeleton(scene);
	ReadAnimation(scene);
}

void AdvModelLoader::ReleaseSource()
{

}

void AdvModelLoader::ReadMesh(const aiScene* scene)
{
	//读取mesh
	if (!scene->HasMeshes())
		return;

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

		//复制骨骼
		if (aiMesh->HasBones())
		{
			aiBone* bone = NULL;
			vector<int> _records;
			_records.resize(vertexNum, 0);
			int vertexId = 0;
			int compIndex = 0;
			mesh->m_JointWeights = (Vector4*)malloc(sizeof(Vector4) * vertexNum);
			mesh->m_JointIndices = (BVector4*)malloc(sizeof(BVector4) * vertexNum);
			m_model->m_Skeleton = make_shared<Skeleton>();
			for (int i = 0; i < aiMesh->mNumBones; ++i)
			{
				bone = aiMesh->mBones[i];
				Joint joint;
				joint.m_Index = i;
				joint.m_Name = bone->mName.C_Str();
				copyMatrix(bone->mOffsetMatrix, joint.m_InvBindPose);
				m_model->m_Skeleton->AddJoint(joint);
				for (int j = 0; j < bone->mNumWeights; ++j)
				{
					vertexId = bone->mWeights[j].mVertexId;
					compIndex = _records[vertexId];
					assert(compIndex < 4);
					mesh->m_JointWeights[vertexId][compIndex] = bone->mWeights[j].mWeight;
					mesh->m_JointIndices[vertexId][compIndex] = i;
					_records[vertexId]++;
				}
			}
		}
	}
}

void AdvModelLoader::ReadSkeleton(const aiScene* scene)
{
	if (!scene->mRootNode)
		return;

}

void AdvModelLoader::ReadAnimation(const aiScene* scene)
{

}