#include "FileInfo.h"

CFileInfo::CFileInfo(const wstring& path) : CFileSystemInfo(path)
{
	m_exists = m_exists && !(m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	if (m_exists)
	{
		int pos = (int)m_name.find_last_of(L".");
		m_extension = m_name.substr(pos + 1, m_name.size() - pos);
		m_name = m_name.substr(0, pos);
	}
}

