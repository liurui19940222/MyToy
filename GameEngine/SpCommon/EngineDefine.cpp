#include "EngineDefine.h"
#include "Math.h"
#include "Input.h"

USING_NAMESPACE_ENGINE

SRect2D::SRect2D() : center(0, 0), halfSize(0, 0) {}

SRect2D::SRect2D(Vector2 center, Vector2 halfSize) : center(center), halfSize(halfSize) {}

SRect2D::SRect2D(float center_x, float center_y, float half_size_x, float half_size_y) : 
	center(center_x, center_y), halfSize(half_size_x, half_size_y) {}

bool SRect2D::Overlay(const Vector2& pos) const
{
	if (pos.x < center.x - halfSize.x) return false;
	else if (pos.x > center.x + halfSize.x) return false;
	else if (pos.y > center.y + halfSize.y) return false;
	else if (pos.y < center.y - halfSize.y) return false;
	return true;
}

vector<SRect2D> SRect2D::Split(vector<float> weights)
{
	float maxWidth = halfSize.x * 2;
	float leftPos = center.x - halfSize.x;
	float halfWidth = 0;
	float w = 0;
	vector<SRect2D> list;
	for (uint i = 0; i < weights.size(); ++i)
	{
		w += weights[i] * maxWidth;
		halfWidth = weights[i] * maxWidth * 0.5f;
		list.push_back(SRect2D{ leftPos + w - halfWidth, center.y,  halfWidth , halfSize.y });
	}
	return list;
}

void SRect2D::Slice(BVector4 border, vector<SRect2D>& out)
{
	out.resize(9);
	float x1, x2, x3, y1, y2, y3, w1, w2, w3, h1, h2, h3;
	float left, top;
	left = center.x - halfSize.x;
	top = center.y + halfSize.y;
	w1 = border.left;
	w2 = (halfSize.x * 2 - border.left - border.right);
	w3 = border.right;
	h1 = border.top;
	h2 = (halfSize.y * 2 - border.top - border.bottom);
	h3 = border.bottom;
	x1 = left + w1 * 0.5f;
	x2 = left + w1 + w2 * 0.5f;
	x3 = left + w1 + w2 + w3 * 0.5f;
	y1 = top - h1 * 0.5f;
	y2 = top - h1 - h2 * 0.5f;
	y3 = top - h1 - h2 - h3 * 0.5f;

	w1 *= 0.5f;
	w2 *= 0.5f;
	w3 *= 0.5f;
	h1 *= 0.5f;
	h2 *= 0.5f;
	h3 *= 0.5f;
	out[0].center.x = x1;
	out[0].center.y = y1;
	out[0].halfSize.x = w1;
	out[0].halfSize.y = h1;

	out[1].center.x = x2;
	out[1].center.y = y1;
	out[1].halfSize.x = w2;
	out[1].halfSize.y = h1;

	out[2].center.x = x3;
	out[2].center.y = y1;
	out[2].halfSize.x = w3;
	out[2].halfSize.y = h1;

	out[3].center.x = x1;
	out[3].center.y = y2;
	out[3].halfSize.x = w1;
	out[3].halfSize.y = h2;

	out[4].center.x = x2;
	out[4].center.y = y2;
	out[4].halfSize.x = w2;
	out[4].halfSize.y = h2;

	out[5].center.x = x3;
	out[5].center.y = y2;
	out[5].halfSize.x = w3;
	out[5].halfSize.y = h2;

	out[6].center.x = x1;
	out[6].center.y = y3;
	out[6].halfSize.x = w1;
	out[6].halfSize.y = h3;

	out[7].center.x = x2;
	out[7].center.y = y3;
	out[7].halfSize.x = w2;
	out[7].halfSize.y = h3;

	out[8].center.x = x3;
	out[8].center.y = y3;
	out[8].halfSize.x = w3;
	out[8].halfSize.y = h3;
}

bool SRect2D::operator==(const SRect2D& rect)
{
	return this->halfSize.x == rect.halfSize.x && this->halfSize.y == rect.halfSize.y 
		&& this->center.x == rect.center.x && this->center.y == rect.center.y;
}

bool SRect2D::operator!=(const SRect2D& rect)
{
	return this->halfSize.x != rect.halfSize.x || this->halfSize.y != rect.halfSize.y
		|| this->center.x != rect.center.x || this->center.y != rect.center.y;
}

void SRect2D::operator*=(float scale)
{
	halfSize.x *= scale;
	halfSize.y *= scale;
}

SRect2D SRect2D::operator*(float scale)
{
	return SRect2D(center.x, center.y, halfSize.x * scale, halfSize.y * scale);
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

Color Color::Lerp(const Color& a, const Color& b, float t)
{
	return Color(CMath::Lerp(a.r, b.r, t), CMath::Lerp(a.g, b.g, t), CMath::Lerp(a.b, b.b, t), CMath::Lerp(a.a, b.a, t));
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
const Color Color::yellow(1.0f, 1.0f, 0.0f, 1.0f);

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
	this->a = (byte)(color.a * 255 + 0.5f);
	this->r = (byte)(color.r * 255 + 0.5f);
	this->g = (byte)(color.g * 255 + 0.5f);
	this->b = (byte)(color.b * 255 + 0.5f);
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
	return Color32((byte)(this->r * color.r), (byte)(this->g * color.g), (byte)(this->b * color.b), (byte)(this->a * color.a));
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

RECT EngineToolkit::GetGlobalRect(HWND& hwnd, int width, int height)
{
	RECT rect;
	POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + width;
	rect.top = p.y;
	rect.bottom = p.y + height;
	return rect;
}

void EngineToolkit::WatchTarget(Vector3& camreaPos, Matrix4x4& viewMatrix, const Vector3& targetPos, float deltaTime, HWND hwnd)
{
	static Vector3 eyePos(0, 6.8, 10);
	static float speed = 100.0f;
	static bool key_down = false;
	static Vector2 lastMousePos(0.0f, 0.0f);
	static Vector2 curMousePos(0.0f, 0.0f);
	static float distance = (eyePos - targetPos).Magnitude();
	if (Input::GetMouseDown(EMouseKey::Right))
	{
		key_down = true;
		lastMousePos = Input::InputMousePosition();
		if (hwnd)
			EngineToolkit::SetCursor(hwnd, IDC_CROSS);
	}
	if (Input::GetMouseUp(EMouseKey::Right))
	{
		key_down = false;
		if (hwnd)
			EngineToolkit::SetCursor(hwnd, IDC_ARROW);
	}
	distance -= Input::GetAxis("Scroll") * deltaTime;
	distance = CMath::Clamp(distance, 8.0f, 20.0f);
	Vector3 dir = (camreaPos - targetPos).Normalize() * distance;
	camreaPos = dir + targetPos;
	if (!key_down) return;
	curMousePos = Input::InputMousePosition();
	float h = (curMousePos.x - lastMousePos.x) * deltaTime * speed;
	float v = (curMousePos.y - lastMousePos.y) * deltaTime * speed;
	lastMousePos = curMousePos;
	dir = Quaternion::AngleAxis(Vector3::up, -h) * Quaternion::AngleAxis(Vector3::Cross(dir, Vector3::up), -v) * dir;
	float angle = Vector3::Angle(dir, Vector3::up);
	if (angle  < 10.0f || angle > 170.0f) return;
	camreaPos = dir + targetPos;

	viewMatrix.MakeLookAt(camreaPos, targetPos, Vector3::up);
}