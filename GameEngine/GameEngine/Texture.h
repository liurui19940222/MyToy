#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include<gl\glew.h>
#include"Object.h"
#include"FileHead.h"
#include"DynamicFactory.h"

enum ENGINE_API ETexWrapMode
{
	Clamp = 0x2900,
	ClampToEdge = 1,
	Repeat = 0x2901,
};

enum ENGINE_API ETexFilterMode
{
	Point = GL_NEAREST,
	Linear = GL_LINEAR,
	Mipmap = GL_LINEAR_MIPMAP_LINEAR,
};

enum ENGINE_API ETexEnvMode
{
	Replace = 0x1E01,
	Modulate = 0x2100,
	Add = 0x0104,
};

class ENGINE_API CTexture : public Object
{
	REFLECT_CLASS(CTexture)
protected:
	UINT m_texId;
	int m_width;
	int m_height;
	int m_format;
	int m_internalFormat;
	ETexEnvMode m_envMode = ETexEnvMode::Replace;
	ETexFilterMode m_filterMode = ETexFilterMode::Linear;
	ETexWrapMode m_wrapMode = ETexWrapMode::ClampToEdge;

public:
	int GetWidth() const;
	int GetHeight() const;
	int GetFormat() const;
	int GetInternalFormat() const;
	UINT GetTextureId() const;
	ETexEnvMode GetEnvMode() const;
	ETexFilterMode GetFilterMode() const;
	ETexWrapMode GetWrapMode() const;
	CTexture* Bind();
	CTexture* SetEnvMode(ETexEnvMode mode);
	CTexture* SetWrapMode(ETexWrapMode mode);
	CTexture* SetFilterMode(ETexFilterMode mode);

	virtual void OnRelease() override;
};

#endif