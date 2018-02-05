#ifndef _ATLAS_H_
#define _ATLAS_H_

#include<Windows.h>
#include"EngineDefine.h"
#include"ApiDefine.h"
#include"Object.hpp"

BEGIN_NAMESPACE_ENGINE

class CAtlas : public Object
{
private:
	SBitmapData bitmap;
	Point2D endingPos;
	int m_width;
	int m_height;
	int instanceId;

public:
	CAtlas(int width, int height);

	bool Push(int width, int heightint, int max_height, uint8* grey_buffer, RGB rgb, Rect2D* out_rect_in_atlas = NULL);

	void Set(int target_x, int target_y, int width, int height, uint8* grey_buffer, RGB rgb);

	bool BeginPush(int width, int height, int max_height);

	void EndPush(int width, int height, int max_height);

	bool TryPush(int width, int height, int max_height);

	bool Get(Rect2D* rect_in_atlas, uint32* out_pixels, Color color);

	inline int GetWidth() const { return m_width; }

	inline int GetHeight() const { return m_height; }

	inline uint32* GetPixels() const { return bitmap.buffer; }

	void Clear();

	void Release();
};

END_NAMESPACE_ENGINE

#endif
