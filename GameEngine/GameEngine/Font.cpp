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
	//����һ������,ȡĬ�ϵ�Face,һ��ΪRegualer
	if (FT_New_Face(library, font_file, 0, &m_FT_Face))
		return;
	//ѡ���ַ���
	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	m_w = _w; m_h = _h;
	m_FT_Face->num_fixed_sizes;
	//��СҪ��64.���ǹ涨�������Ϳ����ˡ�
	FT_Set_Char_Size(m_FT_Face, _w << 6, (int)(_w * 1.4) << 6, 96, 96);
	//�������ָ���ַ���Ⱥ͸߶ȵ��ض�����
	//FT_Set_Pixel_Sizes(m_FT_Face, m_w, m_h);
}

GLuint Font::LoadChar(wchar_t ch, Color32& color, int size)
{
	if (color == m_Colors[ch])
	{
		if (g_CharTexID[ch].m_texID)
			return g_CharTexID[ch].m_texID;
		/* װ������ͼ�����βۣ�����Ĩ����ǰ������ͼ�� */
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

	//�õ���ģ
	FT_Glyph glyph;
	//������ͼ������β۸��Ƶ��µ�FT_Glyph����glyph�С������������һ�������벢������glyph�� 
	if (FT_Get_Glyph(m_FT_Face->glyph, &glyph))
		return 0;

	//ת����λͼ
	FT_Render_Glyph(m_FT_Face->glyph, FT_RENDER_MODE_MONO);//FT_RENDER_MODE_NORMAL  ); 
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//ȡ��λͼ����
	FT_Bitmap &bitmap = bitmap_glyph->bitmap;

	//��λͼ���ݿ����Լ��������������.�����ɿ��Ի�����Ҫ�Ķ��������ˡ�
	int width = bitmap.width;
	int height = bitmap.rows;

	m_FT_Face->size->metrics.y_ppem;		//�������뵽�豸�ռ�
	m_FT_Face->glyph->metrics.horiAdvance;  //ˮƽ�ı�����


	charTex.m_Width = width;
	charTex.m_Height = height;
	charTex.m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;  //�������
	charTex.m_adv_y = m_FT_Face->size->metrics.y_ppem;		//m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	charTex.m_delta_x = (float)bitmap_glyph->left;			//left:����ԭ��(0,0)������λͼ��������ص�ˮƽ����.�����������ص���ʽ��ʾ�� 
	charTex.m_delta_y = (float)bitmap_glyph->top - height;	//Top: ���������β۵�bitmap_top�ֶΡ�
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);  //ָ��һ����ά������ͼƬ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);							   //glTexParameteri():�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);								//������л��

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
		
		glBindTexture(GL_TEXTURE_2D, pCharTex->m_texID);							//�󶨵�Ŀ������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);												
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);						//����������㷨
		//glDisable(GL_TEXTURE_2D);
		int w = pCharTex->m_Width;
		int h = pCharTex->m_Height;

		int ch_x = sx + pCharTex->m_delta_x;
		int ch_y = sy - h - pCharTex->m_delta_y;

		if (maxH < h) maxH = h;
		glBegin(GL_QUADS);													 // ����һ����һ��ԭʼ�Ķ���
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