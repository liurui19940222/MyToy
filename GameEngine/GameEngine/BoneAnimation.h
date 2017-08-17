#ifndef _BONE_ANIMATION_H_
#define _BONE_ANIMATION_H_

#include<string>
#include<vector>
#include<map>
#include"Math.h"
#include"EngineDefine.h"

using namespace std;

struct Joint
{
	Matrix4x4 m_invBindPose;	//������֮��任
	Matrix4x4 m_localMatrix;	//�ֲ�����
	Matrix4x4 m_globalMatrix;	//ȫ�־���
	string m_name;				//�ؽ�����
	byte m_iParent;				//����������0xFF������ؽ�

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
	byte* m_jointIndices;
	Joint** m_joints;
	float* m_weights;
	byte m_count;
};

struct JointPose
{
	Matrix4x4 m_matrix;			//�任����
};

struct SkeletonPose
{
	Skeleton* m_pSkeleton;		//����+�ؽ�����
	JointPose* m_aLoclPose;		//����ֲ��ؽ�����
	Matrix4x4* m_aGlobalPose;	//���ȫ�ֹؽ�����
};

struct AnimationSample
{
	JointPose* m_aJointPose;	//�ؽ���������
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