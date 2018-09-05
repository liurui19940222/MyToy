#pragma once
#include"MeshBufferNormal.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferSkinning) class MeshBufferSkinning : public MeshBufferNormal
{
protected:
	const int ATTR_POS_JOINT_INDEX = 3;
	const int ATTR_POS_JOINT_WEIGHT = 4;

public:
	MeshBufferSkinning();
	MeshBufferSkinning(MeshPtr mesh);
	virtual ~MeshBufferSkinning();

	void MakeJointBuffer(const Vector4* weights, const BVector4* indices, int count);

private:
	GLuint		m_VboJointIndexHandle;
	GLuint		m_VboJointWeightHandle;
};

END_NAMESPACE_ENGINE