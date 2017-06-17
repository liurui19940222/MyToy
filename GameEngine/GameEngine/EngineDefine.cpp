#include "EngineDefine.h"
#include "Math.h"

CEngineDefine::CEngineDefine()
{
}


CEngineDefine::~CEngineDefine()
{
}

Color::Color(){}

Color::Color(float r, float g, float b, float a)
{
	this->a = a;
	this->r = r;
	this->g = g;
	this->b = b;
	Shrink();
}

Color::Color(Color32 color32)
{
	this->a = color32.a / 255.0f;
	this->r = color32.r / 255.0f;
	this->g = color32.g / 255.0f;
	this->b = color32.b / 255.0f;
}

Color::Color(int color)
{
	Color32 color32(color);
	this->r = color32.r / 255.0f;
	this->g = color32.g / 255.0f;
	this->b = color32.b / 255.0f;
	this->a = color32.a / 255.0f;
}

void Color::Shrink()
{
	r = CMath::Clamp(r, 0.0f, 1.0f);
	g = CMath::Clamp(g, 0.0f, 1.0f);
	b = CMath::Clamp(b, 0.0f, 1.0f);
	a = CMath::Clamp(a, 0.0f, 1.0f);
}

int Color::ToInt32()
{
	return _RGBA32((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
}

Color Color::operator+(const Color& value) const
{
	return Color(r + value.r, g + value.g, b + value.b, a + value.a);
}

Color Color::operator-(const Color& value) const
{
	return Color(r - value.r, g - value.g, b - value.b, a - value.a);
}

Color Color::operator*(const Color& value) const
{
	return Color(r * value.r, g * value.g, b * value.b, a * value.a);
}

Color Color::operator/(const Color& value) const
{
	return (*this) * 1 / value;
}

bool Color::operator==(const Color& value) const
{
	return r == value.r && g == value.g && b == value.b && a == value.a;
}

bool Color::operator!=(const Color& value) const
{
	return r != value.r || g != value.g || b != value.b || a != value.a;
}

Color Color::red(){ return Color(1, 0, 0, 1); }
Color Color::blue(){ return Color(0, 0, 1, 1); }
Color Color::green(){ return Color(0, 1, 0, 1); }
Color Color::black(){ return Color(0, 0, 0, 1); }
Color Color::cyan(){ return Color(0, 1, 1, 1); }
Color Color::purple(){ return Color(1, 0, 1, 1); }
Color Color::white(){ return Color(1, 1, 1, 1); }
Color Color::orange(){ return Color(1, 0.5f, 0, 1); }

Color32::Color32(){}

Color32::Color32(byte r, byte g, byte b, byte a)
{
	this->a = a;
	this->r = r;
	this->g = g;
	this->b = b;
}

Color32::Color32(Color color)
{
	this->a = color.a * 255 + 0.5f;
	this->r = color.r * 255 + 0.5f;
	this->g = color.g * 255 + 0.5f;
	this->b = color.b * 255 + 0.5f;
}

Color32::Color32(uint color)
{
	this->b = color;
	this->g = color >> 8;
	this->r = color >> 16;
	this->a = color >> 24;
}

int Color32::ToInt32()
{
	return _RGBA32(r, g, b, a);
}

bool Color32::operator==(const Color32& value) const
{
	return this->r == value.r && this->g == value.g && this->b == value.b && this->a == value.a;
}

bool Color32::operator!=(const Color32& value) const
{
	return this->r != value.r || this->g != value.g || this->b != value.b || this->a != value.a;
}

Color32 Color32::operator*(const Color& color) const
{
	return Color32(this->r * color.r, this->g * color.g, this->b * color.b, this->a * color.a);
}

LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //参数lpcstr类型也可是char*
{
	LPWSTR Pwstr;
	int  i;
	i = MultiByteToWideChar(CP_ACP, 0, lpcstr, -1, NULL, 0);
	Pwstr = new WCHAR[i];
	MultiByteToWideChar(CP_ACP, 0, lpcstr, -1, Pwstr, i);

	return (Pwstr);
}