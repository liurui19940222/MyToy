#ifndef _CSOURCE_FILE_
#define _CSOURCE_FILE_

class ISourceFile
{
public:
	virtual void LoadFromFile(const char* filename) = 0;

	virtual void ReleaseSource() = 0;
};

#endif