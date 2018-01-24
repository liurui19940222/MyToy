#ifndef _CSOURCE_FILE_
#define _CSOURCE_FILE_

#include"SpCommon\ApiDefine.h"

BEGIN_NAMESPACE_ENGINE

class IAssetLoader
{
public:
	virtual void LoadFromFile(const char* filename) = 0;

	virtual void ReleaseSource() = 0;
};

END_NAMESPACE_ENGINE

#endif