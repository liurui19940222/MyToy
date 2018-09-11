#pragma once

#include<string>

using namespace std;

namespace dxgame {
	string format_str(const char* text, ...);
	wstring format_wstr(const wchar_t* text, ...);
}