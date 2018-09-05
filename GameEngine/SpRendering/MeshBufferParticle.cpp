#include "MeshBufferParticle.h"

USING_NAMESPACE_ENGINE;

MeshBufferParticle::MeshBufferParticle() : MeshBufferTexcoord(), m_VboMatrixHandle(0), m_VboColorHandle(0), m_VboTexRangeHandle(0)
{
}

MeshBufferParticle::MeshBufferParticle(MeshPtr mesh) : MeshBufferTexcoord(mesh), m_VboMatrixHandle(0), m_VboColorHandle(0), m_VboTexRangeHandle(0)
{
}

MeshBufferParticle::~MeshBufferParticle()
{
	DeleteBufer(&m_VboMatrixHandle);
	DeleteBufer(&m_VboColorHandle);
	DeleteBufer(&m_VboTexRangeHandle);
}

void MeshBufferParticle::MakeInstanceBuffer(vector<Matrix4x4>& matrices, vector<Color>& colors, vector<TexcoordRange>& ranges, int instanceCount)
{
	BindBuffer();
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboColorHandle, sizeof(Color), 4, instanceCount, &colors[0], ATTR_POS_COLOR, EBufferUsage::DynamicDraw);
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboTexRangeHandle, sizeof(TexcoordRange), 4, instanceCount, &ranges[0], ATTR_POS_TEXRANGE, EBufferUsage::DynamicDraw);
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboMatrixHandle, sizeof(Matrix4x4), 16, instanceCount, &matrices[0], ATTR_POS_MODELMATRIX, EBufferUsage::DynamicDraw);
	UnbindBuffer();
}