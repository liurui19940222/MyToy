#ifndef _CBITMAP_FILE_
#define _CBITMAP_FILE_

#include<Windows.h>
#include<windowsx.h>
#include"ImageFile.h"
#include"EngineDefine.h"

class CBitmapFile : public CImageFile
{
public:
	CBitmapFile();
	~CBitmapFile();

	void LoadFromFileAlpha(const char* filename, Color32 color);

	virtual void LoadFromFile(const char* filename);
};

#endif