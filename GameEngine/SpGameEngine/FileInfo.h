#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include"FileSystemInfo.h"

class ENGINE_API CFileInfo;
typedef shared_ptr<CFileInfo> CFileInfo_PTR;

class ENGINE_API CFileInfo : public CFileSystemInfo
{
private:
	wstring m_extension;
public:
	CFileInfo(const wstring& path);

	inline wstring GetName() { return m_name; }
	inline wstring GetExtension() { return m_extension; }
};

#endif