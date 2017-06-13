#include "BitmapFile.h"


CBitmapFile::CBitmapFile()
{
}


CBitmapFile::~CBitmapFile()
{
}

void CBitmapFile::LoadFromFileAlpha(const char* filename, Color32 color)
{
	LoadFromFile(filename);
	if (info.data == NULL)
		return;
	unsigned char r, g, b;
	r = color.r;
	g = color.g;
	b = color.b;
	unsigned char *pImage;
	pImage = new unsigned char[info.width * info.height * 4];
	int src, dst;
	for (src = 0, dst = 0; src < info.width * info.height * 3; src += 3, dst += 4)
	{
		if (info.data[src] == r && info.data[src + 1] == g && info.data[src + 2] == b)
		{
			pImage[dst + 3] = 0;
		}
		else
		{
			pImage[dst + 3] = 255;
		}
		pImage[dst] = info.data[src];
		pImage[dst + 1] = info.data[src + 1];
		pImage[dst + 2] = info.data[src + 2];
	}
	if (info.data)
	{
		delete[] info.data;
		info.data = NULL;
	}
	info.data = pImage;
	info.format = GL_RGBA;
	info.internalFormat = GL_RGBA;
}

void CBitmapFile::LoadFromFile(const char* filename)
{
	BITMAPFILEHEADER  bitmapFileHeader;
	BITMAPINFOHEADER  bitmapInfoHeader;
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp == NULL)
		return;
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if (bitmapFileHeader.bfType != 0x4d42)
	{
		fclose(fp);
		return;
	}
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	info.width = bitmapInfoHeader.biWidth;
	info.height = bitmapInfoHeader.biHeight;
	unsigned char *pPalette = NULL;
	if (bitmapInfoHeader.biBitCount == 8)
	{
		pPalette = new unsigned char[256 * 4];
	}
	info.data = new unsigned char[info.width * info.height * 3];
	if (info.data == NULL)
	{
		fclose(fp);
		return;
	}
	unsigned char temp;
	if (bitmapInfoHeader.biBitCount == 24)
	{
		fread(info.data, 1, info.width * info.height * 3, fp);
		for (int i = 0; i < info.width * info.height * 3; i += 3)
		{
			temp = info.data[i];
			info.data[i] = info.data[i + 2];
			info.data[i + 2] = temp;
		}
	}
	else if (bitmapInfoHeader.biBitCount == 8)
	{
		fread(pPalette, 1, 1024, fp);
		for (int j = 0; j < info.width * info.height * 3; j += 3)
		{
			fread(&temp, 1, 1, fp);
			info.data[j] = pPalette[temp * 4 + 2];
			info.data[j + 1] = pPalette[temp * 4 + 1];
			info.data[j + 2] = pPalette[temp * 4];
		}
		delete[] pPalette;
	}

	fclose(fp);
	info.format = GL_RGB;
	info.internalFormat = GL_RGB;
}


