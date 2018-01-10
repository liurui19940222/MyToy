#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include"..\SpRendering\SkeletonAnimation.h"
#include"IAssetLoader.h"

class ModelLoader : public IAssetLoader
{
public:
	Model* m_model = NULL;
};

#endif