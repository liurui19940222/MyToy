#ifndef _GUI_TEXT_H_
#define _GUI_TEXT_H_

#include"GUIElement.h"
#include<string>

using namespace std;

class CGUIText : public CGUIElement
{
private:
	wstring m_text;
	EAlignment m_alignment;
	Color m_color;
	int m_fontSize;

public:
	CGUIText();
	virtual void OnRender() override;

	CGUIText* SetText(const wstring& text);
	CGUIText* SetColor(const Color& color);
	CGUIText* SetAlignment(EAlignment alignment);
	CGUIText* SetFontSize(int fontSize);

	wstring GetText() const;
};

#endif