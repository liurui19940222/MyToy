#include "Resources.h"

CResources* CResources::m_instance = NULL;

CResources* CResources::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new CResources();
	return m_instance;
}

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