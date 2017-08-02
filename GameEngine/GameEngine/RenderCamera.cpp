#include "RenderCamera.h"

#pragma comment(lib, "glew32.lib")

CRenderCamera::CRenderCamera() : m_projectionType(EProjectionType::Perspective) , m_cameraClearFlag(ECameraClearFlag::SolidColor)
, m_fov(0) , m_near(0) , m_far(0) , m_left(0) , m_right(0)
, m_top(0) , m_bottom(0), m_depth(0)
{
	m_viewMat.MakeIdentity();
	m_projectionMat.MakeIdentity();
}

CRenderCamera* CRenderCamera::Perspective(float fov, float aspect, float znear, float zfar)
{
	m_fov = fov;
	m_aspect = aspect;
	m_near = znear;
	m_far = zfar;
	m_projectionType = EProjectionType::Perspective;
	m_projectionMat.MakePerspective(fov, aspect, znear, zfar);
	return this;
}

CRenderCamera* CRenderCamera::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_near = znear;
	m_far = zfar;
	m_projectionType = EProjectionType::Ortho;
	m_projectionMat.MakeOrtho(left, right, bottom, top, znear, zfar);
	return this;
}

CRenderCamera* CRenderCamera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	m_eyePos = eye;
	m_centerPos = center;
	m_up = up;
	m_viewMat.MakeLookAt(eye, center, up);
	return this;
}

CRenderCamera* CRenderCamera::SetCameraClearFlag(ECameraClearFlag flag) { m_cameraClearFlag = flag; return this; }
CRenderCamera* CRenderCamera::SetCameraClearColor(const Color& color) { m_clearColor = color; return this; }
EProjectionType CRenderCamera::GetProjectionType() const { return m_projectionType; }
ECameraClearFlag CRenderCamera::GetCameraClearFlag() const { return m_cameraClearFlag; }
const Color& CRenderCamera::GetCameraClearColor() const { return m_clearColor; }
float CRenderCamera::GetFov() const { return m_fov; }
float CRenderCamera::GetAspect() const { return m_aspect; }
float CRenderCamera::GetNear() const { return m_near; }
float CRenderCamera::GetFar() const { return m_far; }
float CRenderCamera::GetLeft() const { return m_left; }
float CRenderCamera::GetRight() const { return m_right; }
float CRenderCamera::GetTop() const { return m_top; }
float CRenderCamera::GetBottom() const { return m_bottom; }
int CRenderCamera::GetDepth() const { return m_depth; }
const Vector3& CRenderCamera::GetEyePosition() const { return m_eyePos; }
const Vector3& CRenderCamera::GetCenterPosition() const { return m_centerPos; }
const Vector3& CRenderCamera::GetUp() const { return m_up; }
const Matrix4x4& CRenderCamera::GetViewMatrix() const { return m_viewMat; }
const Matrix4x4& CRenderCamera::GetProjectionMatrix() const { return m_projectionMat; }