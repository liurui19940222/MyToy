#include "Texture.h"

Texture::~Texture()
{
	Release();
}

Texture* Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_envMode);
	return this;
}

Texture* Texture::SetEnvMode(ETexEnvMode mode)
{
	m_envMode = mode;
	return this;
}

Texture* Texture::SetWrapMode(ETexWrapMode mode)
{
	m_wrapMode = mode;
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	return this;
}

Texture* Texture::SetFilterMode(ETexFilterMode mode)
{
	m_filterMode = mode;
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
	return this;
}

int Texture::GetWidth() const { return m_width; }
int Texture::GetHeight() const { return m_height; }
int Texture::GetFormat() const { return m_format; }
int Texture::GetInternalFormat() const { return m_internalFormat; }
uint Texture::GetTextureId() const { return m_texId; }
ETexEnvMode Texture::GetEnvMode() const { return m_envMode; }
ETexFilterMode Texture::GetFilterMode() const { return m_filterMode; }
ETexWrapMode Texture::GetWrapMode() const { return m_wrapMode; }

void Texture::Release()
{
	if (m_texId)
	{
		glDeleteTextures(1, &m_texId);
		m_texId = 0;
	}
}