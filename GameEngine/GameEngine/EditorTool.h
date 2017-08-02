#ifndef _CEDITOR_TOOL_
#define _CEDITOR_TOOL_

#include"Math.h"
#include"EngineDefine.h"

class CGameObject;

class CEditorTool
{
	static void PrintTree(CGameObject* go, int depth, bool showDepth);

public:
	CEditorTool();
	~CEditorTool();

	static void DrawQuad(Vector3 position, float size);

	static void DrawGrid(Vector3 cameraPos, Vector3 pos, Color color);

	static void DrawRect(SRect2D rect, Matrix4x4& modelToWorldMatrix);

	static void DrawVector(const Vector3& vector, const Vector3& pos, const Color& color);

	static void DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos);

	static void DrawAxis(const CGameObject* go);

	static void PrintTree(bool showDepth = false);

	static void PrintTree(CGameObject* go, bool showDepth = false);
};

#endif
