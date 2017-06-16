#include "Atlas.h"
#include<math.h>

CAtlas::CAtlas(int width, int height)
{
	static int id = 1;
	instanceId = id++;
	bitmap.width = width;
	bitmap.height = height;
	m_width = width;
	m_height = height;
	bitmap.buffer = (uint32*)malloc(sizeof(uint32) * width * height);
	memset(bitmap.buffer, 0, sizeof(uint32) * width * height);
	endingPos.x = 0;
	endingPos.y = 0;
}

bool CAtlas::Push(int width, int height, int max_height, uint8* grey_buffer, RGB rgb, Rect2D* out_rect_in_atlas)
{
	if (!BeginPush(width, height, max_height) || !grey_buffer)
		return false;
	if (out_rect_in_atlas)
	{
		out_rect_in_atlas->x = endingPos.x;
		out_rect_in_atlas->y = endingPos.y;
		out_rect_in_atlas->width = width;
		out_rect_in_atlas->height = height;
	}
	Set(endingPos.x, endingPos.y, width, height, grey_buffer, rgb);
	EndPush(width, height, max_height);
	return true;
}

void CAtlas::Set(int target_x, int target_y, int width, int height, uint8* grey_buffer, RGB rgb)
{
	for (int y = target_y, index_y = 0; y < target_y + height; y++, index_y++)
	{
		for (int x = target_x, index_x = 0; x < target_x + width; x++, index_x++)
		{
			if (x >= m_width || x < 0 || y >= m_height || y < 0)
				continue;
			uint8* p = grey_buffer + index_x + index_y * width;
			bitmap.buffer[x + y * m_width] = _RGBA32(rgb.r, rgb.g, rgb.b, *p);
		}
	}
}

bool CAtlas::BeginPush(int width, int height, int max_height)
{
	int y = endingPos.y;
	if (endingPos.x + width >= m_width)
	{
		y = endingPos.y + max_height;
		endingPos.x = 0;
	}

	if (y >= m_height)
	{
		return false;
	}
	endingPos.y = y;
	return true;
}

bool CAtlas::TryPush(int width, int height, int max_height)
{
	int y = endingPos.y + height;
	if (endingPos.x + width >= m_width)
	{
		y = endingPos.y + max_height * 2;
	}

	if (y >= m_height)
	{
		return false;
	}
	return true;
}

void CAtlas::EndPush(int width, int height, int max_height)
{
	endingPos.x += width;
	if (endingPos.x >= m_width)
	{
		endingPos.x = 0;
		endingPos.y += max_height;
	}
}

bool CAtlas::Get(Rect2D* rect_in_atlas, uint32* out_pixels, Color color)
{
	if (!rect_in_atlas || !out_pixels)
		return false;

	if (rect_in_atlas->x < 0 || rect_in_atlas->x + rect_in_atlas->width >= m_width || rect_in_atlas->y < 0 || rect_in_atlas->y + rect_in_atlas->height >= m_height)
		return false;

	int width = rect_in_atlas->width;
	int height = rect_in_atlas->height;
	int start_x = rect_in_atlas->x;
	int start_y = rect_in_atlas->y;
	uint color_rgba = Color32(color).ToInt32();
	for (int y = start_y, index_y = 0; y < start_y + height; y++, index_y++)
	{
		for (int x = start_x, index_x = 0; x < start_x + width; x++, index_x++)
		{
			Color32 c = bitmap.buffer[x + y * m_width];
			out_pixels[index_x + index_y * width] = (c * color).ToInt32();
		}
	}

	return true;
}

int CAtlas::GetWidth() { return m_width; }

int CAtlas::GetHeight() { return m_height; }

uint32* CAtlas::GetPixels() { return bitmap.buffer; }

void CAtlas::Clear()
{
	endingPos.x = 0;
	endingPos.y = 0;
	if (bitmap.buffer)
		memset(bitmap.buffer, 0, sizeof(uint32) * m_width * m_height);
}

void CAtlas::Release()
{
	if (bitmap.buffer)
		free(bitmap.buffer);
}