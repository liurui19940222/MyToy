#ifndef _CSOURCE_FILE_
#define _CSOURCE_FILE_

#include"ApiDefine.h"

class ENGINE_API ISourceFile
{
public:
	virtual void LoadFromFile(const char* filename) = 0;

	virtual void ReleaseSource() = 0;
};

#endif