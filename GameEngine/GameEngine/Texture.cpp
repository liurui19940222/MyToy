#include "Texture.h"
#include "Application.h"

IMPL_CLASS(CTexture)

CTexture::CTexture() : Object()
{
}


CTexture::~CTexture()
{
}

CTexture* CTexture::Init(CTexture* texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data)
{
	texture->width = width;
	texture->height = height;
	glGenTextures(1, &texture->texId);
	glBindTexture(GL_TEXTURE_2D, texture->texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envMode);
	if (mipmaps)
		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	return texture;
}

CTexture* CTexture::Create(char* filename)
{
	CBitImage* image = CApplication::GetEngine()->CreateImage(filename);
	CTexture* texture = Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
	CApplication::GetEngine()->ReleaseImage(image);
	return texture;
}

CTexture* CTexture::Create(CBitImage* image)
{
	return Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

CTexture* CTexture::Create(char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CBitImage* image = CApplication::GetEngine()->CreateImage(filename);
	CTexture* texture = Create(image, wrapMode, filterMode, envMode, mipmaps);
	CApplication::GetEngine()->ReleaseImage(image);
	return texture;
}

CTexture* CTexture::Create(CBitImage* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CTexture* texture = CApplication::GetEngine()->CreateObject<CTexture>();
	Init(texture, wrapMode, filterMode, envMode, mipmaps, image->GetWidth(), image->GetHeight(), image->GetFormat(), image->GetInternalFormat(), image->GetBytes());
	return texture;
}

CTexture* CTexture::Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	CTexture* texture = CApplication::GetEngine()->CreateObject<CTexture>();
	Init(texture, wrapMode, filterMode, envMode, mipmaps, width, height, GL_RGBA, GL_RGBA, pixels);
	return texture;
}

CTexture* CTexture::Create(UCHAR* pixels, int width, int height)
{
	return Create(pixels, width, height, ETexWrapMode::Repeat, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

void CTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texId);
}

int CTexture::GetWidth()
{
	return width;
}

int CTexture::GetHeight()
{
	return height;
}

void CTexture::OnRelease()
{
	glDeleteTextures(1, &texId);
}