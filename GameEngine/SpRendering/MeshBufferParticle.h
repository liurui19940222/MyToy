#pragma once

#include"..\SpRendering\MeshBufferTexcoord.h"
#include"..\SpRendering\Texture.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(MeshBufferParticle) class MeshBufferParticle : public MeshBufferTexcoord
{
	const int ATTR_POS_COLOR = 2;
	const int ATTR_POS_TEXRANGE = 3;
	const int ATTR_POS_MODELMATRIX = 4;

public:
	MeshBufferParticle();
	MeshBufferParticle(PMesh mesh);
	virtual ~MeshBufferParticle();
	void MakeInstanceBuffer(vector<Matrix4x4>& matrices, vector<Color>& colors, vector<TexcoordRange>& ranges, int instanceCount);

private:
	uint	m_VboMatrixHandle;
	uint	m_VboColorHandle;
	uint	m_VboTexRangeHandle;
};

END_NAMESPACE_ENGINE