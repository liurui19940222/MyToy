#include "Texture2D.h"
#include "Application.h"

IMPL_CLASS(CTexture2D)

CTexture2D* CTexture2D::m_store[2] = { NULL, NULL };

CTexture2D* CTexture2D::Init(CTexture2D* texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data)
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

CTexture2D* CTexture2D::Create(const char* filename)
{
	CBitImage* image = Resources->Load<CBitImage>(filename);
	CTexture2D* texture = Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
	Resources->Unload(string(filename));
	return texture;
}

CTexture2D* CTexture2D::Create(CBitImage* image)
{
	return Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

CTexture2D* CTexture2D::Create(const char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CBitImage* image = Resources->Load<CBitImage>(string(filename));
	CTexture2D* texture = Create(image, wrapMode, filterMode, envMode, mipmaps);
	Resources->Unload(string(filename));
	return texture;
}

CTexture2D* CTexture2D::Create(CBitImage* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CTexture2D* texture = Maker->Instantiate<CTexture2D>();
	Init(texture, wrapMode, filterMode, envMode, mipmaps, image->GetWidth(), image->GetHeight(), image->GetFormat(), image->GetInternalFormat(), image->GetBytes());
	return texture;
}

CTexture2D* CTexture2D::Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CTexture2D* texture = Maker->Instantiate<CTexture2D>();
	Init(texture, wrapMode, filterMode, envMode, mipmaps, width, height, GL_RGBA, GL_RGBA, pixels);
	return texture;
}

CTexture2D* CTexture2D::Create(UCHAR* pixels, int width, int height)
{
	return Create(pixels, width, height, ETexWrapMode::Repeat, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

CTexture2D* CTexture2D::GetOneInStore(EStoreTexture2DId id)
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
	}
	return m_store[id];
}