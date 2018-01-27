#pragma once

#include"MeshBufferTexcoord.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferNormal)class MeshBufferNormal : public MeshBufferTexcoord
{
protected:
	const int ATTR_POS_NORMAL = 2;

public:
	MeshBufferNormal();
	MeshBufferNormal(PMesh mesh);

	void MakeNormalBuffer(const Vector3* normals, int size);
	virtual void MakeBuffer(PMesh mesh) override;
	virtual void ReleaseBuffer() override;

private:
	GLuint			m_VboNormalHandle;
};

END_NAMESPACE_ENGINE
