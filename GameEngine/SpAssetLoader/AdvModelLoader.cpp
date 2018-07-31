#include "AdvModelLoader.h"
#include<assert.h>

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

PModel AdvModelLoader::LoadFromFile(const char* filename)
{
	PModel model = make_shared<Model>();
	const aiScene* scene = m_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (scene == NULL)
		return model;

	ReadMesh(scene, model.get());
	ReadSkeleton(scene, model.get());
	if (model->m_Skeleton.get())
	{
		ReadAnimation(scene, model.get(), *model->m_Skeleton.get());
		SkeletonAnimation::CalculateGlobalMatrix(model->m_Skeleton);
		SkeletonAnimation::CalculateSkinningMatrix(model->m_Skeleton);
	}
	return model;
}

PModel AdvModelLoader::LoadAnimationFromFile(const char* filename, Skeleton& skeleton)
{
	PModel model = make_shared<Model>();
	const aiScene* scene = m_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (scene == NULL)
		return model;

	ReadAnimation(scene, model.get(), skeleton);
	return model;
}

void AdvModelLoader::ReleaseSource()
{

}

void AdvModelLoader::ReadMesh(const aiScene* scene, Model* model)
{
	//读取mesh
	if (!scene->HasMeshes())
		return;

	model->m_Meshes.resize(scene->mNumMeshes);
	Mesh* mesh = NULL;
	aiMesh* aiMesh = NULL;
	uint size = 0;
	uint vertexNum = 0;
	for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		model->m_Meshes[meshIdx] = make_shared<Mesh>();
		mesh = model->m_Meshes[meshIdx].get();
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
			model->m_Skeleton = make_shared<Skeleton>();
			for (int i = 0; i < aiMesh->mNumBones; ++i)
			{
				bone = aiMesh->mBones[i];
				Joint joint;
				joint.m_Index = i;
				joint.m_Name = bone->mName.C_Str();
				copyMatrix(bone->mOffsetMatrix, joint.m_InvBindPose);
				model->m_Skeleton->AddJoint(joint);
				for (int j = 0; j < bone->mNumWeights; ++j)
				{
					vertexId = bone->mWeights[j].mVertexId;
					compIndex = _records[vertexId];
					if (compIndex < 4)
					{
						mesh->m_JointWeights[vertexId][compIndex] = bone->mWeights[j].mWeight;
						mesh->m_JointIndices[vertexId][compIndex] = i;
						_records[vertexId]++;
					}
				}
			}
			model->m_Skeleton->m_GlobalPoses.resize(model->m_Skeleton->GetSize());
			model->m_Skeleton->m_LocalPoses.resize(model->m_Skeleton->GetSize());
			model->m_Skeleton->m_SkiningMatrices.resize(model->m_Skeleton->GetSize());
		}
	}
}

void AdvModelLoader::ReadSkeleton(const aiScene* scene, Model* model)
{
	if (!scene->mRootNode || !model || !model->m_Skeleton.get())
		return;

	aiNode* rootNode = scene->mRootNode;
	copyMatrix(rootNode->mTransformation.Inverse(), model->m_Skeleton->m_BindShapeMat);
	ReadNode(rootNode, 0, model);
}

void AdvModelLoader::ReadNode(aiNode* node, int depth, Model* model)
{
	if (node->mNumChildren == 0)
		return;
	aiNode* temp = NULL;
	Joint* joint = NULL;
	Joint* parent = NULL;
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		temp = node->mChildren[i];
		//从骨架中得到该ai节点对应的关节
		joint = model->m_Skeleton->GetJoint(temp->mName.C_Str());
		if (joint)
		{
			//读取局部变换
			copyMatrix(temp->mTransformation, joint->m_LocalMatrix);
			if (temp->mParent)
			{
				//如果该节点的父节点的父节点为空，说明父节点是根节点
				if (!temp->mParent->mParent)
				{
					joint->m_iParent = JOINT_ROOT;
				}
				else
				{
					parent = model->m_Skeleton->GetJoint(temp->mParent->mName.C_Str());
					if (parent)
						joint->m_iParent = parent->m_Index;
					else
						joint->m_iParent = JOINT_ROOT;
				}
			}
			else
			{
				//理论不会走到这里，每个节点都会有一个Parent
				assert(0);
				joint->m_iParent = JOINT_ROOT;
			}
			//--------------------打印关节树----------------------
			/*string str;
			for (int i = 0; i < depth; i++)
			{
				str += "   ";
			}
			Debug::Log("%s p:%d i:%d", (str + joint->m_Name).c_str(), joint->m_iParent, joint->m_Index);*/
		}
		ReadNode(temp, depth + 1, model);
	}
}

void AdvModelLoader::ReadAnimation(const aiScene* scene, Model* model, Skeleton& skeleton)
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

		//读取所有的通道，按照关键帧位置对应每个关节和姿势整合起来
		map<double, JointTransformation> samples;
		for (int j = 0; j < aiAnim->mNumChannels; ++j)
		{
			aiNodeAnim = aiAnim->mChannels[j];
			jointName = aiNodeAnim->mNodeName.C_Str();
			for (int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k)
			{
				AddPositionSample(samples, aiNodeAnim->mPositionKeys[k], jointName, skeleton);
			}
			for (int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k)
			{
				AddRotationSample(samples, aiNodeAnim->mRotationKeys[k], jointName, skeleton);
			}
			for (int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k)
			{
				AddScalingSample(samples, aiNodeAnim->mScalingKeys[k], jointName, skeleton);
			}
		}
		//转换成AnimationSample
		for_each(samples.begin(), samples.end(), [&](pair<double, JointTransformation> pair) {
			AnimationSample sample;
			sample.m_Time = pair.first * reciprocalOfTicks;
			CopySampleInfo(pair.second, sample);
			anim->m_aSamples.push_back(sample);
		});
		model->m_Animations.push_back(anim);
	}
}

void AdvModelLoader::AddPositionSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName, Skeleton& skeleton)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, vectorKey.mTime, jointName, &transformation, skeleton))
		return;
	transformation->position = vectorKey.mValue;
}

void AdvModelLoader::AddRotationSample(map<double, JointTransformation>& samples, aiQuatKey& quatKey, const char* jointName, Skeleton& skeleton)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, quatKey.mTime, jointName, &transformation, skeleton))
		return;
	transformation->rotation = quatKey.mValue;
}

void AdvModelLoader::AddScalingSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName, Skeleton& skeleton)
{
	Transformation* transformation = NULL;
	if (!GetTransformation(samples, vectorKey.mTime, jointName, &transformation, skeleton))
		return;
	transformation->scaling = vectorKey.mValue;
}

bool AdvModelLoader::GetTransformation(map<double, JointTransformation>& samples, double time, const char* jointName, Transformation** trans, Skeleton& skeleton)
{
	Joint* joint = skeleton.GetJoint(jointName);
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