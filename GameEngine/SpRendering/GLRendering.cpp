#include "GLRendering.h"
#include <gl\glew.h>

void GLRendering::StartUp(const RenderingStartUpParams* params)
{
	GLRenderingStartUpParams* _params = (GLRenderingStartUpParams*)params;
	m_HDC = GetDC(_params->m_HWND);
	SetupPixelFormat(m_HDC);
	m_HRC = wglCreateContext(m_HDC);
	MakeRenderContext();
	glewExperimental = GL_TRUE;
	glewInit();
}

void GLRendering::ShutDown()
{
	wglDeleteContext(m_HRC);
}

void GLRendering::Render(const RenderingObject* obj, Matrix4x4& modelMatrix,
	Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (obj == NULL) return;
	obj->material->Bind();
	obj->material->SetParam("M", modelMatrix);
	obj->material->SetParam("V", viewMatrix);
	obj->material->SetParam("P", projectionMatrix);
	obj->mesh->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, obj->mesh->GetVertexNum());
	obj->material->Unbind();
}

void GLRendering::MakeRenderContext()
{
	wglMakeCurrent(m_HDC, m_HRC);
}

void GLRendering::SetupPixelFormat(HDC hDC)
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}