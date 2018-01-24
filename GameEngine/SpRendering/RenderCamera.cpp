#include"RenderCamera.h"
#include"RenderTexture.h"

using namespace spgameengine;

RenderCamera::RenderCamera() : m_projectionType(EProjectionType::Perspective) , m_cameraClearFlag(ECameraClearFlag::SolidColor)
, m_fov(0) , m_near(0) , m_far(0) , m_left(0) , m_right(0)
, m_top(0) , m_bottom(0), m_depth(0), m_layerMask(Layer::Default), m_skybox(NULL)
{
	m_viewMat.MakeIdentity();
	m_projectionMat.MakeIdentity();
}

void RenderCamera::Perspective(float fov, float aspect, float znear, float zfar)
{
	m_fov = fov;
	m_aspect = aspect;
	m_near = znear;
	m_far = zfar;
	m_projectionType = EProjectionType::Perspective;
	m_projectionMat.MakePerspective(fov, aspect, znear, zfar);
}

void RenderCamera::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_near = znear;
	m_far = zfar;
	m_projectionType = EProjectionType::Ortho;
	m_projectionMat.MakeOrtho(left, right, bottom, top, znear, zfar);
}

void RenderCamera::Ortho(float halfSize, float aspect)
{
	Ortho(-halfSize * aspect, halfSize * aspect, -halfSize, halfSize, 0.01f, 1000.0f);
}

void RenderCamera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	m_eyePos = eye;
	m_centerPos = center;
	m_up = up;
	m_viewMat.MakeLookAt(eye, center, up);
}

void RenderCamera::SetDepth(int depth)
{
	m_depth = depth;
}

RenderCamera* RenderCamera::SetRenderTexture(PRenderTexture texture)
{
	m_renderTexture = texture;
	if (m_renderTexture)
	{
		if (m_projectionType == EProjectionType::Perspective)
			Perspective(m_fov, (float)texture->GetWidth() / (float)texture->GetHeight(), m_near, m_far);
		else if (m_projectionType == EProjectionType::Ortho)
			Ortho(m_top, (float)texture->GetWidth() / (float)texture->GetHeight());
	}
	return this;
}

RenderCamera* RenderCamera::SetSkyBox(PSkyBox skybox) { m_skybox = skybox; m_cameraClearFlag = ECameraClearFlag::SkyBox; return this; }
RenderCamera* RenderCamera::SetCameraClearFlag(ECameraClearFlag flag) { m_cameraClearFlag = flag; return this; }
RenderCamera* RenderCamera::SetCameraClearColor(const Color& color) { m_clearColor = color; return this; }
EProjectionType RenderCamera::GetProjectionType() const { return m_projectionType; }
ECameraClearFlag RenderCamera::GetCameraClearFlag() const { return m_cameraClearFlag; }
PRenderTexture RenderCamera::GetRenderTexture() const { return m_renderTexture; }
const Color& RenderCamera::GetCameraClearColor() const { return m_clearColor; }
float RenderCamera::GetFov() const { return m_fov; }
float RenderCamera::GetAspect() const { return m_aspect; }
float RenderCamera::GetNear() const { return m_near; }
float RenderCamera::GetFar() const { return m_far; }
float RenderCamera::GetLeft() const { return m_left; }
float RenderCamera::GetRight() const { return m_right; }
float RenderCamera::GetTop() const { return m_top; }
float RenderCamera::GetBottom() const { return m_bottom; }
int RenderCamera::GetDepth() const { return m_depth; }
int& RenderCamera::LayerMask() { return m_layerMask; }
const Vector3& RenderCamera::GetEyePosition() const { return m_eyePos; }
const Vector3& RenderCamera::GetCenterPosition() const { return m_centerPos; }
const Vector3& RenderCamera::GetUp() const { return m_up; }
Matrix4x4 RenderCamera::GetViewMatrix() const { return m_viewMat; }
Matrix4x4 RenderCamera::GetProjectionMatrix() const { return m_projectionMat; }

Vector3 RenderCamera::WorldPosToScreen(Vector3 worldPos, float viewport_w, float viewport_h)
{
	Vector4 pos = m_projectionMat * m_viewMat * Vector4(worldPos, 1.0f);
	float w = 1.0f / pos.w;
	pos.x *= w; pos.y *= w; pos.z *= w;
	pos.x = viewport_w * (pos.x + 0.5f);
	pos.y = viewport_h - viewport_h * (pos.y + 0.5f);
	return pos;
}

Vector3 RenderCamera::ScreenPosToViewPort(Vector3 screenPos, float viewport_w, float viewport_h)
{
	return Vector3(screenPos.x - viewport_w * 0.5f, -screenPos.y + viewport_h * 0.5f, screenPos.z);
}