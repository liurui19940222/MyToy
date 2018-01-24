#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include"SpRendering\SkeletonAnimation.h"
#include"IAssetLoader.h"

BEGIN_NAMESPACE_ENGINE

class ModelLoader : public IAssetLoader
{
public:
	ModelLoader() { m_model = make_shared<Model>(); }
	PModel m_model = NULL;
};

END_NAMESPACE_ENGINE

#endif