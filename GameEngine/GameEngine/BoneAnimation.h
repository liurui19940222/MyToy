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
	Matrix4x4 m_localMatrix;	//ģ�;���
	string m_name;				//�ؽ�����
	byte m_iParent;				//����������0xFF������ؽ�
};

struct Skeleton
{
	vector<Joint> m_joints;

	inline void Add(Joint& joint)
	{
		m_joints.push_back(joint);
	}

	inline Joint* Get(string name)
	{
		for (Joint& joint : m_joints)
			if (joint.m_name == name)
				return &joint;
		return NULL;
	}
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