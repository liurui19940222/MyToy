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
	Matrix4x4 m_localAnimMatrix;//为了不让动画覆盖原始的局部矩阵
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
		return 0xFF;
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

struct SkeletonWeight
{
	int m_count;
	Vector4* m_weights;
	BVector4* m_indices;
};

struct JointPose
{
	Matrix4x4 m_matrix;			//变换矩阵
};

struct SkeletonPose
{
	Matrix4x4* m_aGlobalPose;	//多个全局关节姿势
};

struct AnimationSample
{
	float m_time;			//该采样的时间点
	map<byte, JointPose> m_jointPoses;	//该采样所引用的关节和对应的局部姿势
};

struct AnimationClip
{
	Skeleton* m_pSkeleton;
	vector<AnimationSample> m_aSamples;
	float m_length;
	bool m_isLooping;

	void Sample(float t);
};

#endif