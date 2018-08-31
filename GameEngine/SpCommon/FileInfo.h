#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include"FileSystemInfo.h"

class FileInfo;
typedef shared_ptr<FileInfo> FileInfo_PTR;

class FileInfo : public FileSystemInfo
{
private:
	string m_extension;
public:
	FileInfo(const string& path);

	inline string GetName() { return m_name; }
	inline string GetExtension() { return m_extension; }
};

#endif