#ifndef _BIT_IMAGE_H_
#define _BIT_IMAGE_H_

#include"IAssetLoader.h"
#include"..\SpCommon\EngineDefine.h"
#include<freeimage\FreeImage.h>

class ImageLoader : public IAssetLoader
{
	FIBITMAP* m_pFI;

	int m_format;

	int m_internalFormat;

public:
	ImageLoader();

	ImageLoader(const char* filename);
	~ImageLoader();

	virtual void LoadFromFile(const char* filename) override;

	virtual void ReleaseSource() override;

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

#endif