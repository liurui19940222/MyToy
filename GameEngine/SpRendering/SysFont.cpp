#include"SysFont.h"
#include<gl\glew.h>

USING_NAMESPACE_ENGINE

CSysFont::CSysFont(wchar_t* name, int size)
{
	HFONT hFont;

	m_FontListBase = glGenLists(96);

	hFont = CreateFont(size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
		name);
}

CSysFont::~CSysFont()
{
	if (m_FontListBase != 0)
		glDeleteLists(m_FontListBase, 96);
}

void CSysFont::Render(const char* text, const Vector3& position, HDC hdc)
{
	if ((m_FontListBase == 0) || (strlen(text) == 0))
		return;

	SelectObject(hdc, m_HFont);
	wglUseFontBitmaps(hdc, 32, 96, m_FontListBase);

	glPushMatrix();
	glRasterPos2i(position.x, position.y);

	glPushAttrib(GL_LIST_BIT);
	glListBase(m_FontListBase - 32);
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();

	glPopMatrix();
}
