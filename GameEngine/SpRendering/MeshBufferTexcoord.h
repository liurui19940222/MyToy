#pragma once

#include"MeshBuffer.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferTexcoord) class MeshBufferTexcoord : public MeshBuffer
{
protected:
	const int ATTR_POS_TEXCOORD = 1;

public:
	MeshBufferTexcoord();
	MeshBufferTexcoord(PMesh mesh);
	virtual ~MeshBufferTexcoord();

	void MakeTexcoordBuffer(const Vector2* uvs, int size);
	virtual void MakeBuffer(PMesh mesh) override;

private:
	GLuint		 m_VboTexcoordHandle;
};

END_NAMESPACE_ENGINE