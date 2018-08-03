#ifndef _CENGINE_DEFINE_
#define _CENGINE_DEFINE_

#include<Windows.h>
#include"..\ExternInclude\GL\glew.h"
#include<vector>
#include<iostream>
#include"Math.h"
#include"ApiDefine.h"

using namespace std;

#define _RGBA32(r, g, b, a) (a << 24) + (b << 16) + (g << 8) + r
#define _ToCppBool(_boolean) _boolean != 0
#define IS_TYPE(T, p) dynamic_cast<T*>(p) != NULL
#define SECURITY(p) if(p) p
#define fatalError(error) { std::cout << error << std::endl; assert(0); }

typedef unsigned char uint8, byte;
typedef unsigned short uint16, ushort;
typedef unsigned int uint32, uint;

BEGIN_NAMESPACE_ENGINE

typedef TmpVector2<float> Vector2;
typedef TmpVector4<byte> BVector4;

struct Color;
struct Color32
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

struct Color
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
	static const Color yellow;
};

struct SRect2D
{
	Vector2 center;
	Vector2 halfSize;

	SRect2D();
	SRect2D(Vector2 center, Vector2 halfSize);
	SRect2D(float center_x, float center_y, float half_size_x, float half_size_y);

	bool Overlay(const Vector2& pos) const;
	vector<SRect2D> Split(vector<float> weights);
	void Slice(BVector4 border, vector<SRect2D>& out);

	bool operator==(const SRect2D& rect);
	bool operator!=(const SRect2D& rect);
	void operator*=(float scale);
	SRect2D operator*(float scale);
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

struct FRect
{
	float left, top, right, bottom;
};

enum EAlignment
{
	LEFT_TOP = 1 << 16 | 1,
	LEFT_MIDDLE = 1 << 16 | 2,
	LEFT_BOTTOM = 1 << 16 | 3,
	CENTER_TOP = 2 << 16 | 1,
	CENTER_MIDDLE = 2 << 16 | 2,
	CENTER_BOTTOM = 2 << 16 | 3,
	RIGHT_TOP = 3 << 16 | 1,
	RIGHT_MIDDLE = 3 << 16 | 2,
	RIGHT_BOTTOM = 3 << 16 | 3,
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

struct Layer
{
	static const int Default;
	static const int UI;
	static const int Overlay2D;
};

class EngineToolkit
{
public:
	static RECT GetGlobalRect(HWND& hwnd, int width, int height);

	inline static void SetCursor(HWND& hwnd, LPWSTR cursorId)
	{
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)::LoadCursor(NULL, cursorId));
	}

	static void WatchTarget(Vector3& camreaPos, Matrix4x4& viewMatrix, const Vector3& targetPos, float deltaTime, HWND hwnd = NULL);
};

END_NAMESPACE_ENGINE

#endif