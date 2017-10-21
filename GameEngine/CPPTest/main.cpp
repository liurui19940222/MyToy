#include <windows.h>
#include <stdio.h>
#include <string.h>
#define LEN 1024
int FileCount = 0;
// 深度优先递归遍历目录中所有的文件
BOOL  DirectoryList(LPCWSTR Path)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;

	WCHAR FilePathName[LEN];
	// 构造路径
	WCHAR FullPathName[LEN];
	wcscpy(FilePathName, Path);
	wcscat(FilePathName, L"\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("搜索失败!");
		return 0;
	}
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (wcscmp(FindData.cFileName, L".") == 0
			|| wcscmp(FindData.cFileName, L"..") == 0)
		{
			continue;
		}
		// 构造完整路径
		wsprintf(FullPathName, L"%s\\%s", Path, FindData.cFileName);
		FileCount++;
		// 输出本级的文件
		OutputDebugStringW(FullPathName);
		OutputDebugString(L"\n");
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DirectoryList(FullPathName);
		}
	}
	return 0;
}

#include"FileSystemInfo.h"
#include"DirectoryInfo.h"
#include"FileInfo.h"

void PrintAll(const wstring& path)
{
	if (CFileSystemInfo::IsDirectory(path))
	{
		CDirectoryInfo info(path);
		wstring dirname = info.GetFullPath();
		dirname = L"dir:" + dirname + L"\n";
		OutputDebugString(dirname.c_str());
		for (CDirectoryInfo_PTR dir : info.GetDirectorys())
		{
			PrintAll(dir.get()->GetFullPath());
		}
		for (CFileInfo_PTR file : info.GetFiles())
		{
			PrintAll(file.get()->GetFullPath());
		}
	}
	else
	{
		CFileInfo info(path);
		wstring filename = info.GetFullPath();
		filename = L"file:" + filename + L"\n";
		OutputDebugString(filename.c_str());
	}
}

void PrintThis(const wstring& path)
{
	if (CFileSystemInfo::IsDirectory(path))
	{
		CDirectoryInfo info(path);
		wstring dirname = info.GetFullPath();
		dirname = L"dir:" + dirname + L"\n";
		OutputDebugString(dirname.c_str());
		for (CFileSystemInfo_PTR dir : info.GetAllFileSystems())
		{
			wstring s = dir.get()->GetFullPath();
			s = L"s:" + s + L"\n";
			OutputDebugString(s.c_str());
		}
	}
	else
	{
		CFileInfo info(path);
		wstring filename = info.GetFullPath();
		filename = L"file:" + filename + L"\n";
		OutputDebugString(filename.c_str());
	}
}

void main()
{
	PrintAll(L"D:\\by_apk");
	//PrintThis(L"D:\\by_apk");
	system("pause");
}