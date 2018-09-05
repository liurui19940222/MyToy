#include"SkeletonAnimation.h"
#include<iostream>
#include<assert.h>

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

bool SkeletonAnimation::Sample(vector<JointPose>& outPose, AnimationClipPtr clip, SkeletonPtr skeleton, float t, float weight)
{
	if (clip->m_IsLooping)
		t = fmod(t, clip->m_Length);
	else
		t = CMath::Clamp(t, clip->m_aSamples[0].m_Time, clip->m_aSamples[clip->m_aSamples.size() - 1].m_Time);
	if (t > clip->m_Length || t < 0 || clip->m_aSamples.empty())
		return false;
	outPose.resize(skeleton->GetSize());
	for (uint sampleIndex = 0; sampleIndex < clip->m_aSamples.size() - 1; sampleIndex++)
	{
		AnimationSample& a = clip->m_aSamples[sampleIndex];
		AnimationSample& b = clip->m_aSamples[sampleIndex + 1];

		if (a.m_Time <= t && b.m_Time >= t)
		{
			Matrix4x4 mat_a;
			Matrix4x4 mat_b;
			for (byte jointIndex = 0; jointIndex < skeleton->GetSize(); jointIndex++)
			{
				auto it_a = a.m_JointPoses.find(jointIndex);
				auto it_b = b.m_JointPoses.find(jointIndex);
				if (it_a != a.m_JointPoses.end())
					mat_a = it_a->second.m_Matrix;
				else
				{
					outPose[jointIndex] = JointPose{ skeleton->GetJoint(jointIndex)->m_LocalMatrix };
					continue;
				}
				if (it_b != b.m_JointPoses.end())
					mat_b = it_b->second.m_Matrix;
				else
				{
					outPose[jointIndex] = JointPose{ skeleton->GetJoint(jointIndex)->m_LocalMatrix };
					continue;
				}

				outPose[jointIndex] = JointPose{ Matrix4x4::Lerp(mat_a, mat_b, (t - a.m_Time) / (b.m_Time - a.m_Time)) * weight };
			}
		}
	}
	return true;
}

bool SkeletonAnimation::Blend(vector<JointPose>& outPose, AnimationClipPtr* clips, float* timePos, float* weights, int count, SkeletonPtr skeleton)
{
	float total_weight = 0;
	for (int i = 0; i < count; ++i)
	{
		total_weight += weights[i];
	}
	total_weight = 1.0f / total_weight;
	outPose.clear();
	outPose.resize(skeleton->GetSize());
	vector<JointPose> poses;
	for (int i = 0; i < count; ++i)
	{
		bool success = Sample(poses, clips[i], skeleton, timePos[i], weights[i] * total_weight);
		if(success && poses.size())
		{
			for (uint j = 0; j < poses.size(); j++)
			{
				outPose[j].m_Matrix += poses[j].m_Matrix;
			}
		}
	}
	return true;
}

void SkeletonAnimation::CalculateGlobalMatrix(SkeletonPtr skeleton)
{
	if (!skeleton.get())
		return;
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

void SkeletonAnimation::CalculateGlobalMatrix(SkeletonPtr skeleton, vector<JointPose> localPoses)
{
	if (!skeleton.get() || !localPoses.size())
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

void SkeletonAnimation::CalculateSkinningMatrix(SkeletonPtr skeleton)
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

AnimationClipPtr SkeletonAnimation::Slice(AnimationClipPtr srcClip, int ibegin, int iend, const string& newName)
{
	float begin = (float)ibegin, end = (float)iend;
	AnimationClipPtr clip = make_shared<AnimationClip>();
	clip->m_Name = newName;
	if (end <= begin)
		return clip;
	clip->m_FrameCountPerSecond = srcClip->m_FrameCountPerSecond;
	clip->m_IsLooping = srcClip->m_IsLooping;
	clip->m_Length = end - begin + 1.0f;
	clip->m_aSamples.push_back(AnimationSample());
	AnimationSample firstSample, lastSample;
	bool hasFirstSample = false, hasLastSample = false;
	AnimationSample* p = NULL;
	for (int i = 0; i < srcClip->m_aSamples.size(); ++i)
	{
		p = &srcClip->m_aSamples[i];
		if (p->m_Time == begin)
		{
			firstSample = *p;
			firstSample.m_Time -= begin;
			hasFirstSample = true;
		}
		if (p->m_Time == end)
		{
			lastSample = *p;
			lastSample.m_Time -= begin;
			hasLastSample = true;
			break;
		}
		if (p->m_Time > begin && p->m_Time < end)
		{
			clip->m_aSamples.push_back(*p);
			(clip->m_aSamples.end() - 1)->m_Time -= begin;
		}
	}
	assert(hasFirstSample);
	assert(hasLastSample);
	clip->m_aSamples[0] = firstSample;
	clip->m_aSamples.push_back(lastSample);
	return clip;
}

AnimationClipPtr SkeletonAnimation::Slice(AnimationClipPtr srcClip, SkeletonPtr skeleton, float begin, float end, const string& newName)
{
	AnimationClipPtr clip = make_shared<AnimationClip>();
	clip->m_Name = newName;
	if (end <= begin)
		return clip;
	clip->m_FrameCountPerSecond = srcClip->m_FrameCountPerSecond;
	clip->m_IsLooping = srcClip->m_IsLooping;
	clip->m_Length = end - begin + 1.0f;
	clip->m_aSamples.push_back(AnimationSample());
	AnimationSample firstSample, lastSample;
	bool hasFirstSample = false, hasLastSample = false;
	AnimationSample* p = NULL;
	for (int i = 0; i < srcClip->m_aSamples.size(); ++i)
	{
		p = &srcClip->m_aSamples[i];
		if (p->m_Time == begin)
		{
			firstSample = *p;
			firstSample.m_Time -= begin;
			hasFirstSample = true;
		}
		if (p->m_Time == end)
		{
			lastSample = *p;
			lastSample.m_Time -= begin;
			hasLastSample = true;
			break;
		}
		if (p->m_Time > begin && p->m_Time < end)
		{
			clip->m_aSamples.push_back(*p);
			(clip->m_aSamples.end() - 1)->m_Time -= begin;
		}
	}
	if (!hasFirstSample)
	{
		vector<JointPose> pose;
		assert(Sample(pose, srcClip, skeleton, begin, 1.0f));
		firstSample.InitWithPose(pose, begin);
	}
	if (!hasLastSample)
	{
		vector<JointPose> pose;
		assert(Sample(pose, srcClip, skeleton, end, 1.0f));
		lastSample.InitWithPose(pose, end);
	}
	clip->m_aSamples[0] = firstSample;
	clip->m_aSamples.push_back(lastSample);
	return clip;
}