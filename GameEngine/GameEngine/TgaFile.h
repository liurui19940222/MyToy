#ifndef _CTGA_FILE_
#define _CTGA_FILE_

#include"ImageFile.h"

enum TGATypes
{
	TGA_NODATA = 0,
	TGA_INDEXED = 1,
	TGA_RGB = 2,
	TGA_GRAYSCALE = 3,
	TGA_INDEXED_RLE = 9,
	TGA_RGB_RLE = 10,
	TGA_GRAYSCALE_RLE = 11
};

// Image Data Formats
#define	IMAGE_RGB       0
#define IMAGE_RGBA      1
#define IMAGE_LUMINANCE 2

// Image data types
#define	IMAGE_DATA_UNSIGNED_BYTE 0

// Pixel data transfer from file to screen:
// These masks are AND'd with the imageDesc in the TGA header,
// bit 4 is left-to-right ordering
// bit 5 is top-to-bottom
#define BOTTOM_LEFT  0x00	// first pixel is bottom left corner
#define BOTTOM_RIGHT 0x10	// first pixel is bottom right corner
#define TOP_LEFT     0x20	// first pixel is top left corner
#define TOP_RIGHT    0x30	// first pixel is top right corner

// TGA header
struct tgaheader_t
{
	unsigned char  idLength;
	unsigned char  colorMapType;
	unsigned char  imageTypeCode;
	unsigned char  colorMapSpec[5];
	unsigned short xOrigin;
	unsigned short yOrigin;
	unsigned short width;
	unsigned short height;
	unsigned char  bpp;
	unsigned char  imageDesc;
};

struct rgba_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct rgb_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class CTgaFile : public CImageFile
{
private:
	unsigned char  m_colorDepth;
	unsigned char  m_imageDataType;
	unsigned long  m_imageSize;

	void SwapRedBlue();

	bool FlipVertical();

public:
	CTgaFile();
	~CTgaFile();

	virtual void LoadFromFile(const char* filename);
};

#endif