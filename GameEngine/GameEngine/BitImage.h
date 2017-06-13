#ifndef _BIT_IMAGE_H_
#define _BIT_IMAGE_H_

#include"SourceFile.h"
#include"EngineDefine.h"
#include<freeimage\FreeImage.h>

class CBitImage : public CSourceFile
{
	FIBITMAP* m_pFI;

	int m_format;

	int m_internalFormat;

public:
	CBitImage();

	CBitImage(const char* filename);
	~CBitImage();

	virtual void LoadFromFile(const char* filename) override;

	virtual void Release() override;

	uint GetWidth();

	uint GetHeight();

	uint GetBPP();

	int GetFormat();

	int GetInternalFormat();

	BYTE* GetBytes();
};

#endif