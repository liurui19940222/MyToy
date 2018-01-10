#ifndef _ENGINE_SETTING_H_
#define _ENGINE_SETTING_H_

#include"..\SpCommon\ApiDefine.h"

class ENGINE_API CEngineSetting
{
public:
	static bool DrawGrid;
	static float GridStrength;
	static bool DrawGizmos;
	static bool DrawGUILayout;

	static void Init();
};

#endif

