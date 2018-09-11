#include"string_ext.h"
#include<stdio.h>
#include<Windows.h>

string dxgame::format_str(const char* text, ...)
{
	static char buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	va_list ap;
	va_start(ap, text);
	vsprintf_s(buffer, text, ap);
	va_end(ap);
	return string(buffer);
}

wstring dxgame::format_wstr(const wchar_t* text, ...)
{
	static wchar_t buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	va_list ap;
	va_start(ap, text);
	vswprintf_s(buffer, text, ap);
	va_end(ap);
	return wstring(buffer);
}