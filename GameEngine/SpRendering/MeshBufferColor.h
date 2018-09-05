#pragma once

#include"MeshBuffer.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferColor) class MeshBufferColor : public MeshBuffer
{
	const int ATTR_POS_COLOR = 1;
public:
	MeshBufferColor();
	virtual ~MeshBufferColor();
	MeshBufferColor(MeshPtr mesh);

	void MakeColorBuffer(const Color* colors, int size);
	virtual void MakeBuffer(MeshPtr mesh) override;

private:
	GLuint		m_VboColorHandle;
};

END_NAMESPACE_ENGINE
