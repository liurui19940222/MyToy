#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include<string>
#include<Windows.h>
#include"defs.h"

using namespace std;

#define WSTRING_BUFFER_SIZE 4096

BEGIN_NAMESPACE_ENGINE

class CConverter
{
private:
	static inline void StringToValue(const string& str, unsigned short* to) { *to = (unsigned short)stoi(str); };
	static inline void StringToValue(const string& str, int* to) { *to = stoi(str); }
	static inline void StringToValue(const string& str, float* to) { *to = stof(str); }
	static inline void StringToValue(const string& str, double* to) { *to = stod(str); }
	static inline void StringToValue(const string& str, long* to) { *to = stol(str); }
	static inline void StringToValue(const string& str, long long* to) { *to = stoll(str); }
	static inline void StringToValue(const string& str, long double* to) { *to = stold(str); }
	static inline void StringToValue(const string& str, unsigned long* to) { *to = stoul(str); }
	static inline void StringToValue(const string& str, unsigned long long* to) { *to = stoull(str); }
	static inline void StringToValue(const string& str, bool* to) { *to = str == "true" || str == "1"; }
	static inline void WStringToValue(const wstring& str, int* to) { *to = stoi(str); }
	static inline void WStringToValue(const wstring& str, float* to) { *to = stof(str); }
	static inline void WStringToValue(const wstring& str, double* to) { *to = stod(str); }
	static inline void WStringToValue(const wstring& str, long* to) { *to = stol(str); }
	static inline void WStringToValue(const wstring& str, long long* to) { *to = stoll(str); }
	static inline void WStringToValue(const wstring& str, long double* to) { *to = stold(str); }
	static inline void WStringToValue(const wstring& str, unsigned long* to) { *to = stoul(str); }
	static inline void WStringToValue(const wstring& str, unsigned long long* to) { *to = stoull(str); }
	static inline void WStringToValue(const wstring& str, bool* to) { *to = str == L"true" || str == L"1"; }
public:
	template<typename T> static inline string ToString(T value) { return to_string(value); }
	template<typename T> static inline wstring ToWString(T value) { return to_wstring(value); }
	template<typename T> static inline T ToValue(const string& str) { T t; StringToValue(str, &t); return t; }
	template<typename T> static inline T ToValue(const wstring& str) { T t; WStringToValue(str, &t); return t; }

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

	static inline wstring StringToWString(const string &str)
	{
		size_t i;
		string curLocale = setlocale(LC_ALL, NULL);
		setlocale(LC_ALL, "chs");
		const char* _source = str.c_str();
		size_t _dsize = str.size() + 1;
		wchar_t* _dest = new wchar_t[_dsize];
		wmemset(_dest, 0x0, _dsize);
		mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
		wstring result = _dest;
		delete[] _dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	static inline string WStringToString(const wstring &wstr)
	{
		size_t i;
		string curLocale = setlocale(LC_ALL, NULL);
		setlocale(LC_ALL, "chs");
		const wchar_t* _source = wstr.c_str();
		size_t _dsize = 2 * wstr.size() + 1;
		char* _dest = new char[_dsize];
		memset(_dest, 0x0, _dsize);
		wcstombs_s(&i, _dest, _dsize, _source, _dsize);
		string result = _dest;
		delete[] _dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}
};

END_NAMESPACE_ENGINE

#endif

