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
private:
	vector<Joint> m_joints;
public:
	inline void AddJoint(Joint& joint)
	{
		m_joints.push_back(joint);
	}

	inline Joint* GetJoint(string name)
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