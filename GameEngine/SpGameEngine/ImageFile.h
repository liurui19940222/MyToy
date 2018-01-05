#ifndef _CIMAGE_FILE_
#define _CIMAGE_FILE_

#include <Windows.h>
#include <gl\glew.h>
#include <string>  
#include "Debug.h"
#include "SourceFile.h"

struct ENGINE_API SImageInfo {
public:
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLint internalFormat;
	GLubyte* data;
};

class ENGINE_API CImageFile : public ISourceFile
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