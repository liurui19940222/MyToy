#include "stdafx.h"
#include "MeshBufferUIInstance.h"

USING_NAMESPACE_GUI;

MeshBufferUIInstance::MeshBufferUIInstance() : 
	MeshBufferTexcoord(), m_VboTexRangeHandle(0), m_VboModelMatrixHandle(0), m_VboColorHandle(0), m_VboRectSizeHandle(0) {}

MeshBufferUIInstance::MeshBufferUIInstance(PMesh mesh) : 
	MeshBufferTexcoord(mesh), m_VboTexRangeHandle(0), m_VboModelMatrixHandle(0), m_VboColorHandle(0), m_VboRectSizeHandle(0) {}

void MeshBufferUIInstance::MakeInstanceBuffer(vector<TexcoordRange>& ranges, vector<Color>& colors, vector<SRect2D>& sizelist, vector<Matrix4x4>& matrices, int instanceCount)
{
	BindBuffer();
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboTexRangeHandle, sizeof(TexcoordRange), 4, instanceCount, &ranges[0], ATTR_POS_TEXRANGE, EBufferUsage::StaticDraw);
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboColorHandle, sizeof(Color), 4, instanceCount, &colors[0], ATTR_POS_COLOR, EBufferUsage::StaticDraw);
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboRectSizeHandle, sizeof(SRect2D), 4, instanceCount, &sizelist[0], ATTR_POS_RECTSIZE, EBufferUsage::StaticDraw);
	MeshBuffer::MakeInstanceVertexBuffer(&m_VboModelMatrixHandle, sizeof(Matrix4x4), 16, instanceCount, &matrices[0], ATTR_POS_MODELMATRIX, EBufferUsage::StaticDraw);
	UnbindBuffer();
}

void MeshBufferUIInstance::ReleaseBuffer()
{
	DeleteBufer(&m_VboTexRangeHandle);
	DeleteBufer(&m_VboModelMatrixHandle);
	MeshBufferTexcoord::ReleaseBuffer();
}