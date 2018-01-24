#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include"SpAssetLoader\ImageLoader.h"
#include"Texture.h"

namespace spgameengine
{

	enum EStoreTexture2DId
	{
		Black8x8 = 0,
		White8x8 = 1,
	};

	SMART_CLASS(Texture2D) class Texture2D : public Texture
	{

	private:
		static PTexture2D m_store[2];

		static PTexture2D Init(PTexture2D texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data);

	public:
		static PTexture2D Create(const char* filename);
		static PTexture2D Create(ImageLoader* image);
		static PTexture2D Create(const char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
		static PTexture2D Create(ImageLoader* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
		static PTexture2D Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps);
		static PTexture2D Create(UCHAR* pixels, int width, int height);

		static PTexture2D GetOneInStore(EStoreTexture2DId id);
	};

}

#endif