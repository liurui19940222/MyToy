#include "Atlas.h"
#include <math.h>

USING_NAMESPACE_ENGINE

Atlas::Atlas(int width, int height, int border) : Object()
{
	static int id = 1;
	m_Bitmap.width = width;
	m_Bitmap.height = height;
	m_Width = width;
	m_Height = height;
	m_Border = border;
	m_Bitmap.buffer = (uint32*)malloc(sizeof(uint32) * width * height);
	memset(m_Bitmap.buffer, 0, sizeof(uint32) * width * height);
	m_EndingPos.x = 0;
	m_EndingPos.y = 0;
}

bool Atlas::Push(int width, int height, int max_height, uint8* grey_buffer, RGB rgb, Rect2D* out_rect_in_atlas)
{
	if (!BeginPush(width, height, max_height) || !grey_buffer)
		return false;
	if (out_rect_in_atlas)
	{
		out_rect_in_atlas->x = m_EndingPos.x;
		out_rect_in_atlas->y = m_EndingPos.y;
		out_rect_in_atlas->width = width;
		out_rect_in_atlas->height = height;
	}
	Set(m_EndingPos.x, m_EndingPos.y, width, height, grey_buffer, rgb);
	EndPush(width, height, max_height);
	return true;
}

void Atlas::Set(int target_x, int target_y, int width, int height, uint8* grey_buffer, RGB rgb)
{
	for (int y = target_y, index_y = 0; y < target_y + height; y++, index_y++)
	{
		for (int x = target_x, index_x = 0; x < target_x + width; x++, index_x++)
		{
			if (x >= m_Width || x < 0 || y >= m_Height || y < 0)
				continue;
			uint8* p = grey_buffer + index_x + index_y * width;
			m_Bitmap.buffer[x + y * m_Width] = _RGBA32(rgb.r, rgb.g, rgb.b, (*p));
		}
	}
}

bool Atlas::BeginPush(int width, int height, int max_height)
{
	int y = m_EndingPos.y;
	if (m_EndingPos.x + width >= m_Width)
	{
		y = m_EndingPos.y + max_height;
		m_EndingPos.x = 0;
	}

	if (y >= m_Height)
	{
		return false;
	}
	m_EndingPos.y = y;
	return true;
}

bool Atlas::TryPush(int width, int height, int max_height)
{
	int y = m_EndingPos.y + height;
	if (m_EndingPos.x + width >= m_Width)
	{
		y = m_EndingPos.y + max_height * 2;
	}

	if (y >= m_Height)
	{
		return false;
	}
	return true;
}

void Atlas::EndPush(int width, int height, int max_height)
{
	m_EndingPos.x += width;
	if (m_EndingPos.x >= m_Width)
	{
		m_EndingPos.x = 0;
		m_EndingPos.y += max_height;
	}
}

bool Atlas::Get(Rect2D* rect_in_atlas, uint32* out_pixels, Color color)
{
	if (!rect_in_atlas || !out_pixels)
		return false;

	if (rect_in_atlas->x < 0 || rect_in_atlas->x + rect_in_atlas->width >= m_Width || rect_in_atlas->y < 0 || rect_in_atlas->y + rect_in_atlas->height >= m_Height)
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
			Color32 c = m_Bitmap.buffer[x + y * m_Width];
			out_pixels[index_x + index_y * width] = (c * color).ToInt32();
		}
	}

	return true;
}

void Atlas::Clear()
{
	m_EndingPos.x = 0;
	m_EndingPos.y = 0;
	if (m_Bitmap.buffer)
		memset(m_Bitmap.buffer, 0, sizeof(uint32) * m_Width * m_Height);
}

void Atlas::Release()
{
	if (m_Bitmap.buffer)
		free(m_Bitmap.buffer);
}