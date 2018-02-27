#include"Resources.h"
#include"SpAssetLoader\ColladaLoader.h"

PAnimationClip CResources::LoadAnimation(string path)
{
	ColladaLoader* modelFile = _Resources->Load<ColladaLoader>(path);
	PModel model = modelFile->m_model;
	PAnimationClip clip;
	if (model->animationsCount() > 0)
		clip = model->m_Animations[0];
	delete(modelFile);
	return clip;
}

void CResources::Unload(string& path)
{
	auto it = m_sources.find(path);
	if (it != m_sources.end())
	{
		IAssetLoader* sourceFile = it->second;
		sourceFile->ReleaseSource();
		m_sources.erase(it);
		delete(sourceFile);
	}
}