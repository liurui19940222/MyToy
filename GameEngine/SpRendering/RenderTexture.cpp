#include"RenderTexture.h"

USING_NAMESPACE_ENGINE;

RenderTexture::RenderTexture() : m_RboHandle(0), m_FboHandle(0)
{
	
}

RenderTexture::~RenderTexture()
{
	if (m_RboHandle) glDeleteRenderbuffers(1, &m_RboHandle);
	if (m_FboHandle) glDeleteFramebuffers(1, &m_FboHandle);
}

void RenderTexture::MakeBuffer(int width, int height, bool depthBuffer)
{
	if (!m_FboHandle) glGenFramebuffers(1, &m_FboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboHandle);

	if (depthBuffer)
	{
		if (!m_RboHandle) glGenRenderbuffers(1, &m_RboHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RboHandle);
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

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RboHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::BindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboHandle);
}

RenderTexturePtr RenderTexture::Create(int width, int height, bool depthBuffer)
{
	RenderTexturePtr texture(new RenderTexture());
	texture->m_width = width;
	texture->m_height = height;
	texture->MakeBuffer(width, height, depthBuffer);
	return texture;
}