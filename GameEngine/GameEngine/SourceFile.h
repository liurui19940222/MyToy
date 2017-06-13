#ifndef _CSOURCE_FILE_
#define _CSOURCE_FILE_

class CSourceFile
{
public:
	CSourceFile();
	~CSourceFile();

	virtual void LoadFromFile(const char* filename) = 0;

	virtual void Release() = 0;
};

#endif