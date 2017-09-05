#ifndef _ENGINE_SETTING_H_
#define _ENGINE_SETTING_H_

#include"ApiDefine.h"

class ENGINE_API CEngineSetting
{
public:
	static bool DrawGrid;
	static bool DrawGizmos;

	static void Init();
};

#endif

