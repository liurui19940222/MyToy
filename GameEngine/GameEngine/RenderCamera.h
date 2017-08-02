#ifndef _RENDER_CAMERA_H_
#define _RENDER_CAMERA_H_

#include"Math.h"
#include"EngineDefine.h"

enum EProjectionType
{
	Perspective,
	Ortho
};

enum ECameraClearFlag
{
	SolidColor,
	DontClear,
};

class CRenderCamera
{
protected:
	EProjectionType m_projectionType;
	ECameraClearFlag m_cameraClearFlag;
	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
	int m_depth;

	Color m_clearColor;
	Vector3 m_eyePos;
	Vector3 m_centerPos;
	Vector3 m_up;
	Matrix4x4 m_viewMat;
	Matrix4x4 m_projectionMat;

public:
	CRenderCamera();

	CRenderCamera* Perspective(float fov, float aspect, float znear, float zfar);
	CRenderCamera* Ortho(float left, float right, float bottom, float top, float znear, float zfar);
	CRenderCamera* LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	CRenderCamera* SetCameraClearFlag(ECameraClearFlag flag);
	CRenderCamera* SetCameraClearColor(const Color& color);

	EProjectionType GetProjectionType() const;
	ECameraClearFlag GetCameraClearFlag() const;
	const Color& GetCameraClearColor() const;
	float GetFov() const;
	float GetAspect() const;
	float GetNear() const;
	float GetFar() const;
	float GetLeft() const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom() const;
	int GetDepth() const;
	const Vector3& GetEyePosition() const;
	const Vector3& GetCenterPosition() const;
	const Vector3& GetUp() const;
	const Matrix4x4& GetViewMatrix() const;
	const Matrix4x4& GetProjectionMatrix() const;
};

#endif