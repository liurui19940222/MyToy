#ifndef _RENDER_CAMERA_H_
#define _RENDER_CAMERA_H_

#include"SpCommon\Math.h"
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Object.h"

BEGIN_NAMESPACE_ENGINE

enum EProjectionType
{
	Perspective,
	Ortho
};

enum class ECameraClearFlag
{
	SolidColor,
	DontClear,
	SkyBox,
};

SMART_CLASS(RenderTexture) class RenderTexture;
SMART_CLASS(SkyBox) class SkyBox;

SMART_CLASS(RenderCamera) class RenderCamera
{
protected:
	EProjectionType m_projectionType;
	ECameraClearFlag m_cameraClearFlag;
	RenderTexturePtr m_renderTexture;
	SkyBoxPtr m_skybox;
	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
	int m_depth;
	int m_layerMask;

	Color m_clearColor;
	Vector3 m_eyePos;
	Vector3 m_centerPos;
	Vector3 m_up;
	Matrix4x4 m_viewMat;
	Matrix4x4 m_projectionMat;

public:
	RenderCamera();

	virtual void Perspective(float fov, float aspect, float znear, float zfar);
	virtual void Ortho(float left, float right, float bottom, float top, float znear, float zfar);
	virtual void Ortho(float halfSize, float aspect);
	virtual void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	virtual void SetDepth(int depth);
	RenderCamera* SetCameraClearFlag(ECameraClearFlag flag);
	RenderCamera* SetCameraClearColor(const Color& color);
	RenderCamera* SetRenderTexture(RenderTexturePtr texture);
	RenderCamera* SetSkyBox(SkyBoxPtr skybox);
	EProjectionType GetProjectionType() const;
	ECameraClearFlag GetCameraClearFlag() const;
	RenderTexturePtr GetRenderTexture() const;
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
	int& LayerMask();
	const Vector3& GetEyePosition() const;
	const Vector3& GetCenterPosition() const;
	const Vector3& GetUp() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;

	Vector3 WorldPosToScreen(Vector3 worldPos, float viewport_w, float viewport_h);
	Vector3 ScreenPosToViewPort(Vector3 screenPos, float viewport_w, float viewport_h);
};

END_NAMESPACE_ENGINE

#endif