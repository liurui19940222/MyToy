#include "FontManager.h"

USING_NAMESPACE_ENGINE

TrueTypeFontPtr FontManager::LoadFont(int fontId, const char* file_name)
{
	if (fontMap.find(fontId) == fontMap.end())
	{
		TrueTypeFontPtr font = make_shared<TrueTypeFont>(fontId, file_name);
		fontMap.insert(make_pair(fontId, font));
	}
	return GetFont(fontId);
}

TrueTypeFontPtr FontManager::GetFont(int fontId)
{
	if (fontMap.find(fontId) != fontMap.end())
		return fontMap[fontId];
	return nullptr;
}
