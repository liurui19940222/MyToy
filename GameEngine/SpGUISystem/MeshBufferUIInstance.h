#pragma once

#include"GUIDefines.h"
#include"SpRendering\MeshBufferTexcoord.h"
#include"SpRendering\Texture.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(MeshBufferUIInstance) class MeshBufferUIInstance : public MeshBufferTexcoord
{
	const int ATTR_POS_TEXRANGE = 2;
	const int ATTR_POS_COLOR = 3;
	const int ATTR_POS_RECTSIZE = 4;
	const int ATTR_POS_MODELMATRIX = 5;
public:
	MeshBufferUIInstance();
	MeshBufferUIInstance(PMesh mesh);
	virtual ~MeshBufferUIInstance();
	void MakeInstanceBuffer(vector<TexcoordRange>& ranges, vector<Color>& colors, 
		vector<SRect2D>& sizelist, vector<Matrix4x4>& matrices, int instanceCount);

private:
	GLuint			m_VboTexRangeHandle;
	GLuint			m_VboColorHandle;
	GLuint			m_VboRectSizeHandle;
	GLuint			m_VboModelMatrixHandle;
};

END_NAMESPACE_GUI