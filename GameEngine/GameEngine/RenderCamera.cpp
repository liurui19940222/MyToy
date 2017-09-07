#include"RenderCamera.h"
#include"RenderTexture.h"
#include"Application.h"

CRenderCamera::CRenderCamera() : m_projectionType(EProjectionType::Perspective) , m_cameraClearFlag(ECameraClearFlag::SolidColor)
, m_fov(0) , m_near(0) , m_far(0) , m_left(0) , m_right(0)
, m_top(0) , m_bottom(0), m_depth(0), m_layerMask(Layer::Default)
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

CRenderCamera* CRenderCamera::Ortho(float halfSize, float aspect)
{
	return Ortho(-halfSize * aspect, halfSize * aspect, -halfSize, halfSize, 0.01f, 1000.0f);
}

CRenderCamera* CRenderCamera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	m_eyePos = eye;
	m_centerPos = center;
	m_up = up;
	m_viewMat.MakeLookAt(eye, center, up);
	return this;
}

CRenderCamera* CRenderCamera::SetDepth(int depth)
{
	m_depth = depth;
	return this;
}

CRenderCamera* CRenderCamera::SetRenderTexture(CRenderTexture* texture)
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

CRenderCamera* CRenderCamera::SetCameraClearFlag(ECameraClearFlag flag) { m_cameraClearFlag = flag; return this; }
CRenderCamera* CRenderCamera::SetCameraClearColor(const Color& color) { m_clearColor = color; return this; }
EProjectionType CRenderCamera::GetProjectionType() const { return m_projectionType; }
ECameraClearFlag CRenderCamera::GetCameraClearFlag() const { return m_cameraClearFlag; }
CRenderTexture* CRenderCamera::GetRenderTexture() const { return m_renderTexture; }
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
int& CRenderCamera::LayerMask() { return m_layerMask; }
const Vector3& CRenderCamera::GetEyePosition() const { return m_eyePos; }
const Vector3& CRenderCamera::GetCenterPosition() const { return m_centerPos; }
const Vector3& CRenderCamera::GetUp() const { return m_up; }
Matrix4x4 CRenderCamera::GetViewMatrix() const { return m_viewMat; }
Matrix4x4 CRenderCamera::GetProjectionMatrix() const { return m_projectionMat; }

Vector3 CRenderCamera::WorldPosToScreen(Vector3 worldPos)
{
	Vector4 pos = m_projectionMat * m_viewMat * Vector4(worldPos, 1.0f);
	float w = 1.0 / pos.w;
	pos.x *= w; pos.y *= w; pos.z *= w;
	pos.x = _SCW * (pos.x + 0.5f);
	pos.y = _SCH - _SCH * (pos.y + 0.5f);
	return pos;
}

Vector3 CRenderCamera::ScreenPosToViewPort(Vector3 screenPos)
{
	return Vector3(screenPos.x - _SCW * 0.5f, -screenPos.y + _SCH * 0.5f, screenPos.z);
}