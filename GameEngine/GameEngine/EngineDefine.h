#ifndef _CENGINE_DEFINE_
#define _CENGINE_DEFINE_

#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#define _RGBA32(r, g, b, a) (a << 24) + (b << 16) + (g << 8) + r
#define Engine CApplication::GetEngine()
#define Application CApplication::GetInstance()
#define MainCamera Engine->GetCamera()
#define FontManager CFontManager::GetInstance()
#define GUISystem guisystem::CGUISystem::GetInstance()
#define CH_MAP_BITMAP_SIZE_W 512
#define CH_MAP_BITMAP_SIZE_H 512

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;

class Vector2;
struct Color;
struct Color32
{
	byte r, g, b, a;

	Color32();

	Color32(byte r, byte g, byte b, byte a);

	Color32(Color color);

	Color32(uint color);

	int ToInt32();

	bool operator==(const Color32& value) const;

	bool operator!=(const Color32& value) const;

	Color32 operator*(const Color& color) const;
};

struct Color
{
	float r, g, b, a;

	Color();

	Color(float r, float g, float b, float a);

	Color(Color32 color32);

	Color(int color);

	int ToInt32();

	void Shrink();

	Color operator+(const Color& value) const;

	Color operator-(const Color& value) const;

	Color operator*(const Color& value) const;

	Color operator/(const Color& value) const;

	bool operator==(const Color& value) const;

	bool operator!=(const Color& value) const;

	static Color red();
	static Color blue();
	static Color green();
	static Color black();
	static Color cyan();
	static Color purple();
	static Color white();
	static Color orange();
};

struct SRect2D
{
	float center_x;
	float center_y;
	float half_size_x;
	float half_size_y;

	SRect2D();

	SRect2D(float center_x, float center_y, float half_size_x, float half_size_y);

	bool Overlay(Vector2 pos);
};

class CEngineDefine
{
public:
	CEngineDefine();
	~CEngineDefine();

};

LPWSTR AnsiToUnicode(LPCSTR lpcstr);

struct RGB
{
	uint8 r, g, b;
};

struct Point2D
{
	int x, y;
};

struct SBitmapData
{
	uint32* buffer;
	int width;
	int height;
};

struct Rect2D
{
	int x, y, width, height;
};

enum EAlignment
{
	LEFT_TOP		= 1 << 16 | 1,
	LEFT_MIDDLE		= 1 << 16 | 2,
	LEFT_BOTTOM		= 1 << 16 | 3,
	CENTER_TOP		= 2 << 16 | 1,
	CENTER_MIDDLE	= 2 << 16 | 2,
	CENTER_BOTTOM	= 2 << 16 | 3,
	RIGHT_TOP		= 3 << 16 | 1,
	RIGHT_MIDDLE	= 3 << 16 | 2,
	RIGHT_BOTTOM	= 3 << 16 | 3,
};

#define _GetHorizontal(alignment) (EAlignmentHorizontal)(alignment >> 16)
#define _GetVertical(alignment) (EAlignmentVertical)(alignment & 0xffff)
#define _GetAlignment(h, v) (EAlignment)(h << 16 | v)

enum EAlignmentHorizontal
{
	LEFT = 1,
	CENTER = 2,
	RIGHT = 3,
};

enum EAlignmentVertical
{
	TOP = 1,
	MIDDLE = 2,
	BOTTOM = 3,
};

#endif