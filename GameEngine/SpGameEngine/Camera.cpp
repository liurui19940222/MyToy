#include "Camera.h"
#include "GameObject.h"
#include "SpCommon\EngineDefine.h"
#include "Engine.h"
#include "Application.h"
#include"SpRendering\RenderTexture.h"
#include"SpRendering\SkyBox.h"

IMPL_CLASS(CCamera)

void CCamera::OnStart()
{
	_Engine->AddCamera(this);
}

void CCamera::OnDestroy()
{
	_Engine->RemoveCamera(this);
}

void CCamera::OnUpdate()
{

}

CRenderCamera* CCamera::Perspective(float fov, float aspect, float znear, float zfar)
{
	CRenderCamera::Perspective(fov, aspect, znear, zfar);
	return this;
}

CRenderCamera* CCamera::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	CRenderCamera::Ortho(left, right, bottom, top, znear, zfar);
	return this;
}

CRenderCamera* CCamera::Ortho(float halfSize, float aspect)
{
	CRenderCamera::Ortho(halfSize, aspect);
	return this;
}

CRenderCamera* CCamera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	CRenderCamera::LookAt(eye, center, up);
	if (gameObject->GetTag() == "MainCamera")
		gluLookAt(m_eyePos.x, m_eyePos.y, m_eyePos.z, m_centerPos.x, m_centerPos.y, m_centerPos.z, m_up.x, m_up.y, m_up.z);
	return this;
}

CRenderCamera* CCamera::SetDepth(int depth)
{
	CRenderCamera::SetDepth(depth);
	_Engine->RemoveCamera(this);
	_Engine->AddCamera(this);
	return this;
}

CCamera* CCamera::UpdateViewMatrix()
{
	static Vector3 position;
	static Vector3 forward;
	static Vector3 up;
	position = this->gameObject->GetLocalPosition();
	forward = this->gameObject->GetForward();
	up = this->gameObject->GetUp();
	LookAt(position, position + forward, up);
	return this;
}

void CCamera::BeginOneFrame()
{
	if (m_renderTexture)
	{
		glPushAttrib(GL_VIEWPORT_BIT);
		m_renderTexture->BindBuffer();
		glViewport(0, 0, m_renderTexture->GetWidth(), m_renderTexture->GetHeight());
	}
	else
	{
		glViewport(0, 0, _SCW, _SCH);
	}
	if (m_cameraClearFlag == ECameraClearFlag::SolidColor)
	{
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	PrepareFixed();
	UpdateViewMatrix();
	if (m_cameraClearFlag == ECameraClearFlag::SkyBox)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		if (m_skybox) m_skybox->Render(Matrix4x4::Scale(Vector3::one * 50), m_viewMat, m_projectionMat);
	}
}

void CCamera::EndTheFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	if (m_renderTexture)
	{
		glPopAttrib();
	}
}

void CCamera::PrepareFixed()
{
	if (gameObject->GetTag() == "MainCamera")
	{
		if (m_projectionType == EProjectionType::Perspective)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_fov, m_aspect, m_near, m_far);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		else if (m_projectionType == EProjectionType::Ortho)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(m_left, m_right, m_bottom, m_top, m_near, m_far);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
	}
}