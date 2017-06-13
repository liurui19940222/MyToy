#include "Debug.h"
#include<Windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<atltime.h>

wchar_t CDebug::w_buffer[DEBUG_BUFFER_SIZE];

char CDebug::c_buffer[DEBUG_BUFFER_SIZE];

HWND CDebug::hwnd = NULL;

void CDebug::Init(HWND _hwnd)
{
	hwnd = _hwnd;
}

void CDebug::Log(const char* text, ...)
{
	va_list ap;
	va_start(ap, text);
	memset(c_buffer, 0, sizeof(c_buffer));
	memset(w_buffer, 0, sizeof(w_buffer));
	vsprintf_s(c_buffer, text, ap);
	swprintf_s(w_buffer, L"%hs", c_buffer);
	OutputDebugStringW(w_buffer);
	OutputDebugStringW(L"\n");
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