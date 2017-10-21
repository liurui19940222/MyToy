#ifndef _DIRECTORY_INFO_H_
#define _DIRECTORY_INFO_H_

#include<vector>
#include<memory>
#include"FileSystemInfo.h"
#include"FileInfo.h"

class CDirectoryInfo;
typedef shared_ptr<CDirectoryInfo> CDirectoryInfo_PTR;

class CDirectoryInfo : public CFileSystemInfo
{
	template<typename LIST, typename T> void GetAllFileSystems(LIST& list, CDirectoryInfo* parent, int attrs)
	{
		WIN32_FIND_DATA findData;
		HANDLE handle = FindFirstFile((parent->m_fullpath + L"\\*.*").c_str(), &findData);
		while (::FindNextFile(handle, &findData))
		{
			if (wcscmp(findData.cFileName, L".") == 0
				|| wcscmp(findData.cFileName, L"..") == 0)
			{
				continue;
			}
			if (findData.dwFileAttributes & attrs)
			{
				wstring path = parent->m_fullpath + L"\\" + findData.cFileName;
				list.push_back(make_shared<T>(path));
			}
		}
	}

public:

	CDirectoryInfo(const wstring& path);

	vector<CFileSystemInfo_PTR> GetAllFileSystems();
	vector<CDirectoryInfo_PTR> GetDirectorys();
	vector<CFileInfo_PTR> GetFiles();
};

#endif