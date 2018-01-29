#include "stdafx.h"
#include "MeshBufferUIInstance.h"

USING_NAMESPACE_GUI;

MeshBufferUIInstance::MeshBufferUIInstance() : 
	MeshBufferTexcoord(), m_VboTexRangeHandle(0), m_VboModelMatrixHandle(0) {}

MeshBufferUIInstance::MeshBufferUIInstance(PMesh mesh) : 
	MeshBufferTexcoord(mesh), m_VboTexRangeHandle(0), m_VboModelMatrixHandle(0) {}

void MeshBufferUIInstance::MakeTexcoordRangeBuffer(vector<SRect2D>& ranges)
{

}

void MeshBufferUIInstance::MakeModelMatrixBuffer(vector<Matrix4x4>& matrices)
{
	
}