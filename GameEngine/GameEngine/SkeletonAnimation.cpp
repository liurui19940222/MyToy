#include"SkeletonAnimation.h"

Mesh::Mesh(const Mesh& mesh)
{
	if (mesh.m_vertices)
	{
		m_vertices = (Vector3*)malloc(sizeof(Vector3) * mesh.m_vertexCount);
		memcpy(m_vertices, mesh.m_vertices, sizeof(Vector3) * mesh.m_vertexCount);
	}
	if (mesh.m_normals)
	{
		m_normals = (Vector3*)malloc(sizeof(Vector3) * mesh.m_vertexCount);
		memcpy(m_normals, mesh.m_normals, sizeof(Vector3) * mesh.m_vertexCount);
	}
	if (mesh.m_texcoords)
	{
		m_texcoords = (Vector2*)malloc(sizeof(Vector2) * mesh.m_vertexCount);
		memcpy(m_texcoords, mesh.m_texcoords, sizeof(Vector2) * mesh.m_vertexCount);
	}
	if (mesh.m_colors)
	{
		m_colors = (Color*)malloc(sizeof(Color) * mesh.m_vertexCount);
		memcpy(m_colors, mesh.m_colors, sizeof(Color) * mesh.m_vertexCount);
	}
}

Mesh::~Mesh()
{
	if (m_vertices) free(m_vertices);
	if (m_normals) free(m_normals);
	if (m_texcoords) free(m_texcoords);
	if (m_colors) free(m_colors);
}

vector<JointPose> CSkeletonAnimation::Sample(AnimationClip& clip, Skeleton& skeleton, float t, float weight)
{
	vector<JointPose> poses;
	t = clip.m_isLooping ? fmod(t, clip.m_length) : t;
	if (t > clip.m_length || t < 0 || clip.m_aSamples.size() == 0)
		return poses;
	poses.resize(skeleton.GetSize());
	for (int i = 0; i < clip.m_aSamples.size() - 1; i++)
	{
		AnimationSample& a = clip.m_aSamples[i];
		AnimationSample& b = clip.m_aSamples[i + 1];

		if (a.m_time <= t && b.m_time >= t)
		{
			Matrix4x4 mat_a;
			Matrix4x4 mat_b;
			for (byte i = 0; i < skeleton.GetSize(); i++)
			{
				auto it_a = a.m_jointPoses.find(i);
				auto it_b = b.m_jointPoses.find(i);
				if (it_a != a.m_jointPoses.end())
					mat_a = it_a->second.m_matrix;
				else
					mat_a = skeleton.GetJoint(i)->m_localMatrix;
				if (it_b != b.m_jointPoses.end())
					mat_b = it_b->second.m_matrix;
				else
					mat_b = skeleton.GetJoint(i)->m_localMatrix;

				poses[i] = JointPose{ Matrix4x4::Lerp(mat_a, mat_b, (t - a.m_time) / (b.m_time - a.m_time)) * weight };
			}
		}
	}
	return poses;
}

vector<JointPose> CSkeletonAnimation::FullMatchSample(AnimationClip& clip, Skeleton& skeleton, float t, float weight)
{
	vector<JointPose> poses;
	t = clip.m_isLooping ? fmod(t, clip.m_length) : t;
	if (t > clip.m_length || t < 0 || clip.m_aSamples.size() == 0)
		return poses;
	poses.resize(skeleton.GetSize());
	for (byte joint = 0, index = 0; joint < skeleton.GetSize(); joint++)
	{
		AnimationSample* a = NULL;
		AnimationSample* b = NULL;
		for (int i = 0; i < clip.m_aSamples.size() && (!a || !b); i++)
		{
			index = clip.m_aSamples.size() - i - 1;
			if (!a && clip.m_aSamples[index].m_time <= t && clip.m_aSamples[index].m_jointPoses.find(joint) != clip.m_aSamples[index].m_jointPoses.end())
				a = &clip.m_aSamples[index];
			if (!b && clip.m_aSamples[i].m_time >= t && clip.m_aSamples[i].m_jointPoses.find(joint) != clip.m_aSamples[i].m_jointPoses.end())
				b = &clip.m_aSamples[i];
		}
		Matrix4x4 mat_a = a ? a->m_jointPoses.find(joint)->second.m_matrix : skeleton.GetJoint(joint)->m_localMatrix;
		Matrix4x4 mat_b = b ? b->m_jointPoses.find(joint)->second.m_matrix : skeleton.GetJoint(joint)->m_localMatrix;
		float a_time = a ? a->m_time : 0;
		float b_time = b ? b->m_time : 1;
		poses[joint] = JointPose { Matrix4x4::Lerp(mat_a, mat_b, (t - a_time) / (b_time - a_time)) * weight };
	}
	return poses;
}

vector<JointPose> CSkeletonAnimation::Blend(AnimationClip** clips, float* timePos, float* weights, int count, Skeleton& skeleton)
{
	float total_weight = 0;
	for (int i = 0; i < count; ++i)
	{
		total_weight += weights[i];
	}
	total_weight = 1.0f / total_weight;
	vector<JointPose> jointPoses(skeleton.GetSize());
	for (int i = 0; i < count; ++i)
	{
		vector<JointPose> poses = Sample(*clips[i], skeleton, timePos[i], weights[i] * total_weight);
		if(poses.size())
		{
			for (int j = 0; j < poses.size(); j++)
			{
				jointPoses[j].m_matrix += poses[j].m_matrix;
			}
		}
	}
	return jointPoses;
}

void CSkeletonAnimation::CalculateGlobalMatrix(Skeleton& skeleton)
{
	for (Joint& joint : skeleton.GetJoints())
	{
		Matrix4x4 matj = Matrix4x4::Identity();
		Joint* p = &joint;
		do {
			matj = p->m_localMatrix * matj;
			if (p->m_iParent == 0xFF)
				break;
			p = skeleton.GetJoint(p->m_iParent);
		} while (true);
		skeleton.m_globalPoses[joint.m_Index] = matj;
	}
}

void CSkeletonAnimation::CalculateGlobalMatrix(Skeleton& skeleton, vector<JointPose> localPoses)
{
	if (!localPoses.size())
		return;
	for (Joint& joint : skeleton.GetJoints())
	{
		Matrix4x4 matj = Matrix4x4::Identity();
		Joint* p = &joint;
		do {
			matj = localPoses[p->m_Index].m_matrix * matj;
			if (p->m_iParent == 0xFF)
				break;
			p = skeleton.GetJoint(p->m_iParent);
		} while (true);
		skeleton.m_globalPoses[joint.m_Index] = matj;
	}
}

void CSkeletonAnimation::CalculateSkinningMatrix(Skeleton& skeleton)
{
	Matrix4x4 mat;
	vector<Joint>& joints = skeleton.GetJoints();
	for (int i = 0; i < joints.size(); ++i)
	{
		mat.MakeZero();
		mat += skeleton.m_globalPoses[i] * joints[i].m_invBindPose * skeleton.m_bindShapeMat;
		skeleton.m_skiningMatrices[i] = mat;
	}
}