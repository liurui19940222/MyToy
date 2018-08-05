#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include"SpRendering\SkeletonAnimation.h"

BEGIN_NAMESPACE_ENGINE

class ModelLoader
{
public:
	ModelLoader() {  }

	virtual PModel LoadFromFile(const char* filename) = 0;
	virtual void ReleaseSource() = 0;
};

END_NAMESPACE_ENGINE

#endif