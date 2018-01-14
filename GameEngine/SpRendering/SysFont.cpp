#include"SysFont.h"
#include<gl\glew.h>

CSysFont::CSysFont(wchar_t* name, int size)
{
	HFONT hFont;

	m_fontListBase = glGenLists(96);

	hFont = CreateFont(size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
		name);
}

CSysFont::~CSysFont()
{
	if (m_fontListBase != 0)
		glDeleteLists(m_fontListBase, 96);
}

void CSysFont::Render(const char* text, const Vector3& position, HDC hdc)
{
	if ((m_fontListBase == 0) || (strlen(text) == 0))
		return;

	SelectObject(hdc, m_hfont);
	wglUseFontBitmaps(hdc, 32, 96, m_fontListBase);

	glPushMatrix();
	glRasterPos2i(position.x, position.y);

	glPushAttrib(GL_LIST_BIT);
	glListBase(m_fontListBase - 32);
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();

	glPopMatrix();
}
