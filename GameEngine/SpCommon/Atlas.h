#ifndef _ATLAS_H_
#define _ATLAS_H_

#include<Windows.h>
#include"EngineDefine.h"
#include"ApiDefine.h"
#include"Object.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(Atlas) class Atlas : public Object
{
private:
	SBitmapData			m_Bitmap;
	Point2D				m_EndingPos;
	int					m_Width;
	int					m_Height;
	int					m_Border;

	void Release();
public:
	Atlas(int width, int height, int border);
	~Atlas();

	bool Push(int width, int heightint, int max_height, uint8* grey_buffer, RGB rgb, Rect2D* out_rect_in_atlas = NULL);

	void Set(int target_x, int target_y, int width, int height, uint8* grey_buffer, RGB rgb);

	bool BeginPush(int width, int height, int max_height);

	void EndPush(int width, int height, int max_height);

	bool TryPush(int width, int height, int max_height);

	bool Get(Rect2D* rect_in_atlas, uint32* out_pixels, Color color);

	inline int GetWidth() const { return m_Width; }

	inline int GetHeight() const { return m_Height; }

	inline uint32* GetPixels() const { return m_Bitmap.buffer; }

	void Clear();
};

END_NAMESPACE_ENGINE

#endif
