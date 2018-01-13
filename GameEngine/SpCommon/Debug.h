#ifndef _CDEBUG_
#define _CDEBUG_

#define DEBUG_BUFFER_SIZE 1024
#include<Windows.h>
#include<string>
#include"Math.h"
#include"..\SpCommon\EngineDefine.h"

using namespace std;

class CDebug
{
private:
	static wchar_t w_buffer[DEBUG_BUFFER_SIZE];
	static char c_buffer[DEBUG_BUFFER_SIZE];
	static HWND hwnd;

public:
	static void Init(HWND hwnd);
	static void Log(const string str, ...);
	static void Log(const char* text, ...);
	static void Log(const wchar_t* text, ...);
	static void Log(Matrix4x4& mat);
	static void Log(const Vector3& v);
	static void Log(const Vector4& v);
	static void Log(const Quaternion& q);
	static void Log(const Color& color);
	static void Box(const char* text, ...);
	static void Box(const wchar_t* text, ...);
};

#endif