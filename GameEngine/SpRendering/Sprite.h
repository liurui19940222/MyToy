#pragma once

#include"Texture.h"
#include<map>
#include<functional>
#include <ExternInclude\rapidxml\rapidxml.hpp>

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(Sprite) class Sprite
{
public:
	TexturePtr			m_Texture;
	TexcoordRange		m_Range;
	ushort				m_Width;
	ushort				m_Height;
	BVector4			m_Border;
	static SpritePtr CreateSprite(TexturePtr texture, TexcoordRange range, ushort width, ushort height);

	inline void SetBorder(BVector4 border) { m_Border = border; }
	inline void Slice(vector<TexcoordRange>& out) { m_Range.Slice(m_Border, out, m_Width, m_Height, m_Texture->GetWidth(), m_Texture->GetHeight()); }
};

SMART_CLASS(SpriteSet) class SpriteSet
{
	typedef function<void(string key, string value)> attr_call;
public:
	SpriteSet();
	SpriteSet(const char* tex_filename, const char* xml_filename);
	void LoadAtlas(const char* tex_filename, const char* xml_filename);
	SpritePtr GetSprite(const string& name) const;

private:
	void ForeachAttribute(rapidxml::xml_node<>* node, attr_call func);

	map<string, SpritePtr>	m_Sprites;
};

END_NAMESPACE_ENGINE