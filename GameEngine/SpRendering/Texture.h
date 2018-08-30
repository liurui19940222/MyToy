#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include<gl\glew.h>
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Object.h"

BEGIN_NAMESPACE_ENGINE

enum ETexWrapMode
{
	Clamp = 0x2900,
	ClampToEdge = 0x812F,
	Repeat = 0x2901,
};

enum ETexFilterMode
{
	Point = GL_NEAREST,
	Linear = GL_LINEAR,
	MipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum ETexEnvMode
{
	Replace = 0x1E01,
	Modulate = 0x2100,
	Add = 0x0104,
};

struct TexcoordRange
{
	Vector2		m_StartingPoint;
	Vector2		m_Size;

	TexcoordRange();
	TexcoordRange(float startingX, float startingY, float sizeX, float sizeY);
	TexcoordRange(Vector2 staringPoint, Vector2 size);

	static const TexcoordRange none;
	static const TexcoordRange full;

	void Slice(BVector4 border, vector<TexcoordRange>& out, float internalWidth, float internalHeight, float rawWidth, float rawHeight);
};

SMART_CLASS(Texture) class Texture : public Object
{
protected:
	uint m_texId;
	int m_width;
	int m_height;
	int m_format;
	int m_internalFormat;
	ETexEnvMode m_envMode = ETexEnvMode::Replace;
	ETexFilterMode m_filterMode = ETexFilterMode::Linear;
	ETexWrapMode m_wrapMode = ETexWrapMode::ClampToEdge;

public:
	virtual ~Texture();
	int GetWidth() const;
	int GetHeight() const;
	int GetFormat() const;
	int GetInternalFormat() const;
	uint GetTextureId() const;
	ETexEnvMode GetEnvMode() const;
	ETexFilterMode GetFilterMode() const;
	ETexWrapMode GetWrapMode() const;
	Texture* Bind();
	Texture* SetEnvMode(ETexEnvMode mode);
	Texture* SetWrapMode(ETexWrapMode mode);
	Texture* SetFilterMode(ETexFilterMode mode);
	Texture* UpdateData(int width, int height, int format, int internalFormat, UCHAR* data, bool mipmaps);
	void Release();
};

END_NAMESPACE_ENGINE

#endif