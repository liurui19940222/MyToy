#ifndef _BIT_IMAGE_H_
#define _BIT_IMAGE_H_

#include"SpCommon\EngineDefine.h"
#include<freeimage\FreeImage.h>

BEGIN_NAMESPACE_ENGINE

class ImageLoader
{
	FIBITMAP* m_pFI;

	int m_format;

	int m_internalFormat;

public:
	ImageLoader();

	ImageLoader(const char* filename);
	~ImageLoader();

	virtual void LoadFromFile(const char* filename);

	virtual void ReleaseSource();

	uint GetWidth();

	uint GetHeight();

	uint GetBPP();

	int GetFormat();

	int GetInternalFormat();

	BYTE* GetBytes();

	bool ChangeSize(int width, int height);

	bool Save(const char* path, FREE_IMAGE_FORMAT fif);

	static ImageLoader* Create(int width, int height, int bpp, BYTE* data);
};

END_NAMESPACE_ENGINE

#endif