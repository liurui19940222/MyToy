#ifndef _SYS_FONT_H_
#define _SYS_FONT_H_

#include<Windows.h>
#include"SpCommon\Math.h"

namespace spgameengine
{

	class CSysFont
	{
		unsigned int		m_FontListBase;
		HFONT				m_HFont;

		~CSysFont();
	public:
		CSysFont(wchar_t* name, int size);

		void Render(const char* text, const Vector3& position, HDC hdc);
	};

}

#endif