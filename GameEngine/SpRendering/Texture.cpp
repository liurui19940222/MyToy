#include "Texture.h"

CTexture::~CTexture()
{
	Release();
}

CTexture* CTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_envMode);
	return this;
}

CTexture* CTexture::SetEnvMode(ETexEnvMode mode)
{
	m_envMode = mode;
	return this;
}

CTexture* CTexture::SetWrapMode(ETexWrapMode mode)
{
	m_wrapMode = mode;
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	return this;
}

CTexture* CTexture::SetFilterMode(ETexFilterMode mode)
{
	m_filterMode = mode;
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
	return this;
}

int CTexture::GetWidth() const { return m_width; }
int CTexture::GetHeight() const { return m_height; }
int CTexture::GetFormat() const { return m_format; }
int CTexture::GetInternalFormat() const { return m_internalFormat; }
uint CTexture::GetTextureId() const { return m_texId; }
ETexEnvMode CTexture::GetEnvMode() const { return m_envMode; }
ETexFilterMode CTexture::GetFilterMode() const { return m_filterMode; }
ETexWrapMode CTexture::GetWrapMode() const { return m_wrapMode; }

void CTexture::Release()
{
	if (m_texId)
	{
		glDeleteTextures(1, &m_texId);
		m_texId = 0;
	}
}