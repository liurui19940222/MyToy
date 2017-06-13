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
};

class CTexture : public Object, public DynamicCreate<CTexture>
{
private:
	UINT texId;
	int width;
	int height;

	static CTexture* Init(CTexture* texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data);

public:
	CTexture();
	~CTexture();

	static CTexture* Create(char* filename, EImageFormat format);

	static CTexture* Create(CImageFile* image);

	static CTexture* Create(char* filename, EImageFormat format, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);

	static CTexture* Create(CImageFile* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);

	static CTexture* Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);

	static CTexture* Create(UCHAR* pixels, int width, int height);

	int GetWidth();

	int GetHeight();

	void Bind();

	virtual void OnRelease() override;
};

#endif