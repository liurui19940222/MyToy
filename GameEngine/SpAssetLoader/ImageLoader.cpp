#include "ImageLoader.h"

using namespace spgameengine;

#ifndef GL_BGR_EXT
#define GL_BGR_EXT                        0x80E0
#endif
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT                       0x80E1
#endif

ImageLoader::ImageLoader()
{
}

ImageLoader::ImageLoader(const char* filename)
{
	LoadFromFile(filename);
}

ImageLoader::~ImageLoader()
{
	ReleaseSource();
}

void ImageLoader::LoadFromFile(const char* filename)
{
	ReleaseSource();
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(filename);

	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		m_pFI = FreeImage_Load(fif, filename, 0);
	}

	if (m_pFI)
	{
		uint bpp = GetBPP();
		m_internalFormat = bpp >> 3;
		if (bpp == 24)
			m_format = GL_BGR_EXT;
		else if (bpp == 32)
			m_format = GL_BGRA_EXT;
	}
}

uint ImageLoader::GetWidth()
{
	return FreeImage_GetWidth(m_pFI);
}

uint ImageLoader::GetHeight()
{
	return FreeImage_GetHeight(m_pFI);
}

uint ImageLoader::GetBPP()
{
	return FreeImage_GetBPP(m_pFI);
}

int ImageLoader::GetFormat()
{
	return m_format;
}

int ImageLoader::GetInternalFormat()
{
	return m_internalFormat;
}

BYTE* ImageLoader::GetBytes()
{
	return FreeImage_GetBits(m_pFI);
}

void ImageLoader::ReleaseSource()
{
	if (m_pFI) {
		FreeImage_Unload(m_pFI);
		m_pFI = NULL;
	}
}

bool ImageLoader::ChangeSize(int width, int height)
{
	if (m_pFI)
	{
		FIBITMAP* bm = FreeImage_Rescale(m_pFI, width, height);
		if (bm)
		{
			ReleaseSource();
			m_pFI = bm;
			return true;
		}
	}
	return false;
}

bool ImageLoader::Save(const char* path, FREE_IMAGE_FORMAT fif)
{
	if (m_pFI)
	{
		return FreeImage_Save(fif, m_pFI, path) == 1;
	}
	return false;
}

ImageLoader* ImageLoader::Create(int width, int height, int bpp, BYTE* data)
{
	ImageLoader* image = new ImageLoader();
	image->m_pFI = FreeImage_Allocate(width, height, bpp);
	BYTE* bytes = FreeImage_GetBits(image->m_pFI);
	int pixelSize = bpp >> 3;
	memcpy(bytes, data, width * height * pixelSize);
	BYTE temp = 0;

	//swap r with b
	for (int i = 0; i < width * height * pixelSize; i += pixelSize)
	{
		temp = bytes[i];
		bytes[i] = bytes[i + 2];
		bytes[i + 2] = temp;
	}
	return image;
}