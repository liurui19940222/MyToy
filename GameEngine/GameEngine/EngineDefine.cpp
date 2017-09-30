#include "EngineDefine.h"
#include "Math.h"

SRect2D::SRect2D() : center_x(0), center_y(0), half_size_x(0), half_size_y(0) {}

SRect2D::SRect2D(float center_x, float center_y, float half_size_x, float half_size_y) : 
	center_x(center_x), center_y(center_y), half_size_x(half_size_x), half_size_y(half_size_y) {}

bool SRect2D::Overlay(const Vector2& pos) const
{
	if (pos.x < center_x - half_size_x) return false;
	else if (pos.x > center_x + half_size_x) return false;
	else if (pos.y > center_y + half_size_y) return false;
	else if (pos.y < center_y - half_size_y) return false;
	return true;
}

vector<SRect2D> SRect2D::Split(vector<float> weights)
{
	float maxWidth = half_size_x * 2;
	float leftPos = center_x - half_size_x;
	float halfWidth = 0;
	float w = 0;
	vector<SRect2D> list;
	for (int i = 0; i < weights.size(); ++i)
	{
		w += weights[i] * maxWidth;
		halfWidth = weights[i] * maxWidth * 0.5f;
		list.push_back(SRect2D{ leftPos + w - halfWidth, center_y,  halfWidth , half_size_y });
	}
	return list;
}

bool SRect2D::operator==(const SRect2D& rect)
{
	return this->half_size_x == rect.half_size_x && this->half_size_y == rect.half_size_y 
		&& this->center_x == rect.center_x && this->center_y == rect.center_y;
}

bool SRect2D::operator!=(const SRect2D& rect)
{
	return this->half_size_x != rect.half_size_x || this->half_size_y != rect.half_size_y
		|| this->center_x != rect.center_x || this->center_y != rect.center_y;
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

Color::Color(const Color32& color32)
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

int Color::ToInt32() const
{
	return _RGBA32((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
}

Color Color::operator+(float value) const
{
	return Color(r + value, g + value, b + value, a + value);
}

Color Color::operator-(float value) const
{
	return Color(r - value, g - value, b - value, a - value);
}

Color Color::operator*(float value) const
{
	return Color(r * value, g * value, b * value, a * value);
}

Color Color::operator/(float value) const
{
	return (*this) * (1 / value);
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
	return Color(r / value.r, g / value.g, b / value.b, a / value.a);
}

bool Color::operator==(const Color& value) const
{
	return r == value.r && g == value.g && b == value.b && a == value.a;
}

bool Color::operator!=(const Color& value) const
{
	return r != value.r || g != value.g || b != value.b || a != value.a;
}

Color Color::Hex(int hex)
{
	return Color32::Hex(hex);
}

const Color Color::red(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::blue(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::green(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::grey(0.4f, 0.4f, 0.4f, 1.0f);
const Color Color::black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::cyan(0.0f, 1.0f, 1.0f, 1.0f);
const Color Color::purple(1.0f, 0.0f, 1.0f, 1.0f);
const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::orange(1.0f, 0.5f, 0.0f, 1.0f);

Color32::Color32(){}

Color32::Color32(byte r, byte g, byte b, byte a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color32::Color32(const Color& color)
{
	this->a = color.a * 255 + 0.5f;
	this->r = color.r * 255 + 0.5f;
	this->g = color.g * 255 + 0.5f;
	this->b = color.b * 255 + 0.5f;
}

Color32::Color32(uint color)
{
	this->r = color & 0xFF;
	this->g = (color >> 8) & 0xFF;
	this->b = (color >> 16) & 0xFF;
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

Color32 Color32::Hex(int hexColor)
{
	return Color32(hexColor >> 24, hexColor >> 16 & 0xFF, hexColor >> 8 & 0xFF, hexColor & 0xFF);
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

const int Layer::Default = 1;
const int Layer::UI = 10;
const int Layer::Overlay2D = 30;