#include "TgaFile.h"

CTgaFile::CTgaFile()
{
}


CTgaFile::~CTgaFile()
{
}

void CTgaFile::SwapRedBlue()
{
	switch (m_colorDepth)
	{
	case 32:
	{
			   unsigned char temp;
			   rgba_t* source = (rgba_t*)info.data;

			   for (int pixel = 0; pixel < (info.width * info.height); ++pixel)
			   {
				   temp = source[pixel].b;
				   source[pixel].b = source[pixel].r;
				   source[pixel].r = temp;
			   }
	} break;
	case 24:
	{
			   unsigned char temp;
			   rgb_t* source = (rgb_t*)info.data;

			   for (int pixel = 0; pixel < (info.width * info.height); ++pixel)
			   {
				   temp = source[pixel].b;
				   source[pixel].b = source[pixel].r;
				   source[pixel].r = temp;
			   }
	} break;
	default:
		// ignore other color depths
		break;
	}
}

bool CTgaFile::FlipVertical()
{
	if (!info.data)
		return false;

	if (m_colorDepth == 32)
	{
		rgba_t* tmpBits = new rgba_t[info.width];
		if (!tmpBits)
			return false;

		int lineWidth = info.width * 4;

		rgba_t* top = (rgba_t*)info.data;
		rgba_t* bottom = (rgba_t*)(info.data + lineWidth*(info.height - 1));

		for (int i = 0; i < (info.height / 2); ++i)
		{
			memcpy(tmpBits, top, lineWidth);
			memcpy(top, bottom, lineWidth);
			memcpy(bottom, tmpBits, lineWidth);

			top = (rgba_t*)((unsigned char*)top + lineWidth);
			bottom = (rgba_t*)((unsigned char*)bottom - lineWidth);
		}

		delete[] tmpBits;
		tmpBits = 0;
	}
	else if (m_colorDepth == 24)
	{
		rgb_t* tmpBits = new rgb_t[info.width];
		if (!tmpBits)
			return false;

		int lineWidth = info.width * 3;

		rgb_t* top = (rgb_t*)info.data;
		rgb_t* bottom = (rgb_t*)(info.data + lineWidth*(info.height - 1));

		for (int i = 0; i < (info.height / 2); ++i)
		{
			memcpy(tmpBits, top, lineWidth);
			memcpy(top, bottom, lineWidth);
			memcpy(bottom, tmpBits, lineWidth);

			top = (rgb_t*)((unsigned char*)top + lineWidth);
			bottom = (rgb_t*)((unsigned char*)bottom - lineWidth);
		}

		delete[] tmpBits;
		tmpBits = 0;
	}

	return true;
}

void CTgaFile::LoadFromFile(const char* filename)
{
	FILE *pFile = fopen(filename, "rb");

	if (!pFile)
		return;

	tgaheader_t tgaHeader;

	// read the TGA header
	fread(&tgaHeader, 1, sizeof(tgaheader_t), pFile);

	// see if the image type is one that we support (RGB, RGB RLE, GRAYSCALE, GRAYSCALE RLE)
	if (((tgaHeader.imageTypeCode != TGA_RGB) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE) &&
		(tgaHeader.imageTypeCode != TGA_RGB_RLE) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE_RLE)) ||
		tgaHeader.colorMapType != 0)
	{
		fclose(pFile);
		return;
	}

	// get image width and height
	info.width = tgaHeader.width;
	info.height = tgaHeader.height;

	// colormode -> 3 = BGR, 4 = BGRA
	int colorMode = tgaHeader.bpp / 8;

	// we don't handle less than 24 bit
	if (colorMode < 3)
	{
		fclose(pFile);
		return;
	}

	m_imageSize = info.width * info.height * colorMode;

	// allocate memory for TGA image data
	info.data = new unsigned char[m_imageSize];

	// skip past the id if there is one
	if (tgaHeader.idLength > 0)
		fseek(pFile, SEEK_CUR, tgaHeader.idLength);

	// read image data
	if (tgaHeader.imageTypeCode == TGA_RGB || tgaHeader.imageTypeCode == TGA_GRAYSCALE)
	{
		fread(info.data, 1, m_imageSize, pFile);
	}
	else
	{
		// this is an RLE compressed image
		unsigned char id;
		unsigned char length;
		rgba_t color = { 0, 0, 0, 0 };
		unsigned int i = 0;

		while (i < m_imageSize)
		{
			id = fgetc(pFile);

			// see if this is run length data
			if (id >= 128)// & 0x80)
			{
				// find the run length
				length = (unsigned char)(id - 127);

				// next 3 (or 4) bytes are the repeated values
				color.b = (unsigned char)fgetc(pFile);
				color.g = (unsigned char)fgetc(pFile);
				color.r = (unsigned char)fgetc(pFile);

				if (colorMode == 4)
					color.a = (unsigned char)fgetc(pFile);

				// save everything in this run
				while (length > 0)
				{
					info.data[i++] = color.b;
					info.data[i++] = color.g;
					info.data[i++] = color.r;

					if (colorMode == 4)
						info.data[i++] = color.a;

					--length;
				}
			}
			else
			{
				// the number of non RLE pixels
				length = unsigned char(id + 1);

				while (length > 0)
				{
					color.b = (unsigned char)fgetc(pFile);
					color.g = (unsigned char)fgetc(pFile);
					color.r = (unsigned char)fgetc(pFile);

					if (colorMode == 4)
						color.a = (unsigned char)fgetc(pFile);

					info.data[i++] = color.b;
					info.data[i++] = color.g;
					info.data[i++] = color.r;

					if (colorMode == 4)
						info.data[i++] = color.a;

					--length;
				}
			}
		}
	}

	fclose(pFile);

	switch (tgaHeader.imageTypeCode)
	{
	case TGA_RGB:
	case TGA_RGB_RLE:
		if (3 == colorMode)
		{
			info.format = GL_RGB;
			info.internalFormat = GL_RGB;
			m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
			m_colorDepth = 24;
		}
		else
		{
			info.format = GL_RGBA;
			info.internalFormat = GL_RGBA;
			m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
			m_colorDepth = 32;
		}
		break;

	case TGA_GRAYSCALE:
	case TGA_GRAYSCALE_RLE:
		info.format = IMAGE_LUMINANCE;
		info.internalFormat = IMAGE_LUMINANCE;
		m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
		m_colorDepth = 8;
		break;
	}

	if ((tgaHeader.imageDesc & TOP_LEFT) == TOP_LEFT)
		FlipVertical();

	// swap the red and blue components in the image data
	SwapRedBlue();
}
