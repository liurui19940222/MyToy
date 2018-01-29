#pragma once

#include"GUIDefines.h"
#include"SpRendering\MeshBufferTexcoord.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(MeshBufferUIInstance) class MeshBufferUIInstance : MeshBufferTexcoord
{
	const int ATTR_POS_TEXRANGE = 2;
	const int ATTR_POS_MODELMATRIX = 3;
public:
	MeshBufferUIInstance();
	MeshBufferUIInstance(PMesh mesh);
	void MakeTexcoordRangeBuffer(vector<SRect2D>& ranges);
	void MakeModelMatrixBuffer(vector<Matrix4x4>& matrices);
	virtual void ReleaseBuffer() override;

private:
	GLuint			m_VboTexRangeHandle;
	GLuint			m_VboModelMatrixHandle;
};

END_NAMESPACE_GUI