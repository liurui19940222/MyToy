#ifndef _BONE_ANIMATION_H_
#define _BONE_ANIMATION_H_

#include<string>
#include<vector>
#include<map>
#include"SpCommon\Object.h"
#include"Math.h"
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Debug.h"
#include"SpCommon\ApiDefine.h"

using namespace std;

BEGIN_NAMESPACE_ENGINE

typedef TmpVector4<byte> BVector4;
struct JointPose;

#define JOINT_ROOT 0xFF

SMART_STRUCT(Joint) struct Joint
{
	string		m_Name;				//关节名字
	byte		m_Index;			//在数组中的索引
	byte		m_iParent;			//父索引，或0xFF代表根关节
	Matrix4x4	m_InvBindPose;		//绑定姿势之逆变换
	Matrix4x4	m_LocalMatrix;		//局部矩阵

	Joint() {
		m_InvBindPose.MakeIdentity();
		m_LocalMatrix.MakeIdentity();
	}
};

SMART_STRUCT(Skeleton) struct Skeleton
{
	vector<Joint>		m_Joints;
	vector<JointPose>	m_LocalPoses;
	vector<Matrix4x4>	m_GlobalPoses;
	vector<Matrix4x4>	m_SkiningMatrices;
	Matrix4x4			m_BindShapeMat;

	inline void AddJoint(Joint& joint)
	{
		m_Joints.push_back(joint);
	}

	inline Joint* GetJoint(const string& name)
	{
		for (Joint& joint : m_Joints)
			if (joint.m_Name == name)
				return &joint;

		return NULL;
	}

	inline Joint* GetJoint(int index)
	{
		if (index >= m_Joints.size())
			return NULL;
		return &m_Joints[index];
	}

	inline byte GetJointIndex(const string& name)
	{
		for (byte i = 0; i < m_Joints.size(); ++i)
			if (m_Joints[i].m_Name == name)
				return i;
		return JOINT_ROOT;
	}

	inline vector<Joint>& GetJoints()
	{
		return m_Joints;
	}

	//得到关节数量
	inline int GetSize()
	{
		return m_Joints.size();
	}
};

SMART_STRUCT(JointPose) struct JointPose
{
	Matrix4x4 m_Matrix;						//变换矩阵
};

SMART_STRUCT(AnimationSample) struct AnimationSample
{
	float				 m_Time;			//该采样的时间点
	map<byte, JointPose> m_JointPoses;		//该采样所引用的关节和对应的局部姿势

	inline void InitWithPose(vector<JointPose>& pose, float time) 
	{
		m_JointPoses.clear();
		m_Time = time;
		for (int i = 0; i < pose.size(); ++i)
		{
			m_JointPoses.insert(make_pair((byte)i, pose[i]));
		}
	}
};

SMART_STRUCT(AnimationClip) struct AnimationClip
{
	vector<AnimationSample> m_aSamples;
	float					m_FrameCountPerSecond;
	float					m_Length;
	bool					m_IsLooping;
	string					m_Name;
};

SMART_STRUCT(Mesh) struct Mesh
{
	Vector3*	m_Vertices = NULL;
	Vector3*	m_Normals = NULL;
	Vector2*	m_Texcoords = NULL;
	Color*		m_Colors = NULL;
	ushort*		m_Indices = NULL;
	Vector4*	m_JointWeights = NULL;
	BVector4*	m_JointIndices = NULL;
	int			m_VertexCount = 0;
	int			m_TriangleCount = 0;

	inline Mesh() {}
	Mesh(const Mesh& mesh);
	~Mesh();
};

SMART_STRUCT(Model) struct Model
{
	PSkeleton				m_Skeleton;
	vector<PMesh>			m_Meshes;
	vector<PAnimationClip>	m_Animations;

	inline size_t meshesCount() { return m_Meshes.size(); }
	inline size_t animationsCount() { return m_Animations.size(); }
};

class SkeletonAnimation {

public:
	static bool Sample(vector<JointPose>& outPose, PAnimationClip clip, PSkeleton skeleton, float t, float weight);
	static bool Blend(vector<JointPose>& outPose, PAnimationClip* clips, float* timePos, float* weights, int count, PSkeleton skeleton);
	static PAnimationClip Slice(PAnimationClip srcClip, int ibegin, int iend, const string& newName);
	static PAnimationClip Slice(PAnimationClip srcClip, PSkeleton skeleton, float begin, float end, const string& newName);
	static void CalculateGlobalMatrix(PSkeleton skeleton);
	static void CalculateGlobalMatrix(PSkeleton skeleton, vector<JointPose> localPoses);
	static void CalculateSkinningMatrix(PSkeleton skeleton);
};

END_NAMESPACE_ENGINE

#endif