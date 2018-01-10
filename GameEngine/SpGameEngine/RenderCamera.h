#ifndef _RENDER_CAMERA_H_
#define _RENDER_CAMERA_H_

#include"Math.h"
#include"..\SpCommon\EngineDefine.h"

enum ENGINE_API EProjectionType
{
	Perspective,
	Ortho
};

enum ENGINE_API ECameraClearFlag
{
	SolidColor,
	DontClear,
	SkyBox,
};

class ENGINE_API CRenderTexture;
class ENGINE_API CSkyBox;

class ENGINE_API CRenderCamera
{
protected:
	EProjectionType m_projectionType;
	ECameraClearFlag m_cameraClearFlag;
	CRenderTexture* m_renderTexture;
	CSkyBox* m_skybox;
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
	CRenderCamera();

	virtual CRenderCamera* Perspective(float fov, float aspect, float znear, float zfar);
	virtual CRenderCamera* Ortho(float left, float right, float bottom, float top, float znear, float zfar);
	virtual CRenderCamera* Ortho(float halfSize, float aspect);
	virtual CRenderCamera* LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	virtual CRenderCamera* SetDepth(int depth);
	CRenderCamera* SetCameraClearFlag(ECameraClearFlag flag);
	CRenderCamera* SetCameraClearColor(const Color& color);
	CRenderCamera* SetRenderTexture(CRenderTexture* texture);
	CRenderCamera* SetSkyBox(CSkyBox* skybox);
	EProjectionType GetProjectionType() const;
	ECameraClearFlag GetCameraClearFlag() const;
	CRenderTexture* GetRenderTexture() const;
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

	Vector3 WorldPosToScreen(Vector3 worldPos);
	Vector3 ScreenPosToViewPort(Vector3 screenPos);
};

#endif