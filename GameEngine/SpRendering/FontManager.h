#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include"TrueTypeFont.h"
#include"SpCommon\Singleton.h"

BEGIN_NAMESPACE_ENGINE

#define _LoadFont(fontId, file_name) FontManager::GetInstance()->LoadFont(fontId, file_name)
#define _GetFont(fontId) FontManager::GetInstance()->GetFont(fontId)
#define _FontManager FontManager::GetInstance()

class FontManager : public Singleton<FontManager>
{
private:
	map<int, TrueTypeFontPtr> fontMap;

public:
	TrueTypeFontPtr LoadFont(int fontId, const char* file_name);

	TrueTypeFontPtr GetFont(int fontId);
};

END_NAMESPACE_ENGINE

#endif