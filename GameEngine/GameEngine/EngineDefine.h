#ifndef _CENGINE_DEFINE_
#define _CENGINE_DEFINE_

#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>


#define _RGBA32(r, g, b, a) (a << 24) + (b << 16) + (g << 8) + r
#define Engine CApplication::GetEngine()
#define Application CApplication::GetInstance()
#define MainCamera Engine->GetCamera()
#define CH_MAP_BITMAP_SIZE_W 512
#define CH_MAP_BITMAP_SIZE_H 512

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;

struct Color;
struct Color32
{
	byte a, r, g, b;

	Color32();

	Color32(byte r, byte g, byte b, byte a);

	Color32(Color color);

	Color32(int color);

	int ToInt32();

	bool operator==(const Color32& value) const;

	bool operator!=(const Color32& value) const;
};

struct Color
{
	float a, r, g, b;

	Color();

	Color(float r, float g, float b, float a);

	Color(Color32 color32);

	Color(int color);

	int ToInt32();

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
	LEFT_TOP,
	LEFT_CENTER,
	LEFT_BOTTOM,
	CENTER_TOP,
	CENTER_MIDDLE,
	CENTER_BOTTOM,
	RIGHT_TOP,
	RIGHT_MIDDLE,
	RIGHT_BOTTOM,
};

#endif