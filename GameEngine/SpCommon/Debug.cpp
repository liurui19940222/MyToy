#include "Debug.h"
#include<Windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<atltime.h>

USING_NAMESPACE_ENGINE

wchar_t CDebug::w_buffer[DEBUG_BUFFER_SIZE];

char CDebug::c_buffer[DEBUG_BUFFER_SIZE];

HWND CDebug::hwnd = NULL;

void CDebug::Init(HWND _hwnd)
{
	hwnd = _hwnd;
}

void CDebug::Log(const string str, ...)
{
	va_list ap;
	va_start(ap, str);
	memset(c_buffer, 0, sizeof(c_buffer));
	vsprintf_s(c_buffer, str.c_str(), ap);
	OutputDebugStringA(c_buffer);
	OutputDebugStringA("\n");
	va_end(ap);
}

void CDebug::Log(const wstring str, ...)
{
	va_list ap;
	va_start(ap, str);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, str.c_str(), ap);
	OutputDebugStringW(w_buffer);
	OutputDebugStringW(L"\n");
	va_end(ap);
}

void CDebug::Log(const char* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(c_buffer, 0, sizeof(c_buffer));
	vsprintf_s(c_buffer, text, ap);
	OutputDebugStringA(c_buffer);
	OutputDebugStringA("\n");
	va_end(ap);
}

void CDebug::Log(const wchar_t* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, text, ap);
	OutputDebugStringW(w_buffer);
	OutputDebugStringW(L"\n");
	va_end(ap);
}

void CDebug::Log(Matrix4x4& mat)
{
	Log("%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n%g \t\t%g \t\t%g \t\t%g\n", 
		mat[0][0], mat[1][0], mat[2][0], mat[3][0], 
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
		);
}

void CDebug::Log(const Vector3& v)
{
	Log("%g %g %g", v.x, v.y, v.z);
}

void CDebug::Log(const Vector4& v)
{
	Log("%g %g %g %g", v.x, v.y, v.z, v.w);
}

void CDebug::Log(const Quaternion& q)
{
	Log("%g %g %g %g", q.x, q.y, q.z, q.w);
}

void CDebug::Log(const Color& color)
{
	Log("%g %g %g %g", color.r, color.g, color.b, color.a);
}

void CDebug::Box(const char* text, ...)
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

void CDebug::Box(const wchar_t* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(w_buffer, 0, sizeof(w_buffer));
	vswprintf_s(w_buffer, text, ap);
	MessageBoxExW(hwnd, w_buffer, L"", MB_OK, 0);
	va_end(ap);
}