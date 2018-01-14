#ifndef _TRUE_TYPE_FONT_H_
#define _TRUE_TYPE_FONT_H_

#include<map>
#include<vector>
#include<ft2build.h>
#include FT_FREETYPE_H
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Atlas.h"

using namespace std;

//�����ַ���Ϣ
class CCharacterInfo
{
public:
	CAtlas* atlas;
	int code;
	Rect2D rect;
	int left_padding;
	int top;
	int advance_x;

	CCharacterInfo(int code);

	bool GetBitmap(SBitmapData* out_bitmap, Color color);
};

//ĳ���ֺŵ���������
//�ɰ�������ͼ����ͼ���Ĵ�СԤ����
class CTrueTypeFontSize
{
private:
	int fontSize;

	FT_Library* ft_lib;
	FT_Face* ft_face;

	map<int, CCharacterInfo*> characters;

	vector<CAtlas*> atlases;

	CAtlas* GetEnoughAtlas(int width, int height,int max_height);

public:
	CTrueTypeFontSize(int size, FT_Library* ft_lib, FT_Face* ft_face);

	CCharacterInfo* GetCharacter(int code);

	vector<CAtlas*>* GetAtlases();

	void Release();
};

//һ��������
//���ֺŰ������ֺŵ�ͼ������Ϣ
class CTrueTypeFont
{
private:
	int id;
	const char* name;
	FT_Library ft_lib;
	FT_Face ft_face;

	map<int, CTrueTypeFontSize*> sizeMap;

public:
	CTrueTypeFont(int id);

	CTrueTypeFont(int id, const char* file_name);

	bool LoadFromPath(const char* file_name);

	CCharacterInfo* GetCharacter(wchar_t ch, int size);

	vector<CAtlas*>* GetAtlases(int size);

	void Release();
};

#endif