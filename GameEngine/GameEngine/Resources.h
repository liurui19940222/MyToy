#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define _Resources CResources::GetInstance()

#include<Windows.h>
#include<map>
#include<string>
#include"SourceFile.h"
#include"DynamicFactory.h"
#include"Singleton.h"

using namespace std;

class CResources : public CSingleton<CResources>
{
private:
	map<string, ISourceFile*> m_sources;

public:
	template<typename T>
	T* Load(string path)
	{
		auto it = m_sources.find(path);
		ISourceFile* sourceFile = NULL;
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

	void Unload(string& path);
};

#endif