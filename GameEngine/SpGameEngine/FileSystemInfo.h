#ifndef _FILE_SYSTEM_INFO_H_
#define _FILE_SYSTEM_INFO_H_

#include<windows.h>
#include<stdio.h>
#include<string>
#include<memory>
#include"..\SpCommon\ApiDefine.h"

using namespace std;

enum class EFileAttributes
{
	ReadOnly = 1,
	Hidden = 2,
	System = 4,
	Directory = 16,
	Archive = 32,
	Device = 64,
	Normal = 128,
	Temporary = 256,
	SparseFile = 512,
	ReparsePoint = 1024,
	Compressed = 2048,
	Offline = 4096,
	NotContentIndexed = 8192,
	Encrypted = 16384
};

class ENGINE_API CFileSystemInfo;
typedef shared_ptr<CFileSystemInfo> CFileSystemInfo_PTR;

class ENGINE_API CFileSystemInfo
{
protected:
	HANDLE m_handle;
	WIN32_FIND_DATA m_findData;
	wstring m_fullpath;
	wstring m_fullname;
	wstring m_name;
	wstring m_prevpath;
	bool m_exists;

public:

	CFileSystemInfo(const wstring& path);

	inline wstring GetFullPath() { return m_fullpath; }
	inline wstring GetFullName() { return m_fullname; }
	inline wstring GetPrevPath() { return m_prevpath; }
	inline bool Exists() { return m_exists; }
	inline bool HasAttributes(int attrributes) { return m_findData.dwFileAttributes & attrributes; }
	inline virtual wstring ToString() { return m_fullpath; }

	static bool IsDirectory(const wstring& path);
};

#endif