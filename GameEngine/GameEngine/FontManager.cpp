#include "FontManager.h"

CFontManager * CFontManager::instance = nullptr;

CFontManager* CFontManager::GetInstance()
{
	if (!instance)
		instance = new CFontManager;
	return instance;
}

CTrueTypeFont* CFontManager::LoadFont(int fontId, const char* file_name)
{
	if (fontMap.find(fontId) == fontMap.end())
	{
		CTrueTypeFont* font = new CTrueTypeFont(fontId, file_name);
		fontMap.insert(make_pair(fontId, font));
	}
	return GetFont(fontId);
}

CTrueTypeFont* CFontManager::GetFont(int fontId)
{
	if (fontMap.find(fontId) != fontMap.end())
		return fontMap[fontId];
	return nullptr;
}
