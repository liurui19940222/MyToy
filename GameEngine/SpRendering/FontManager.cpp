#include "FontManager.h"

USING_NAMESPACE_ENGINE

PTrueTypeFont FontManager::LoadFont(int fontId, const char* file_name)
{
	if (fontMap.find(fontId) == fontMap.end())
	{
		PTrueTypeFont font = make_shared<TrueTypeFont>(fontId, file_name);
		fontMap.insert(make_pair(fontId, font));
	}
	return GetFont(fontId);
}

PTrueTypeFont FontManager::GetFont(int fontId)
{
	if (fontMap.find(fontId) != fontMap.end())
		return fontMap[fontId];
	return nullptr;
}
