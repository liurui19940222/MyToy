#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include"Texture2D.h"
#include"MeshBuffer.h"
#include"..\SpCommon\Object.h"

enum class ESkyBoxFace
{
	Top = 0,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class CSkyBox : public Object{
	CTexture2D* m_textures[6];
	CMeshBuffer m_buffer[6];
	void InitBuffer();
public:
	CSkyBox();
	static CSkyBox* Create(const char* top, const char* bottom, const char* front, const char* back, const char* left, const char* right);
	void Render(const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection);
	virtual void OnInitialize() override;
	virtual void OnRelease() override;
};

#endif