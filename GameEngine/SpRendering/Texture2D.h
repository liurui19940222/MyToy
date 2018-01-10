#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include"..\SpAssetLoader\ImageLoader.h"
#include"Texture.h"

enum EStoreTexture2DId
{
	Black8x8 = 0,
	White8x8 = 1,
};

class CTexture2D : public CTexture
{
private:
	static CTexture2D* m_store[2];

	static CTexture2D* Init(CTexture2D* texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data);

public:
	static CTexture2D* Create(const char* filename);
	static CTexture2D* Create(ImageLoader* image);
	static CTexture2D* Create(const char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture2D* Create(ImageLoader* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture2D* Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
	static CTexture2D* Create(UCHAR* pixels, int width, int height);

	static CTexture2D* GetOneInStore(EStoreTexture2DId id);
};

#endif