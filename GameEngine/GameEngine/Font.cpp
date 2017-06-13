#include "Font.h"

//#pragma comment(lib , "freetype/freetype.lib")

GLuint texture_id[MAX_NO_TEXTURES];

xCharTexture g_CharTexID[65536];

Color32 m_Colors[65536];

Font::Font()
{
}


Font::~Font()
{
}

void Font::Load(const char* font_file, int _w, int _h)
{
	FT_Library library;
	if (FT_Init_FreeType(&library))
		return;
	m_FT2Lib = library;
	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face(library, font_file, 0, &m_FT_Face))
		return;
	//选择字符表
	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	m_w = _w; m_h = _h;
	m_FT_Face->num_fixed_sizes;
	//大小要乘64.这是规定。照做就可以了。
	FT_Set_Char_Size(m_FT_Face, _w << 6, (int)(_w * 1.4) << 6, 96, 96);
	//用来存放指定字符宽度和高度的特定数据
	//FT_Set_Pixel_Sizes(m_FT_Face, m_w, m_h);
}

GLuint Font::LoadChar(wchar_t ch, Color32& color, int size)
{
	if (color == m_Colors[ch])
	{
		if (g_CharTexID[ch].m_texID)
			return g_CharTexID[ch].m_texID;
		/* 装载字形图像到字形槽（将会抹掉先前的字形图像） */
		if (FT_Load_Char(m_FT_Face, ch,
			FT_LOAD_DEFAULT |
			FT_LOAD_MONOCHROME |
			FT_LOAD_TARGET_MONO))
		{
			return 0;
		}
	}
	else m_Colors[ch] = color;

	/*if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, ch ), FT_LOAD_FORCE_AUTOHINT ))
	throw std::runtime_error("FT_Load_Glyph failed");*/

	xCharTexture& charTex = g_CharTexID[ch];

	//得到字模
	FT_Glyph glyph;
	//把字形图像从字形槽复制到新的FT_Glyph对象glyph中。这个函数返回一个错误码并且设置glyph。 
	if (FT_Get_Glyph(m_FT_Face->glyph, &glyph))
		return 0;

	//转化成位图
	FT_Render_Glyph(m_FT_Face->glyph, FT_RENDER_MODE_MONO);//FT_RENDER_MODE_NORMAL  ); 
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据
	FT_Bitmap &bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int width = bitmap.width;
	int height = bitmap.rows;

	m_FT_Face->size->metrics.y_ppem;		//伸缩距离到设备空间
	m_FT_Face->glyph->metrics.horiAdvance;  //水平文本排列


	charTex.m_Width = width;
	charTex.m_Height = height;
	charTex.m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;  //步进宽度
	charTex.m_adv_y = m_FT_Face->size->metrics.y_ppem;		//m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	charTex.m_delta_x = (float)bitmap_glyph->left;			//left:字形原点(0,0)到字形位图最左边象素的水平距离.它以整数象素的形式表示。 
	charTex.m_delta_y = (float)bitmap_glyph->top - height;	//Top: 类似于字形槽的bitmap_top字段。
	glGenTextures(1, &charTex.m_texID);
	glBindTexture(GL_TEXTURE_2D, charTex.m_texID);

	unsigned char* pBuf = new unsigned char[width * height * 4];
	int i, j, index, col;

	FT_Bitmap nb;
	FT_Bitmap_New(&nb);
	//nb.pitch = 8;
	//nb.width = width;
	//nb.rows = height;
	//nb.buffer = new unsigned char[width * height * 4];
	FT_Bitmap_Convert(m_FT2Lib, &bitmap, &nb, 1);
	width = nb.width;
	height = nb.rows;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			unsigned char _vl = (i >= nb.width || j >= nb.rows) ? 0 : nb.buffer[i + nb.width*j];
			pBuf[(4 * i + (height - j - 1) * width * 4)] = color.r;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 1] = color.g;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 2] = color.b;
			pBuf[(4 * i + (height - j - 1) * width * 4) + 3] = _vl > 0 ? 0xFF : 0x00;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);  //指定一个二维的纹理图片
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);							   //glTexParameteri():纹理过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);								//纹理进行混合

	/*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);*/
	FT_Bitmap_Done(m_FT2Lib, &nb);
	delete[] pBuf;
	return charTex.m_chaID;
}

xCharTexture* Font::GetTextChar(wchar_t ch, Color32& color, int size)
{
	LoadChar(ch, color, size);
	return &g_CharTexID[ch];
}

void Font::Draw(wchar_t* _strText, int x, int y, int maxW, int h, Color32& color, int size)
{
	y = -y;
	int sx = x;
	int sy = y;
	int maxH = h;
	size_t nLen = wcslen(_strText);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(1, -1, 1);
	float total_w = 0;
	float total_h = 0;
	for (int i = 0; i < nLen; i++)
	{
		if (_strText[i] == '\n')
		{
			sx = x; sy += maxH;
			continue;
		}
		xCharTexture* pCharTex = GetTextChar(_strText[i], color, size);
		
		glBindTexture(GL_TEXTURE_2D, pCharTex->m_texID);							//绑定到目标纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);												
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);						//特殊的像素算法
		//glDisable(GL_TEXTURE_2D);
		int w = pCharTex->m_Width;
		int h = pCharTex->m_Height;

		int ch_x = sx + pCharTex->m_delta_x;
		int ch_y = sy - h - pCharTex->m_delta_y;

		if (maxH < h) maxH = h;
		glBegin(GL_QUADS);													 // 定义一个或一组原始的顶点
		{
			glTexCoord2f(0.0f, 1.0f); glVertex3f(ch_x, ch_y, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(ch_x + w, ch_y, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(ch_x + w, ch_y + h, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(ch_x, ch_y + h, 1.0f);
		}
		glEnd();
		sx += pCharTex->m_adv_x;
		if (sx > x + maxW)
		{
			sx = x; sy += maxH + 12;
		}
	}
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}