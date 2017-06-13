#ifndef _CPNG_FILE_
#define _CPNG_FILE_

//#include <libpng\png.h>
#include "ImageFile.h"

//#pragma comment(lib,"libpng/libpng16d.lib")  

class CPngFile : public CImageFile
{
public:
	CPngFile();
	~CPngFile();

	void GetPNGtextureInfo(int color_type, SImageInfo *texinfo);

	int ReadPNGFromFile(const char *filename);

	virtual void LoadFromFile(const char* filename);
};

#endif