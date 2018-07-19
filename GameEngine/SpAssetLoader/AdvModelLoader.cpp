#include "AdvModelLoader.h"

USING_NAMESPACE_ENGINE;

void copyMatrix(aiMatrix4x4& m1, Matrix4x4& m2)
{
	memcpy_s(&m2, sizeof(Matrix4x4), &(m1.Transpose()), sizeof(aiMatrix4x4));
}

void copyVector3(aiVector3D& v1, Vector3& v2)
{
	v2.x = v1.x;
	v2.y = v1.y;
	v2.z = v1.z;
}

void copyQuaternion(aiQuaternion& q1, Quaternion& q2)
{
	q2.x = q1.x;
	q2.y = q1.y;
	q2.z = q1.z;
	q2.w = q1.w;
}

void AdvModelLoader::LoadFromFile(const char* filename)
{
	const aiScene* scene = m_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (scene == NULL)
		return;

	ReadMesh(scene);
	ReadSkeleton(scene);
	ReadAnimation(scene);
	SkeletonAnimation::CalculateGlobalMatrix(m_model->m_Skeleton);
	SkeletonAnimation::CalculateSkinningMatrix(m_model->m_Skeleton);
}

void AdvModelLoader::ReleaseSource()
{

}

void AdvModelLoader::ReadMesh(const aiScene* scene)
{
	//��ȡmesh
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

		//���ƶ���
		if (aiMesh->HasPositions())
		{
			size = sizeof(Vector3) * vertexNum;
			mesh->m_Vertices = (Vector3*)malloc(size);
			memcpy_s(mesh->m_Vertices, size, aiMesh->mVertices, size);
			mesh->m_VertexCount = vertexNum;
		}

		//��������
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

		//����UV
		if (aiMesh->HasTextureCoords(0))
		{
			mesh->m_Texcoords = (Vector2*)malloc(sizeof(Vector2) * vertexNum);
			for (int i = 0; i < aiMesh->mNumVertices; ++i)
			{
				mesh->m_Texcoords[i].x = aiMesh->mTextureCoords[0][i].x;
				mesh->m_Texcoords[i].y = aiMesh->mTextureCoords[0][i].y;
			}
		}

		//����Normals
		if (aiMesh->HasNormals())
		{
			size = sizeof(Vector3) * vertexNum;
			mesh->m_Normals = (Vector3*)malloc(size);
			memcpy_s(mesh->m_Normals, size, aiMesh->mNormals, size);
		}

		//���ƹ���
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
			m_model->m_Skeleton->m_GlobalPoses.resize(m_model->m_Skeleton->GetSize());
			m_model->m_Skeleton->m_LocalPoses.resize(m_model->m_Skeleton->GetSize());
			m_model->m_Skeleton->m_SkiningMatrices.resize(m_model->m_Skeleton->GetSize());
		}
	}
}

void AdvModelLoader::ReadSkeleton(const aiScene* scene)
{
	if (!scene->mRootNode)
		return;

	aiNode* rootNode = scene->mRootNode;
	copyMatrix(rootNode->mTransformation.Inverse(), m_model->m_Skeleton->m_BindShapeMat);
	ReadNode(rootNode, 0);
}

void AdvModelLoader::ReadNode(aiNode* node, int depth)
{
	if (node->mNumChildren == 0)
		return;
	aiNode* temp = NULL;
	Joint* joint = NULL;
	Joint* parent = NULL;
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		temp = node->mChildren[i];
		//�ӹǼ��еõ���ai�ڵ��Ӧ�Ĺؽ�
		joint = m_model->m_Skeleton->GetJoint(temp->mName.C_Str());
		if (joint)
		{
			//��ȡ�ֲ��任
			copyMatrix(temp->mTransformation, joint->m_LocalMatrix);
			if (temp->mParent)
			{
				//����ýڵ�ĸ��ڵ�ĸ��ڵ�Ϊ�գ�˵�����ڵ��Ǹ��ڵ�
				if (!temp->mParent->mParent)
				{
					joint->m_iParent = JOINT_ROOT;
				}
				else
				{
					parent = m_model->m_Skeleton->GetJoint(temp->mParent->mName.C_Str());
					if (parent)
						joint->m_iParent = parent->m_Index;
					else
						joint->m_iParent = JOINT_ROOT;
				}
			}
			else
			{
				//���۲����ߵ����ÿ���ڵ㶼����һ��Parent
				assert(0);
				joint->m_iParent = JOINT_ROOT;
			}
			//--------------------��ӡ�ؽ���----------------------
			/*string str;
			for (int i = 0; i < depth; i++)
			{
				str += "   ";
			}
			Debug::Log("%s p:%d i:%d", (str + joint->m_Name).c_str(), joint->m_iParent, joint->m_Index);*/
		}
		ReadNode(temp, depth + 1);
	}
}

void AdvModelLoader::ReadAnimation(const aiScene* scene)
{
	if (!scene->HasAnimations())
		return;
	aiAnimation* aiAnim = NULL;
	aiNodeAnim* aiNodeAnim = NULL;
	aiVectorKey aiVectorKey;
	PAnimationClip anim = NULL;
	const char* jointName = NULL;
	double reciprocalOfTicks = 0.0;

	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		anim = make_shared<AnimationClip>();
		aiAnim = scene->mAnimations[i];
		reciprocalOfTicks = 1.0 / aiAnim->mTicksPerSecond;
		anim->m_Length = (float)(aiAnim->mDuration * reciprocalOfTicks);
		anim->m_Name = aiAnim->mName.C_Str();

		//��ȡ���е�ͨ�������չؼ�֡λ�ö�Ӧÿ���ؽں�������������
		map<double, JointTransformation> samples;
		for (int j = 0; j < aiAnim->mNumChannels; ++j)
		{
			aiNodeAnim = aiAnim->mChannels[j];
			jointName = aiNodeAnim->mNodeName.C_Str();
			for (int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k)
			{
				AddPositionSample(samples, aiNodeAnim->mPositionKeys[k], jointName);
			}
			for (int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k)
			{
				AddRotationSample(samples, aiNodeAnim->mRotationKeys[k], jointName);
			}
			for (int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k)
			{
				AddScalingSample(samples, aiNodeAnim->mScalingKeys[k], jointName);
			}
		}
		//ת����AnimationSample
		for_each(samples.begin(), samples.end(), [&](pair<double, JointTransformation> pair) {
			AnimationSample sample;
			sample.m_Time = pair.first * reciprocalOfTicks;
			CopySampleInfo(pair.second, sample);
			anim->m_aSamples.push_back(sample);
		});
		m_model->m_Animations.push_back(anim);
	}
}

void AdvModelLoader::AddPositionSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, vectorKey.mTime, jointName, &transformation))
		return;
	transformation->position = vectorKey.mValue;
}

void AdvModelLoader::AddRotationSample(map<double, JointTransformation>& samples, aiQuatKey& quatKey, const char* jointName)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, quatKey.mTime, jointName, &transformation))
		return;
	transformation->rotation = quatKey.mValue;
}

void AdvModelLoader::AddScalingSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, vectorKey.mTime, jointName, &transformation))
		return;
	transformation->scaling = vectorKey.mValue;
}

bool AdvModelLoader::GetTransformation(map<double, JointTransformation>& samples, double time, const char* jointName, Transformation** trans)
{
	Joint* joint = m_model->m_Skeleton->GetJoint(jointName);
	if (!joint)
		return false;
	auto it = samples.find(time);
	if (it == samples.end())
	{
		samples.insert(make_pair(time, JointTransformation()));
	}
	JointTransformation& jtrans = samples[time];
	auto jit = jtrans.transformations.find(joint->m_Index);
	if (jit == jtrans.transformations.end())
	{
		jtrans.transformations.insert(make_pair(joint->m_Index, Transformation()));
	}
	*trans = &jtrans.transformations[joint->m_Index];
	return true;
}

void AdvModelLoader::CopySampleInfo(JointTransformation& from, AnimationSample& to)
{
	for_each(from.transformations.begin(), from.transformations.end(), [&](pair<byte, Transformation> pair) {
		aiMatrix4x4 mat(pair.second.scaling, pair.second.rotation, pair.second.position);
		JointPose pose;
		copyMatrix(mat, pose.m_Matrix);
		to.m_JointPoses.insert(make_pair(pair.first, pose));
	});
}