#ifndef _CENGINE_DEFINE_
#define _CENGINE_DEFINE_

#include<Windows.h>
#include<gl\glew.h>
#include"Math.h"
#include"ApiDefine.h"

#define _RGBA32(r, g, b, a) (a << 24) + (b << 16) + (g << 8) + r
#define CH_MAP_BITMAP_SIZE_W 512
#define CH_MAP_BITMAP_SIZE_H 512

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef TmpVector2<float> Vector2;

struct ENGINE_API Color;
struct ENGINE_API Color32
{
	byte r, g, b, a;

	Color32();

	Color32(byte r, byte g, byte b, byte a);

	Color32(const Color& color);

	Color32(uint color);

	int ToInt32();

	bool operator==(const Color32& value) const;

	bool operator!=(const Color32& value) const;

	Color32 operator*(const Color& color) const;

	static Color32 Hex(int hexColor);
};

struct ENGINE_API Color
{
	float r, g, b, a;

	Color();

	Color(float r, float g, float b, float a);

	Color(const Color32& color32);

	Color(int color);

	int ToInt32() const;

	void Shrink();

	Color operator+(float value) const;

	Color operator-(float value) const;

	Color operator*(float value) const;

	Color operator/(float value) const;

	Color operator+(const Color& value) const;

	Color operator-(const Color& value) const;

	Color operator*(const Color& value) const;

	Color operator/(const Color& value) const;

	bool operator==(const Color& value) const;

	bool operator!=(const Color& value) const;

	static Color Hex(int hex);

	static const Color red;
	static const Color blue;
	static const Color green;
	static const Color grey;
	static const Color black;
	static const Color cyan;
	static const Color purple;
	static const Color white;
	static const Color orange;
};

struct ENGINE_API SRect2D
{
	float center_x;
	float center_y;
	float half_size_x;
	float half_size_y;

	SRect2D();

	SRect2D(float center_x, float center_y, float half_size_x, float half_size_y);

	bool Overlay(const Vector2& pos) const;

	bool operator==(const SRect2D& rect);
	bool operator!=(const SRect2D& rect);
};

LPWSTR AnsiToUnicode(LPCSTR lpcstr);

struct ENGINE_API RGB
{
	uint8 r, g, b;
};

struct ENGINE_API Point2D
{
	int x, y;
};

struct ENGINE_API SBitmapData
{
	uint32* buffer;
	int width;
	int height;
};

struct ENGINE_API Rect2D
{
	int x, y, width, height;
};

enum ENGINE_API EAlignment
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

enum ENGINE_API EAlignmentHorizontal
{
	LEFT = 1,
	CENTER = 2,
	RIGHT = 3,
};

enum ENGINE_API EAlignmentVertical
{
	TOP = 1,
	MIDDLE = 2,
	BOTTOM = 3,
};

struct ENGINE_API Layer
{
	static constexpr int Default = 1;
	static constexpr int UI = 10;
	static constexpr int Overlay2D = 30;
};

#endif