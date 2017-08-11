#ifndef _BONE_ANIMATION_H_
#define _BONE_ANIMATION_H_

#include<string>
#include"Math.h"
#include"EngineDefine.h"

using namespace std;

struct Joint
{
	Matrix4x4 m_invBindPose;	//绑定姿势之逆变换
	string m_name;				//关节名字
	byte m_iParent;				//父索引，或0xFF代表根关节
};

struct Skeleton
{
	uint m_jointCount;			//关节数目
	Joint* m_aJoint;			//关节数组
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