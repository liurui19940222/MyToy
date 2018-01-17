#pragma once

#include"Math.h"
#include"EngineDefine.h"
#include<gl\glew.h>

class FastPainter
{
	static HDC hdc;
	static HGLRC hrc;
public:
	static void Init(HWND hwnd, int width, int height);
	static void SetupPixelFormat(HDC hDC);
	static void MakeRenderContext();
	static void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	static void Viewport(int x, int y, int width, int height);
	static void Perspective(float fov, float aspect, float znear, float zfar);

	static void DrawQuad(const Vector3& position, float size);
	static void DrawGrid(const Vector3& cameraPos, const Vector3& pos, const Color& color);
	static void DrawCone(const Matrix4x4& matrix, const Color& color, float radius, float height);
	static void DrawRect(const SRect2D& rect, const Matrix4x4& modelToWorldMatrix);
	static void DrawVector(const Vector3& vector, const Vector3& pos, const Color& color);
	static void DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos);
	static void DrawAxis(Matrix4x4& modelToWorldMatrix, const Vector3& scale);
	static void DrawAxis(Matrix4x4& modelToWorldMatrix);
};

