#ifndef _CEDITOR_TOOL_
#define _CEDITOR_TOOL_

#include"Math.h"
#include"EngineDefine.h"

class CEditorTool
{
public:
	CEditorTool();
	~CEditorTool();

	static void DrawQuad(Vector3 position, float size);

	static void DrawGrid(Vector3 cameraPos, Vector3 pos, Color color);

	static void DrawRect(SRect2D rect, Matrix4x4& modelToWorldMatrix);
};

#endif
