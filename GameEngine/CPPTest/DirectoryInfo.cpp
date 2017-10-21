#include "DirectoryInfo.h"

CDirectoryInfo::CDirectoryInfo(const wstring& path) : CFileSystemInfo(path)
{
	m_exists = m_exists && (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

vector<CFileSystemInfo_PTR> CDirectoryInfo::GetAllFileSystems()
{
	vector<CFileSystemInfo_PTR> list;
	GetAllFileSystems<vector<CFileSystemInfo_PTR>, CFileSystemInfo>(list, this, 0xFFFFFFFF);
	return list;
}

vector<CDirectoryInfo_PTR> CDirectoryInfo::GetDirectorys()
{
	vector<CDirectoryInfo_PTR> list;
	GetAllFileSystems<vector<CDirectoryInfo_PTR>, CDirectoryInfo>(list, this, FILE_ATTRIBUTE_DIRECTORY);
	return list;
}

vector<CFileInfo_PTR> CDirectoryInfo::GetFiles()
{
	vector<CFileInfo_PTR> list;
	GetAllFileSystems<vector<CFileInfo_PTR>, CFileInfo>(list, this, ~FILE_ATTRIBUTE_DIRECTORY);
	return list;
}