#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include<string>
#include<Windows.h>

using namespace std;

class CConverter
{
private:
	static constexpr int WSTRING_BUFFER_SIZE = 4096;

	static inline void StringToValue(const string& str, int* to) { *to = stoi(str); }
	static inline void StringToValue(const string& str, float* to) { *to = stof(str); }
	static inline void StringToValue(const string& str, double* to) { *to = stod(str); }
	static inline void StringToValue(const string& str, long* to) { *to = stol(str); }
	static inline void StringToValue(const string& str, long long* to) { *to = stoll(str); }
	static inline void StringToValue(const string& str, long double* to) { *to = stold(str); }
	static inline void StringToValue(const string& str, unsigned long* to) { *to = stoul(str); }
	static inline void StringToValue(const string& str, unsigned long long* to) { *to = stoull(str); }
	static inline void StringToValue(const string& str, bool* to) { *to = str == "true" || str == "1"; }
public:
	template<typename T> static inline string ToString(T value) { return to_string(value); }
	template<typename T> static inline T ToValue(const string& str) { T t; StringToValue(str, &t); return t; }

	static inline wstring FormatWString(wchar_t* text, ...)
	{
		static wchar_t m_wstringBuffer[WSTRING_BUFFER_SIZE];
		va_list ap;
		va_start(ap, text);
		memset(m_wstringBuffer, 0, sizeof(m_wstringBuffer));
		vswprintf_s(m_wstringBuffer, text, ap);
		va_end(ap);
		return wstring(m_wstringBuffer);
	}
};

#endif

