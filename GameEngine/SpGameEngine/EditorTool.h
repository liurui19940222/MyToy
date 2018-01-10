#ifndef _CEDITOR_TOOL_
#define _CEDITOR_TOOL_

#include"Math.h"
#include"SkeletonAnimation.h"
#include"..\SpCommon\EngineDefine.h"
#include"..\SpCommon\ApiDefine.h"

class ENGINE_API CGameObject;

class ENGINE_API CEditorTool
{
	static void PrintTree(CGameObject* go, int depth, bool showDepth);

public:

	static void DrawQuad(const Vector3& position, float size);
	static void DrawGrid(const Vector3& cameraPos, const Vector3& pos, const Color& color);
	static void DrawCone(const Matrix4x4& matrix, const Color& color, float radius, float height);
	static void DrawRect(const SRect2D& rect, const Matrix4x4& modelToWorldMatrix);
	static void DrawVector(const Vector3& vector, const Vector3& pos, const Color& color);
	static void DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos);
	static void DrawAxis(Matrix4x4& modelToWorldMatrix, const Vector3& scale);
	static void DrawAxis(Matrix4x4& modelToWorldMatrix);
	static void DrawSkeleton(Matrix4x4& modelToWorldMatrix, Skeleton& skeleton);
	static void PrintTree(bool showDepth = false);
	static void PrintTree(CGameObject* go, bool showDepth = false);
	static void WatchTarget(CGameObject& camera, const Vector3& targetPos);
};

#endif
