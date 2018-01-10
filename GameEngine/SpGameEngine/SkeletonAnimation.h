#ifndef _BONE_ANIMATION_H_
#define _BONE_ANIMATION_H_

#include<string>
#include<vector>
#include<map>
#include"Math.h"
#include"..\SpCommon\EngineDefine.h"
#include"..\SpCommon\Debug.h"
#include"..\SpCommon\ApiDefine.h"

using namespace std;

typedef TmpVector4<byte> BVector4;
struct JointPose;

struct Joint
{
	string m_name;				//关节名字
	byte m_Index;				//在数组中的索引
	byte m_iParent;				//父索引，或0xFF代表根关节
	Matrix4x4 m_invBindPose;	//绑定姿势之逆变换
	Matrix4x4 m_localMatrix;	//局部矩阵

	Joint() {
		m_invBindPose.MakeIdentity();
		m_localMatrix.MakeIdentity();
	}
};

struct ENGINE_API Skeleton
{
	vector<Joint> m_joints;
	vector<JointPose> m_localPoses;
	vector<Matrix4x4> m_globalPoses;
	vector<Matrix4x4> m_skiningMatrices;
	vector<Vector4> m_weights;
	vector<BVector4> m_indices;
	Matrix4x4 m_bindShapeMat;

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

	//得到关节数量
	inline int GetSize()
	{
		return m_joints.size();
	}

	//得到权重数量
	inline int GetWeightSize()
	{
		return m_weights.size();
	}
};

struct ENGINE_API JointPose
{
	Matrix4x4 m_matrix;			//变换矩阵
};

struct ENGINE_API AnimationSample
{
	float m_time;			//该采样的时间点
	map<byte, JointPose> m_jointPoses;	//该采样所引用的关节和对应的局部姿势
};

struct ENGINE_API AnimationClip
{
	vector<AnimationSample> m_aSamples;
	float m_length;
	bool m_isLooping;
};

struct ENGINE_API Mesh
{
	Vector3* m_vertices = NULL;
	Vector3* m_normals = NULL;
	Vector2* m_texcoords = NULL;
	Color* m_colors = NULL;
	int m_vertexCount = 0;

	inline Mesh() {}
	Mesh(const Mesh& mesh);
	~Mesh();
};

struct ENGINE_API Model
{
	Skeleton m_skeleton;
	Mesh* m_meshes = NULL;
	int m_meshCount = 0;
	AnimationClip* m_animations = NULL;
	int m_animationCount = 0;
};

class ENGINE_API CSkeletonAnimation {

public:
	static vector<JointPose> Sample(AnimationClip& clip, Skeleton& skeleton, float t, float weight);
	static vector<JointPose> FullMatchSample(AnimationClip& clip, Skeleton& skeleton, float t, float weight);
	static vector<JointPose> Blend(AnimationClip** clips, float* timePos, float* weights, int count, Skeleton& skeleton);
	static void CalculateGlobalMatrix(Skeleton& skeleton);
	static void CalculateGlobalMatrix(Skeleton& skeleton, vector<JointPose> localPoses);
	static void CalculateSkinningMatrix(Skeleton& skeleton);
};

#endif