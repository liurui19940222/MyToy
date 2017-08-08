#include "Camera.h"
#include "GameObject.h"
#include "EngineDefine.h"
#include "Engine.h"
#include "Application.h"
#include "RenderTexture.h"

IMPL_CLASS(CCamera)

void CCamera::OnStart()
{
	Engine->AddCamera(this);
}

void CCamera::OnDestroy()
{
	Engine->RemoveCamera(this);
}

void CCamera::OnUpdate()
{
	
}

void CCamera::OnRender()
{

}

CRenderCamera* CCamera::Perspective(float fov, float aspect, float znear, float zfar)
{
	CRenderCamera::Perspective(fov, aspect, znear, zfar);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	return this;
}

CRenderCamera* CCamera::Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	CRenderCamera::Ortho(left, right, bottom, top, znear, zfar);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	return this;
}

CRenderCamera* CCamera::Ortho(float halfSize, float aspect)
{
	CRenderCamera::Ortho(halfSize, aspect);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-halfSize * aspect, halfSize * aspect, -halfSize, halfSize, 0.01f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	return this;
}

CRenderCamera* CCamera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	CRenderCamera::LookAt(eye, center, up);
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	return this;
}

CRenderCamera* CCamera::SetDepth(int depth)
{
	CRenderCamera::SetDepth(depth);
	Engine->RemoveCamera(this);
	Engine->AddCamera(this);
	return this;
}

void CCamera::BeginOneFrame()
{
	if (m_renderTexture)
	{
		m_renderTexture->BindBuffer();
		glViewport(0, 0, m_renderTexture->GetWidth(), m_renderTexture->GetHeight());
	}
	else
	{
		glViewport(0, 0, Application->GetWindowWidth(), Application->GetWindowHeight());
	}
	if (m_cameraClearFlag == ECameraClearFlag::SolidColor)
	{
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	glLoadIdentity();
	static Vector3 position;
	static Vector3 forward;
	static Vector3 up;
	position = this->gameObject->GetLocalPosition();
	forward = this->gameObject->GetForward();
	up = this->gameObject->GetUp();
	LookAt(position, position + forward, up);
}

void CCamera::EndTheFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
}
