#include "Texture.h"

USING_NAMESPACE_ENGINE;

TexcoordRange::TexcoordRange() : m_StartingPoint(0.0f, 0.0f), m_Size(0.0f, 0.0f) {}

TexcoordRange::TexcoordRange(float startingX, float startingY, float sizeX, float sizeY)
	: m_StartingPoint(startingX, startingY), m_Size(sizeX, sizeY) {}

TexcoordRange::TexcoordRange(Vector2 staringPoint, Vector2 size)
	: m_StartingPoint(staringPoint), m_Size(size) {}

void TexcoordRange::Slice(BVector4 border, vector<TexcoordRange>& out, float internalWidth, float internalHeight, float rawWidth, float rawHeight)
{
	out.resize(9);
	float w1, w2, w3, h1, h2, h3, t1, t2;
	t1 = 1.0f / rawWidth;
	t2 = 1.0f / rawHeight;
	w1 = border.left * t1;
	w2 = (internalWidth - border.left - border.right) * t1;
	w3 = border.right * t1;
	h1 = border.top * t2;
	h2 = (internalHeight - border.top - border.bottom) * t2;
	h3 = border.bottom * t2;

	out[0].m_StartingPoint.x = m_StartingPoint.x;
	out[0].m_StartingPoint.y = m_StartingPoint.y + m_Size.y - h1;
	out[0].m_Size.x = w1;
	out[0].m_Size.y = h1;

	out[1].m_StartingPoint.x = m_StartingPoint.x + w1;
	out[1].m_StartingPoint.y = m_StartingPoint.y + m_Size.y - h1;
	out[1].m_Size.x = w2;
	out[1].m_Size.y = h1;

	out[2].m_StartingPoint.x = m_StartingPoint.x + w1 + w2;
	out[2].m_StartingPoint.y = m_StartingPoint.y + m_Size.y - h1;
	out[2].m_Size.x = w3;
	out[2].m_Size.y = h1;

	out[3].m_StartingPoint.x = m_StartingPoint.x;
	out[3].m_StartingPoint.y = m_StartingPoint.y + h1;
	out[3].m_Size.x = w1;
	out[3].m_Size.y = h2;

	out[4].m_StartingPoint.x = m_StartingPoint.x + w1;
	out[4].m_StartingPoint.y = m_StartingPoint.y + h1;
	out[4].m_Size.x = w2;
	out[4].m_Size.y = h2;

	out[5].m_StartingPoint.x = m_StartingPoint.x + w1 + w2;
	out[5].m_StartingPoint.y = m_StartingPoint.y + h1;
	out[5].m_Size.x = w3;
	out[5].m_Size.y = h2;

	out[6].m_StartingPoint.x = m_StartingPoint.x;
	out[6].m_StartingPoint.y = m_StartingPoint.y;
	out[6].m_Size.x = w1;
	out[6].m_Size.y = h3;

	out[7].m_StartingPoint.x = m_StartingPoint.x + w1;
	out[7].m_StartingPoint.y = m_StartingPoint.y;
	out[7].m_Size.x = w2;
	out[7].m_Size.y = h3;

	out[8].m_StartingPoint.x = m_StartingPoint.x + w1 + w2;
	out[8].m_StartingPoint.y = m_StartingPoint.y;
	out[8].m_Size.x = w3;
	out[8].m_Size.y = h3;
}

const TexcoordRange TexcoordRange::none(0.0f, 0.0f, 0.0f, 0.0f);
const TexcoordRange TexcoordRange::full(0.0f, 0.0f, 1.0f, 1.0f);

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

Texture* Texture::UpdateData(int width, int height, int format, int internalFormat, UCHAR* data, bool mipmaps)
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	if (mipmaps)
	{
		if (data) gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, data);
	}
	else  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
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