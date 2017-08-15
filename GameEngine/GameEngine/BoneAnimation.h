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
	Matrix4x4 m_invBindPose;	//绑定姿势之逆变换
	Matrix4x4 m_localMatrix;	//模型矩阵
	string m_name;				//关节名字
	byte m_iParent;				//父索引，或0xFF代表根关节
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