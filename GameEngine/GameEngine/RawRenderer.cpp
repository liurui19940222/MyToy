#include"RawRenderer.h"
#include"Application.h"
#include<gl\glew.h>

CRawFontRenderer::CRawFontRenderer() : CFontRenderer()
{
	Init(NULL, 18, 0, Color::white, EAlignment::CENTER_MIDDLE, SRect2D(0.0f, 0.0f, 100.0f, 100.0f));
}

float CRawFontRenderer::GetPixelScale()
{
	return 1.0f;
}

CRawRenderer::CRawRenderer()
{
	
}

void CRawRenderer::SetupPixelFormat()
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	SetPixelFormat(m_hdc, pixelFormat, &pfd);
}

void CRawRenderer::SetupRenderContext(HWND hwnd)
{
	m_hwnd = hwnd;
	m_hdc = GetDC(hwnd);
	SetupPixelFormat();
	m_hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hrc);
	m_initialized = true;
}

void CRawRenderer::Release()
{
	wglMakeCurrent(m_hdc, NULL);
	wglDeleteContext(m_hrc);
}

void CRawRenderer::SetupProjection(float width, float height)
{
	if (height == 0)
		height = 1;
	m_width = (float)width;
	m_height = (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CRawRenderer::BeginOrtho()
{
	wglMakeCurrent(m_hdc, m_hrc);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_width, 0, m_height, -1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CRawRenderer::EndOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	SetupProjection(m_width, m_height);
	glEnable(GL_DEPTH_TEST);
}

void CRawRenderer::BeginBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CRawRenderer::EndBlend()
{
	glDisable(GL_BLEND);
}

void CRawRenderer::SetColor(const Color& color)
{
	glColor4f(color.r, color.g, color.b, color.a);
}

void CRawRenderer::DrawRect(const SRect2D& rect, const Color& color)
{
	glPushMatrix();
	SetColor(color);
	glTranslatef(rect.center_x, rect.center_y, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-rect.half_size_x, rect.half_size_y, 0.0f);
	glVertex3f(-rect.half_size_x, -rect.half_size_y, 0.0f);
	glVertex3f(rect.half_size_x, -rect.half_size_y, 0.0f);
	glVertex3f(rect.half_size_x, rect.half_size_y, 0.0f);
	glEnd();
	glPopMatrix();
}

void CRawRenderer::DrawLine(const Vector3& p0, const Vector3& p1, const Color& color, float width)
{
	SetColor(color);
	glPushMatrix();
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p1.x, p1.y, p1.z);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void CRawRenderer::DrawPoint(const Vector3& p, const Color& color, float size)
{
	SetColor(color);
	glPushMatrix();
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(p.x, p.y, p.z);
	glEnd();
	glPointSize(1.0f);
	glPopMatrix();
}

void CRawRenderer::DrawTexture(CTexture& texture, const Vector3& position)
{
	CRawRenderer::DrawTexture(texture, SRect2D{position.x, position.y, texture.GetWidth() * 0.5f, texture.GetHeight() * 0.5f});
}

void CRawRenderer::DrawTexture(CTexture& texture, const SRect2D& rect)
{
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslatef(rect.center_x, rect.center_y, 0.0f);
	texture.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(rect.half_size_x, rect.half_size_y, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-rect.half_size_x, rect.half_size_y, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-rect.half_size_x, -rect.half_size_y, 0);
	glTexCoord2f(1, 0);
	glVertex3f(rect.half_size_x, -rect.half_size_y, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glDisable(GL_BLEND);
}

void CRawRenderer::DrawString(const wstring& str, const SRect2D& rect, const Vector3& position, const Color& color, int size, EAlignment alignment)
{
	m_fontRenderer.SetRenderType(ERenderType::Fixed);
	m_fontRenderer.SetFont(FontManager->GetFont(1));
	m_fontRenderer.SetTextRect(rect);
	m_fontRenderer.SetColor(color);
	m_fontRenderer.SetFontSize(size);
	m_fontRenderer.SetTextAlignment(alignment);
	m_fontRenderer.SetText(str);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	m_fontRenderer.OnRender(Matrix4x4::Identity(), Matrix4x4::Identity(), Matrix4x4::Identity());
	glPopMatrix();
}

HDC CRawRenderer::GetDcHandle()
{
	return m_hdc;
}

HWND CRawRenderer::GetWindowHandle()
{
	return m_hwnd;
}

bool CRawRenderer::Initialized()
{
	return m_initialized;
}