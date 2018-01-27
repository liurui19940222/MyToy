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
	MeshBufferSkinning(PMesh mesh);
	MeshBufferSkinning(PMesh mesh, const vector<Vector4>& weights, const vector<BVector4>& indices);

	void MakeJointBuffer(const vector<Vector4>& weights, const vector<BVector4>& indices);
	virtual void ReleaseBuffer() override;

private:
	GLuint		m_VboJointIndexHandle;
	GLuint		m_VboJointWeightHandle;
};

END_NAMESPACE_ENGINE