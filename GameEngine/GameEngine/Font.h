#ifndef _FONT_H_
#define _FONT_H_

#include <vector>
#include <map>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftbitmap.h>

#include"GLHead.h"
#include"Object.h"
#include"EngineDefine.h"

using namespace std;

#define MAX_NO_TEXTURES 1

#define CUBE_TEXTURE 0


struct xCharTexture
{
	GLuint  m_texID;
	wchar_t m_chaID;
	int     m_Width;
	int     m_Height;

	int     m_adv_x;
	int     m_adv_y;
	int     m_delta_x;
	int     m_delta_y;
public:
	xCharTexture()
	{
		m_texID = 0;
		m_chaID = 0;
		m_Width = 0;
		m_Height = 0;
	}
};

class Font : Object
{
private:
	FT_Library m_FT2Lib;
	FT_Face    m_FT_Face;

public:
	Font();
	~Font();

	int   m_w;
	int   m_h;

	void Load(const char* font_file, int _w, int _h);

	GLuint LoadChar(wchar_t ch, Color32& color, int size);

	xCharTexture* GetTextChar(wchar_t ch, Color32& color, int size);

	void Draw(wchar_t* _strText, int x, int y, int maxW, int h, Color32& color, int size);
};

#endif