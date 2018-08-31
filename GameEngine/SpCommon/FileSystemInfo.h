#ifndef _FILE_SYSTEM_INFO_H_
#define _FILE_SYSTEM_INFO_H_

#include<windows.h>
#include<stdio.h>
#include<string>
#include<memory>
#include"ApiDefine.h"

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

class FileSystemInfo;
typedef shared_ptr<FileSystemInfo> FileSystemInfo_PTR;

class FileSystemInfo
{
protected:
	HANDLE m_handle;
	WIN32_FIND_DATAA m_findData;
	string m_fullpath;
	string m_fullname;
	string m_name;
	string m_prevpath;
	bool m_exists;

public:

	FileSystemInfo(const string& path);

	inline string GetFullPath() { return m_fullpath; }
	inline string GetFullName() { return m_fullname; }
	inline string GetPrevPath() { return m_prevpath; }
	inline bool Exists() { return m_exists; }
	inline bool HasAttributes(int attrributes) { return (m_findData.dwFileAttributes & attrributes) > 0; }
	inline virtual string ToString() { return m_fullpath; }

	static bool IsDirectory(const string& path);

	static bool IsExists(const string& path);
};

#endif