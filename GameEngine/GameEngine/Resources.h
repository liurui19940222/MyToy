#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include<Windows.h>
#include<map>
#include<string>
#include"SourceFile.h"
#include"DynamicFactory.h"

using namespace std;

class CResources
{
private:
	static CResources* m_instance;

	map<string, ISourceFile*> m_sources;

public:
	static CResources* GetInstance();

	template<typename T>
	T* Load(string path)
	{
		auto it = m_sources.find(path);
		ISourceFile* sourceFile = NULL;
		if (it == m_sources.end())
		{
			sourceFile = (ISourceFile*)DynamicFactory::Instance().Create<T>();
			sourceFile->LoadFromFile(path.c_str());
			m_sources.insert(make_pair(path, sourceFile));
		}
		else
		{
			sourceFile = it->second;
		}
		return dynamic_cast<T*>(sourceFile);
	}

	void Unload(string& path);
};

#endif