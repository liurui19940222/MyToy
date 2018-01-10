#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define _Resources CResources::GetInstance()

#include<Windows.h>
#include<map>
#include<string>
#include"..\SpAssetLoader\IAssetLoader.h"
#include"DynamicFactory.h"
#include"..\SpCommon\Singleton.h"
#include"..\SpRendering\SkeletonAnimation.h"

using namespace std;

class ENGINE_API CResources : public CSingleton<CResources>
{
private:
	map<string, IAssetLoader*> m_sources;

public:
	template<typename T>
	T* Load(string path)
	{
		auto it = m_sources.find(path);
		IAssetLoader* sourceFile = NULL;
		if (it == m_sources.end())
		{
			sourceFile = new T();
			sourceFile->LoadFromFile(path.c_str());
			m_sources.insert(make_pair(path, sourceFile));
		}
		else
		{
			sourceFile = it->second;
		}
		return dynamic_cast<T*>(sourceFile);
	}

	AnimationClip* LoadAnimation(string path);

	void Unload(string& path);
};

#endif