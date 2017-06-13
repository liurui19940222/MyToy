#ifndef _CIMAGE_FILE_
#define _CIMAGE_FILE_

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <string>  
//#include <libpng\png.h>
#include "Debug.h"
#include "SourceFile.h"

struct SImageInfo {
public:
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLint internalFormat;
	GLubyte* data;
};

class CImageFile : public CSourceFile
{
protected:
	SImageInfo info;

public:
	CImageFile();
	~CImageFile();

	SImageInfo* GetImageInfo();

	GLsizei GetWidth();

	GLsizei GetHeight();

	GLenum GetFormat();

	GLint GetInternalFormat();

	GLubyte* GetData();

	virtual void Release();
};

#endif