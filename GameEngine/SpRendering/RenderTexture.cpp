#include"RenderTexture.h"

void RenderTexture::MakeBuffer(int width, int height, bool depthBuffer)
{
	if (!m_fboHandle) glGenFramebuffers(1, &m_fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

	if (depthBuffer)
	{
		if (!m_rboHandle) glGenRenderbuffers(1, &m_rboHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rboHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	if (!m_texId) glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMode);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_format = GL_RGBA;
	m_internalFormat = GL_RGBA8;

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::BindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
}

void RenderTexture::ReleaseBuffer()
{
	if (m_rboHandle) glDeleteRenderbuffers(1, &m_rboHandle);
	if (m_fboHandle) glDeleteFramebuffers(1, &m_fboHandle);
}

PRenderTexture RenderTexture::Create(int width, int height, bool depthBuffer)
{
	PRenderTexture texture(new RenderTexture());
	texture->m_width = width;
	texture->m_height = height;
	texture->MakeBuffer(width, height, depthBuffer);
	return texture;
}