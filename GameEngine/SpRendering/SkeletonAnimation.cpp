#include"SkeletonAnimation.h"

USING_NAMESPACE_ENGINE

Mesh::Mesh(const Mesh& mesh)
{
	m_VertexCount = mesh.m_VertexCount;
	m_TriangleCount = mesh.m_TriangleCount;
	if (mesh.m_Vertices)
	{
		m_Vertices = (Vector3*)malloc(sizeof(Vector3) * mesh.m_VertexCount);
		memcpy(m_Vertices, mesh.m_Vertices, sizeof(Vector3) * mesh.m_VertexCount);
	}
	if (mesh.m_Normals)
	{
		m_Normals = (Vector3*)malloc(sizeof(Vector3) * mesh.m_VertexCount);
		memcpy(m_Normals, mesh.m_Normals, sizeof(Vector3) * mesh.m_VertexCount);
	}
	if (mesh.m_Texcoords)
	{
		m_Texcoords = (Vector2*)malloc(sizeof(Vector2) * mesh.m_VertexCount);
		memcpy(m_Texcoords, mesh.m_Texcoords, sizeof(Vector2) * mesh.m_VertexCount);
	}
	if (mesh.m_Colors)
	{
		m_Colors = (Color*)malloc(sizeof(Color) * mesh.m_VertexCount);
		memcpy(m_Colors, mesh.m_Colors, sizeof(Color) * mesh.m_VertexCount);
	}
	if (mesh.m_Indices)
	{
		m_Indices = (ushort*)malloc(sizeof(ushort) * mesh.m_TriangleCount * 3);
		memcpy(m_Indices, mesh.m_Indices, sizeof(ushort) * mesh.m_TriangleCount * 3);
	}
	if (mesh.m_JointWeights)
	{
		m_JointWeights = (Vector4*)malloc(sizeof(Vector4) * mesh.m_VertexCount);
		memcpy(m_JointWeights, mesh.m_JointWeights, sizeof(Vector4) * mesh.m_VertexCount);
	}
	if (mesh.m_JointIndices)
	{
		m_JointIndices = (BVector4*)malloc(sizeof(BVector4) * mesh.m_VertexCount);
		memcpy(m_JointIndices, mesh.m_JointIndices, sizeof(BVector4) * mesh.m_VertexCount);
	}
}

Mesh::~Mesh()
{
	if (m_Vertices) { free(m_Vertices); m_Vertices = NULL; }
	if (m_Normals) { free(m_Normals); m_Normals = NULL; }
	if (m_Texcoords) { free(m_Texcoords); m_Texcoords = NULL; }
	if (m_Colors) { free(m_Colors); m_Colors = NULL; }
	if (m_Indices) { free(m_Indices); m_Indices = NULL; }
	if (m_JointWeights) { free(m_JointWeights); m_JointWeights = NULL; }
	if (m_JointIndices) { free(m_JointIndices); m_JointIndices = NULL; }
}

vector<JointPose> SkeletonAnimation::Sample(PAnimationClip clip, PSkeleton skeleton, float t, float weight)
{
	vector<JointPose> poses;
	t = clip->m_IsLooping ? fmod(t, clip->m_Length) : t;
	if (t > clip->m_Length || t < 0 || clip->m_aSamples.empty())
		return poses;
	poses.resize(skeleton->GetSize());
	for (uint i = 0; i < clip->m_aSamples.size() - 1; i++)
	{
		AnimationSample& a = clip->m_aSamples[i];
		AnimationSample& b = clip->m_aSamples[i + 1];

		if (a.m_Time <= t && b.m_Time >= t)
		{
			Matrix4x4 mat_a;
			Matrix4x4 mat_b;
			for (byte i = 0; i < skeleton->GetSize(); i++)
			{
				auto it_a = a.m_JointPoses.find(i);
				auto it_b = b.m_JointPoses.find(i);
				if (it_a != a.m_JointPoses.end())
					mat_a = it_a->second.m_Matrix;
				else
					mat_a = skeleton->GetJoint(i)->m_LocalMatrix;
				if (it_b != b.m_JointPoses.end())
					mat_b = it_b->second.m_Matrix;
				else
					mat_b = skeleton->GetJoint(i)->m_LocalMatrix;

				poses[i] = JointPose{ Matrix4x4::Lerp(mat_a, mat_b, (t - a.m_Time) / (b.m_Time - a.m_Time)) * weight };
			}
		}
	}
	return poses;
}

vector<JointPose> SkeletonAnimation::FullMatchSample(PAnimationClip clip, PSkeleton skeleton, float t, float weight)
{
	vector<JointPose> poses;
	t = clip->m_IsLooping ? fmod(t, clip->m_Length) : t;
	if (t > clip->m_Length || t < 0 || clip->m_aSamples.empty())
		return poses;
	poses.resize(skeleton->GetSize());
	for (byte joint = 0, index = 0; joint < skeleton->GetSize(); joint++)
	{
		AnimationSample* a = NULL;
		AnimationSample* b = NULL;
		for (uint i = 0; i < clip->m_aSamples.size() && (!a || !b); i++)
		{
			index = (byte)(clip->m_aSamples.size() - i - 1);
			if (!a && clip->m_aSamples[index].m_Time <= t && clip->m_aSamples[index].m_JointPoses.find(joint) != clip->m_aSamples[index].m_JointPoses.end())
				a = &clip->m_aSamples[index];
			if (!b && clip->m_aSamples[i].m_Time >= t && clip->m_aSamples[i].m_JointPoses.find(joint) != clip->m_aSamples[i].m_JointPoses.end())
				b = &clip->m_aSamples[i];
		}
		Matrix4x4 mat_a = a ? a->m_JointPoses.find(joint)->second.m_Matrix : skeleton->GetJoint(joint)->m_LocalMatrix;
		Matrix4x4 mat_b = b ? b->m_JointPoses.find(joint)->second.m_Matrix : skeleton->GetJoint(joint)->m_LocalMatrix;
		float a_time = a ? a->m_Time : 0;
		float b_time = b ? b->m_Time : 1;
		poses[joint] = JointPose { Matrix4x4::Lerp(mat_a, mat_b, (t - a_time) / (b_time - a_time)) * weight };
	}
	return poses;
}

vector<JointPose> SkeletonAnimation::Blend(PAnimationClip* clips, float* timePos, float* weights, int count, PSkeleton skeleton)
{
	float total_weight = 0;
	for (int i = 0; i < count; ++i)
	{
		total_weight += weights[i];
	}
	total_weight = 1.0f / total_weight;
	vector<JointPose> jointPoses(skeleton->GetSize());
	for (int i = 0; i < count; ++i)
	{
		vector<JointPose> poses = Sample(clips[i], skeleton, timePos[i], weights[i] * total_weight);
		if(poses.size())
		{
			for (uint j = 0; j < poses.size(); j++)
			{
				jointPoses[j].m_Matrix += poses[j].m_Matrix;
			}
		}
	}
	return jointPoses;
}

void SkeletonAnimation::CalculateGlobalMatrix(PSkeleton skeleton)
{
	for (Joint& joint : skeleton->GetJoints())
	{
		Matrix4x4 matj = Matrix4x4::Identity();
		Joint* p = &joint;
		do {
			matj = p->m_LocalMatrix * matj;
			if (p->m_iParent == JOINT_ROOT)
				break;
			p = skeleton->GetJoint(p->m_iParent);
			if (!p)
				break;
		} while (true);
		skeleton->m_GlobalPoses[joint.m_Index] = matj;
	}
}

void SkeletonAnimation::CalculateGlobalMatrix(PSkeleton skeleton, vector<JointPose> localPoses)
{
	if (!localPoses.size())
		return;
	for (Joint& joint : skeleton->GetJoints())
	{
		Matrix4x4 matj = Matrix4x4::Identity();
		Joint* p = &joint;
		do {
			matj = localPoses[p->m_Index].m_Matrix * matj;
			if (p->m_iParent == JOINT_ROOT)
				break;
			p = skeleton->GetJoint(p->m_iParent);
		} while (true);
		skeleton->m_GlobalPoses[joint.m_Index] = matj;
	}
}

void SkeletonAnimation::CalculateSkinningMatrix(PSkeleton skeleton)
{
	Matrix4x4 mat;
	vector<Joint>& joints = skeleton->GetJoints();
	for (uint i = 0; i < joints.size(); ++i)
	{
		mat.MakeZero();
		mat += skeleton->m_GlobalPoses[i] * joints[i].m_InvBindPose * skeleton->m_BindShapeMat;
		skeleton->m_SkiningMatrices[i] = mat;
	}
}