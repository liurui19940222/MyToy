#include "Texture2D.h"

using namespace spgameengine;

PTexture2D Texture2D::m_store[2] = { NULL, NULL };

PTexture2D Texture2D::Init(PTexture2D texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data)
{
	glEnable(GL_TEXTURE_2D);
	texture->m_wrapMode = wrapMode;
	texture->m_filterMode = filterMode;
	texture->m_envMode = envMode;
	texture->m_width = width;
	texture->m_height = height;
	glGenTextures(1, &texture->m_texId);
	glBindTexture(GL_TEXTURE_2D, texture->m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	if (mipmaps)
		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	return texture;
}

PTexture2D Texture2D::Create(const char* filename)
{
	if (!filename) return NULL;
	ImageLoader* image = new ImageLoader(filename);
	PTexture2D texture = Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
	image->ReleaseSource();
	delete image;
	return texture;
}

PTexture2D Texture2D::Create(ImageLoader* image)
{
	return Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

PTexture2D Texture2D::Create(const char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	if (!filename) return NULL;
	ImageLoader* image = new ImageLoader(filename);
	PTexture2D texture = Create(image, wrapMode, filterMode, envMode, mipmaps);
	image->ReleaseSource();
	delete image;
	return texture;
}

PTexture2D Texture2D::Create(ImageLoader* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	PTexture2D texture(new Texture2D());
	Init(texture, wrapMode, filterMode, envMode, mipmaps, image->GetWidth(), image->GetHeight(), image->GetFormat(), image->GetInternalFormat(), image->GetBytes());
	return texture;
}

PTexture2D Texture2D::Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	PTexture2D texture(new Texture2D());
	Init(texture, wrapMode, filterMode, envMode, mipmaps, width, height, GL_RGBA, GL_RGBA, pixels);
	return texture;
}

PTexture2D Texture2D::Create(UCHAR* pixels, int width, int height)
{
	return Create(pixels, width, height, ETexWrapMode::Repeat, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

PTexture2D Texture2D::GetOneInStore(EStoreTexture2DId id)
{
	if (m_store[id] == NULL)
	{
		size_t size = 8 * 8 * sizeof(UINT);
		UCHAR* pixels = (UCHAR*)malloc(size);
		if(id == EStoreTexture2DId::White8x8)
			memset(pixels, 255, size);
		else
			memset(pixels, 0, size);
		m_store[id] = Create(pixels, 8, 8);
		free(pixels);
	}
	return m_store[id];
}