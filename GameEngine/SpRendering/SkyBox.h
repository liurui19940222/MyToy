#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include"Texture2D.h"
#include"MeshBufferTexcoord.h"
#include"SpCommon\Object.h"

BEGIN_NAMESPACE_ENGINE

enum class ESkyBoxFace
{
	Top = 0,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

SMART_CLASS(SkyBox) class SkyBox : public Object {
	Texture2DPtr m_textures[6];
	MeshBufferTexcoordPtr m_buffer[6];
	void InitBuffer();
public:
	SkyBox();
	virtual ~SkyBox();
	static SkyBoxPtr Create(const char* top, const char* bottom, const char* front, const char* back, const char* left, const char* right);
	void Render(const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection);
	void Release();
	virtual void OnInitialize() override;
};

END_NAMESPACE_ENGINE

#endif