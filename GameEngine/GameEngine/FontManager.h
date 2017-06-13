#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include"TrueTypeFont.h"

#define _LoadFont(fontId, file_name) CFontManager::GetInstance()->LoadFont(fontId, file_name)
#define _GetFont(fontId) CFontManager::GetInstance()->GetFont(fontId)

class CFontManager
{
private:
	static CFontManager* instance;

	int font_id;

	map<int, CTrueTypeFont*> fontMap;

public:
	static CFontManager* GetInstance();

	CTrueTypeFont* LoadFont(int fontId, const char* file_name);

	CTrueTypeFont* GetFont(int fontId);
};

#endif