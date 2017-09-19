#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include"Object.h"
#include"Texture2D.h"
#include"MeshBuffer.h"

enum class ENGINE_API ESkyBoxFace
{
	Top = 0,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class ENGINE_API CSkyBox : public Object
{
	CTexture2D* m_textures[6];
	CMeshBuffer m_buffer[6];
public:
	static CSkyBox* Create(const char* top, const char* bottom, const char* front, const char* back, const char* left, const char* right);
	void Render(const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection);
	virtual void OnInitialize() override;
	virtual void OnRelease() override;
};

#endif