#ifndef _BIT_IMAGE_H_
#define _BIT_IMAGE_H_

#include"SourceFile.h"
#include"EngineDefine.h"
#include"DynamicFactory.h"
#include<freeimage\FreeImage.h>

class ENGINE_API CBitImage : public ISourceFile
{
	REFLECT_CLASS(CBitImage)
	FIBITMAP* m_pFI;

	int m_format;

	int m_internalFormat;

public:
	CBitImage();

	CBitImage(const char* filename);
	~CBitImage();

	virtual void LoadFromFile(const char* filename) override;

	virtual void ReleaseSource() override;

	uint GetWidth();

	uint GetHeight();

	uint GetBPP();

	int GetFormat();

	int GetInternalFormat();

	BYTE* GetBytes();

	bool Save(const char* path, FREE_IMAGE_FORMAT fif);

	static CBitImage* Create(int width, int height, int bpp, BYTE* data);
};

#endif