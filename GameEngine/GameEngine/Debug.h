#ifndef _CDEBUG_
#define _CDEBUG_

#define DEBUG_BUFFER_SIZE 256
#include<Windows.h>

class CDebug
{
private:
	static wchar_t w_buffer[DEBUG_BUFFER_SIZE];
	static char c_buffer[DEBUG_BUFFER_SIZE];
	static HWND hwnd;

public:
	static void Init(HWND hwnd);

	static void Log(const char* text, ...);

	static void Log(const wchar_t* text, ...);

	static void Box(const char* text, ...);

	static void Box(const wchar_t* text, ...);
};

#endif