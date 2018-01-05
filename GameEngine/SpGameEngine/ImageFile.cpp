#include "ImageFile.h"


CImageFile::CImageFile()
{
}


CImageFile::~CImageFile()
{
}

SImageInfo* CImageFile::GetImageInfo()
{
	return &info;
}

GLsizei CImageFile::GetWidth()
{
	return info.width;
}

GLsizei CImageFile::GetHeight()
{
	return info.height;
}

GLenum CImageFile::GetFormat()
{
	return info.format;
}

GLint CImageFile::GetInternalFormat()
{
	return info.internalFormat;
}

GLubyte* CImageFile::GetData()
{
	return info.data;
}

void CImageFile::Release()
{
	if (info.data)
		free(info.data);
}