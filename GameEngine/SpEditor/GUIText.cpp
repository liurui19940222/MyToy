#include"GUIText.h"
#include"GUIManager.h"

CGUIText::CGUIText() : CGUIElement(), m_color(Color::white), m_alignment(EAlignment::CENTER_MIDDLE), m_fontSize(13)
{
	SetFill(false);
}

void CGUIText::OnRender()
{
	CGUIElement::OnRender();
	if (m_text.size() > 0 && Visible())
		m_manager->GetRenderer()->DrawString(m_text, m_rect, m_position, m_color, m_fontSize, m_alignment);
}

CGUIText* CGUIText::SetText(const wstring& text)
{
	m_text = text;
	return this;
}

CGUIText* CGUIText::SetColor(const Color& color)
{
	m_color = color;
	return this;
}

CGUIText* CGUIText::SetAlignment(EAlignment alignment)
{
	m_alignment = alignment;
	return this;
}

CGUIText* CGUIText::SetFontSize(int fontSize)
{
	m_fontSize = fontSize;
	return this;
}

wstring CGUIText::GetText() const
{
	return m_text;
}