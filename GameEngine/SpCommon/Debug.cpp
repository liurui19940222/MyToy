#include "Debug.h"
#include<Windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<atltime.h>

USING_NAMESPACE_ENGINE

wchar_t Debug::w_buffer[DEBUG_BUFFER_SIZE];
char Debug::c_buffer[DEBUG_BUFFER_SIZE];
HWND Debug::hwnd = NULL;
wstring Debug::timingtemp;
long Debug::timingBeginningTime = 0;

void Debug::Init(HWND _hwnd)
{
	hwnd = _hwnd;
}

void Debug::Log(const string str, ...)
{
	va_list ap;
	va_start(ap, str);
	memset(c_buffer, 0, sizeof(c_buffer));
	vsprintf_s(c_buffer, str.c_str(), ap);
	OutputDebugStringA(c_buffer);
	OutputDebugStringA("\n");
	va_end(ap);
}

void Debug::Log(const wstring str, ...)
{
	va_list ap;
	va_start(ap, str);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, str.c_str(), ap);
	OutputDebugStringW(w_buffer);
	OutputDebugStringW(L"\n");
	va_end(ap);
}

void Debug::Log(const char* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(c_buffer, 0, sizeof(c_buffer));
	vsprintf_s(c_buffer, text, ap);
	OutputDebugStringA(c_buffer);
	OutputDebugStringA("\n");
	va_end(ap);
}

void Debug::Log(const wchar_t* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, text, ap);
	OutputDebugStringW(w_buffer);
	OutputDebugStringW(L"\n");
	va_end(ap);
}

void Debug::Log(Matrix4x4& mat)
{
	Log("%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n", 
		mat[0][0], mat[1][0], mat[2][0], mat[3][0], 
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
		);
}

void Debug::Log(const Vector3& v)
{
	Log("%g %g %g", v.x, v.y, v.z);
}

void Debug::Log(const Vector4& v)
{
	Log("%g %g %g %g", v.x, v.y, v.z, v.w);
}

void Debug::Log(const Quaternion& q)
{
	Log("%g %g %g %g", q.x, q.y, q.z, q.w);
}

void Debug::Log(const Color& color)
{
	Log("%g %g %g %g", color.r, color.g, color.b, color.a);
}

void Debug::Box(const char* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(c_buffer, 0, sizeof(c_buffer));
	memset(w_buffer, 0, sizeof(w_buffer));
	vsprintf_s(c_buffer, text, ap);
	swprintf_s(w_buffer, L"%hs", c_buffer);
	MessageBoxExW(hwnd, w_buffer, L"", MB_OK, 0);
	va_end(ap);
}

void Debug::Box(const wchar_t* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, text, ap);
	MessageBoxExW(hwnd, w_buffer, L"", MB_OK, 0);
	va_end(ap);
}

void Debug::BeginTiming(const wchar_t* text)
{
	timingtemp.clear();
	timingtemp.append(text);
	timingBeginningTime = GetTickCount();
}

void Debug::EndTiming()
{
	if (timingtemp.empty())
		return;
	Debug::Log(timingtemp.c_str(), GetTickCount() - timingBeginningTime);
	timingtemp.clear();
}