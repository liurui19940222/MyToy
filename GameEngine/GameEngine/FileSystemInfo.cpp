#include "FileSystemInfo.h"

CFileSystemInfo::CFileSystemInfo(const wstring& path)
{
	m_handle = FindFirstFile(path.c_str(), &m_findData);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		m_exists = false;
		return;
	}
	m_exists = true;
	m_fullpath = path;
	m_name = m_findData.cFileName;
	m_fullname = m_findData.cFileName;
	int pos = (int)m_fullpath.find(m_fullname);
	m_prevpath = m_fullpath.substr(0, pos);
}

bool CFileSystemInfo::IsDirectory(const wstring& path)
{
	CFileSystemInfo info(path);
	return info.HasAttributes(FILE_ATTRIBUTE_DIRECTORY);
}