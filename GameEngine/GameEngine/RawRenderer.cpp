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

map<int, CSysFont*> CRawRenderer::m_fonts;
CRawFontRenderer CRawRenderer::m_fontRenderer;

CSysFont* CRawRenderer::PrepareFont(int size)
{
	auto it = m_fonts.find(size);
	if (it == m_fonts.end())
	{
		CSysFont* font = new CSysFont(L"Verdana", size);
		m_fonts.insert(make_pair(size, font));
		return font;
	}
	return it->second;
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

void CRawRenderer::DrawString(const string& str, const Vector3& position, const Color& color, int size)
{
	glPushMatrix();
	SetColor(color);
	CSysFont* font = PrepareFont(size);
	font->Render(str.c_str(), position);
	glPopMatrix();
}

void CRawRenderer::DrawLine(const Vector3& p0, const Vector3& p1, const Color& color, float width)
{

}

void CRawRenderer::DrawPoint(const Vector3& p, float size)
{

}

void CRawRenderer::DrawTexture(const CTexture& texture, const Vector3& position)
{

}

void CRawRenderer::RenderString(const wstring& str, const SRect2D& rect, const Vector3& position, const Color& color, int size, EAlignment alignment)
{
	m_fontRenderer.SetTextRect(rect);
	m_fontRenderer.SetColor(color);
	m_fontRenderer.SetFontSize(size);
	m_fontRenderer.SetTextAlignment(alignment);
	m_fontRenderer.SetText(str);
	m_fontRenderer.OnRender(Matrix4x4::Translate(position), _MainCamera->GetViewMatrix(), _MainCamera->GetProjectionMatrix());
}