#ifndef _CTEXTURE_
#define _CTEXTURE_

#include"Object.h"
#include"GLHead.h"
#include"FileHead.h"
#include"DynamicFactory.h"

enum ETexWrapMode
{
	Clamp = 0x2900,
	ClampToEdge = 1,
	Repeat = 0x2901,
};

enum ETexFilterMode
{
	Linear = 0x2601,
};

enum ETexEnvMode
{
	Replace = 0x1E01,
	Modulate = 0x2100,
	Add = 0x0104,
};

class CTexture : public Object
{
	REFLECT_CLASS(CTexture)
protected:
	UINT m_texId;
	int m_width;
	int m_height;
	int m_format;
	int m_internalFormat;
	ETexEnvMode m_envMode;
	ETexFilterMode m_filterMode;
	ETexWrapMode m_wrapMode;

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