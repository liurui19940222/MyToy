#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include"SpRendering\SkeletonAnimation.h"
#include"IAssetLoader.h"
namespace spgameengine
{
	class ModelLoader : public IAssetLoader
	{
	public:
		ModelLoader() { m_model = make_shared<Model>(); }
		PModel m_model = NULL;
	};
}
#endif