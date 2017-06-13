#include "BitImage.h"

#ifndef GL_BGR_EXT
#define GL_BGR_EXT                        0x80E0
#endif
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT                       0x80E1
#endif

#pragma comment(lib, "lib/FreeImage.lib")

CBitImage::CBitImage()
{
}

CBitImage::CBitImage(const char* filename)
{
	LoadFromFile(filename);
}

CBitImage::~CBitImage()
{
	Release();
}

void CBitImage::LoadFromFile(const char* filename)
{
	Release();
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

uint CBitImage::GetWidth()
{
	return FreeImage_GetWidth(m_pFI);
}

uint CBitImage::GetHeight()
{
	return FreeImage_GetHeight(m_pFI);
}

uint CBitImage::GetBPP()
{
	return FreeImage_GetBPP(m_pFI);
}

int CBitImage::GetFormat()
{
	return m_format;
}

int CBitImage::GetInternalFormat()
{
	return m_internalFormat;
}

BYTE* CBitImage::GetBytes()
{
	return FreeImage_GetBits(m_pFI);
}

void CBitImage::Release()
{
	if (m_pFI) {
		FreeImage_Unload(m_pFI);
		m_pFI = NULL;
	}
}