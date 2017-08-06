#include "Resources.h"

void CResources::Unload(string& path)
{
	auto it = m_sources.find(path);
	if (it != m_sources.end())
	{
		ISourceFile* sourceFile = it->second;
		sourceFile->ReleaseSource();
		m_sources.erase(it);
		delete(sourceFile);
	}
}