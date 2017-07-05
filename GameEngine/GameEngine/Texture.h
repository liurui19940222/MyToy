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
private:
	UINT texId;
	int width;
	int height;
	ETexEnvMode envMode;

	static CTexture* Init(CTexture* texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data);

public:
	CTexture();
	~CTexture();

	static CTexture* Create(char* filename);
	static CTexture* Create(CBitImage* image);
	static CTexture* Create(char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture* Create(CBitImage* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture* Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture* Create(UCHAR* pixels, int width, int height);

	int GetWidth();
	int GetHeight();
	CTexture* Bind();
	CTexture* SetEnvMode(ETexEnvMode mode);
	CTexture* SetWrapMode(ETexWrapMode mode);
	CTexture* SetFilterMode(ETexFilterMode mode);
	virtual void OnRelease() override;
};

#endif