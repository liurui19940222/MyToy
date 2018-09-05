#include "Texture2D.h"

USING_NAMESPACE_ENGINE;

IMPL_RTTI(Texture2D, Texture::GetMetadata(), {
	PROP(Texture2D, m_AssetRef, EType::String)
})

Texture2DPtr Texture2D::m_store[2] = { NULL, NULL };

Texture2DPtr Texture2D::Init(Texture2DPtr texture, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps, int width, int height, int format, int internalFormat, UCHAR* data)
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
	texture->UpdateData(width, height, format, internalFormat, data, mipmaps);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

Texture2DPtr Texture2D::Create(const char* filename, bool mipmaps)
{
	if (!filename) return NULL;
	ImageLoader image(filename);
	Texture2DPtr texture = Create(&image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, mipmaps);
	texture->m_AssetRef = filename;
	return texture;
}

Texture2DPtr Texture2D::Create(ImageLoader* image)
{
	return Create(image, ETexWrapMode::ClampToEdge, ETexFilterMode::Linear, ETexEnvMode::Replace, false);
}

Texture2DPtr Texture2D::Create(const char* filename, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	if (!filename) return NULL;
	ImageLoader image(filename);
	Texture2DPtr texture = Create(&image, wrapMode, filterMode, envMode, mipmaps);
	texture->m_AssetRef = filename;
	return texture;
}

Texture2DPtr Texture2D::Create(ImageLoader* image, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	Texture2DPtr texture(new Texture2D());
	Init(texture, wrapMode, filterMode, envMode, mipmaps, image->GetWidth(), image->GetHeight(), image->GetFormat(), image->GetInternalFormat(), image->GetBytes());
	return texture;
}

Texture2DPtr Texture2D::Create(UCHAR* pixels, int width, int height, ETexWrapMode wrapMode, ETexFilterMode filterMode, ETexEnvMode envMode, bool mipmaps)
{
	Texture2DPtr texture(new Texture2D());
	Init(texture, wrapMode, filterMode, envMode, mipmaps, width, height, GL_RGBA, GL_RGBA, pixels);
	return texture;
}

Texture2DPtr Texture2D::Create(UCHAR* pixels, int width, int height, bool mipmaps)
{
	return Create(pixels, width, height, ETexWrapMode::Repeat, ETexFilterMode::Linear, ETexEnvMode::Replace, mipmaps);
}

Texture2DPtr Texture2D::GetOneInStore(EStoreTexture2DId id)
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

void Texture2D::OnSerialize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
{
	Texture::OnSerialize(depth, meta, value, allocator);
}

void Texture2D::OnDeserialize(int depth, const Metadata* meta, Value& value)
{
	Texture::OnDeserialize(depth, meta, value);
	Release();
	if (!m_AssetRef.empty())
	{
		ImageLoader loader(m_AssetRef.c_str());
		Init(dynamic_pointer_cast<Texture2D>(shared_from_this()), m_wrapMode, m_filterMode, m_envMode, m_mipmaps, loader.GetWidth(), 
			loader.GetHeight(), loader.GetFormat(), loader.GetInternalFormat(), loader.GetBytes());
	}
}