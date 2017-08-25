#ifndef _BONE_ANIMATION_H_
#define _BONE_ANIMATION_H_

#include<string>
#include<vector>
#include<map>
#include"Math.h"
#include"EngineDefine.h"
#include"Debug.h"

using namespace std;

typedef TmpVector4<byte> BVector4;

struct Joint
{
	string m_name;				//关节名字
	byte m_iParent;				//父索引，或0xFF代表根关节
	Matrix4x4 m_invBindPose;	//绑定姿势之逆变换
	Matrix4x4 m_localMatrix;	//局部矩阵
	Matrix4x4 m_globalMatrix;	//全局矩阵

	Joint() {
		m_invBindPose.MakeIdentity();
		m_localMatrix.MakeIdentity();
		m_globalMatrix.MakeIdentity();
	}
};

struct Skeleton
{
	vector<Joint> m_joints;

	inline void AddJoint(Joint& joint)
	{
		m_joints.push_back(joint);
	}

	inline Joint* GetJoint(const string& name)
	{
		for (Joint& joint : m_joints)
			if (joint.m_name == name)
				return &joint;

		return NULL;
	}

	inline Joint* GetJoint(int index)
	{
		return &m_joints[index];
	}

	inline byte GetJointIndex(const string& name)
	{
		for (byte i = 0; i < m_joints.size(); ++i)
			if (m_joints[i].m_name == name)
				return i;
		return -1;
	}

	inline vector<Joint>& GetJoints()
	{
		return m_joints;
	}

	inline int GetSize()
	{
		return m_joints.size();
	}
};

struct JointWeight
{
	byte* m_jointIndices = NULL;
	float* m_weights = NULL;
	byte m_count = 0;

	inline static void Copy(JointWeight& dst, const JointWeight& src)
	{
		if (!dst.m_jointIndices) dst.m_jointIndices = (byte*)malloc(src.m_count);
		if (!dst.m_weights) dst.m_weights = (float*)malloc(sizeof(float) * src.m_count);
		memcpy(dst.m_jointIndices, src.m_jointIndices, src.m_count);
		memcpy(dst.m_weights, src.m_weights, src.m_count * sizeof(float));
	}
};

struct SkeletonWeight
{
	int m_count;
	JointWeight* m_jointWeights;

	inline static void Free(SkeletonWeight& skeletonWeight)
	{
		for (int i = 0; i < skeletonWeight.m_count; i++)
		{
			free(skeletonWeight.m_jointWeights[i].m_jointIndices);
			free(skeletonWeight.m_jointWeights[i].m_weights);
		}
		free(skeletonWeight.m_jointWeights);
	}
};

struct JointPose
{
	Matrix4x4 m_matrix;			//变换矩阵
};

struct SkeletonPose
{
	Skeleton* m_pSkeleton;		//骨骼+关节数量
	JointPose* m_aLoclPose;		//多个局部关节姿势
	Matrix4x4* m_aGlobalPose;	//多个全局关节姿势
};

struct AnimationSample
{
	JointPose* m_aJointPose;	//关节姿势数组
};

struct AnimationClip
{
	Skeleton* m_pSkeleton;
	float m_framePerSecond;
	uint m_frameCount;
	AnimationSample* m_aSamples;
	bool m_isLooping;
};

#endif