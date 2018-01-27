#pragma once

#include"MeshBuffer.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferColor) class MeshBufferColor : public MeshBuffer
{
	const int ATTR_POS_COLOR = 1;
public:
	MeshBufferColor();
	MeshBufferColor(PMesh mesh);

	void MakeColorBuffer(const Color* colors, int size);
	virtual void MakeBuffer(PMesh mesh) override;
	virtual void ReleaseBuffer() override;

private:
	GLuint		m_VboColorHandle;
};

END_NAMESPACE_ENGINE
