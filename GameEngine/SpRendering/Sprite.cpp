#include "Sprite.h"
#include <fstream>
#include <sstream>
#include "..\SpCommon\Converter.h"
#include "Texture2D.h"

using namespace rapidxml;

USING_NAMESPACE_ENGINE;

PSprite Sprite::CreateSprite(PTexture texture, TexcoordRange range, ushort width, ushort height)
{
	PSprite sprite = make_shared<Sprite>();
	sprite->m_Texture = texture;
	sprite->m_Range = range;
	sprite->m_Width = width;
	sprite->m_Height = height;
	return sprite;
}

SpriteSet::SpriteSet() {}

SpriteSet::SpriteSet(const char* tex_filename, const char* xml_filename)
{
	LoadAtlas(tex_filename, xml_filename);
}

PSprite SpriteSet::GetSprite(const string& name) const
{
	auto it = m_Sprites.find(name);
	if (it != m_Sprites.end())
	{
		return it->second;
	}
	return PSprite();
}

void SpriteSet::LoadAtlas(const char* tex_filename, const char* xml_filename)
{
	//加载贴图
	PTexture texture = Texture2D::Create(tex_filename);
	//加载xml
	ifstream is(xml_filename);
	stringstream ss;
	ss << is.rdbuf();
	is.close();
	size_t size = sizeof(char) * ss.str().size();
	char* xml = (char*)malloc(size + 1);
	ss.read(xml, size);
	xml[size] = '\0';
	xml_document<> xmlDoc;
	xmlDoc.parse<0>(xml);
	xml_node<>* root = xmlDoc.first_node();
	if (root == NULL)
		return;
	//读取宽高
	float atlas_width = -1, atlas_height = -1;
	ForeachAttribute(root, [&atlas_width, &atlas_height](string key, string value) {
		if (key == "width")
			atlas_width = CConverter::ToValue<float>(value);
		else if (key == "height")
			atlas_height = CConverter::ToValue<float>(value);
	});
	//读取每一个sprite，保存到map
	string n; float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
	for (xml_node<> *it = xmlDoc.first_node()->first_node();
		it; it = it->next_sibling())
	{
		ForeachAttribute(it, [&n, &x, &y, &w, &h](string key, string value) {
			if (key == "n")
				n = value;
			else if (key == "x")
				x = CConverter::ToValue<float>(value);
			else if (key == "y")
				y = CConverter::ToValue<float>(value);
			else if (key == "w")
				w = CConverter::ToValue<float>(value);
			else if (key == "h")
				h = CConverter::ToValue<float>(value);
		});
		PSprite sprite = make_shared<Sprite>();
		sprite->m_Texture = texture;
		sprite->m_Range.m_StartingPoint.x = x / atlas_width;
		sprite->m_Range.m_StartingPoint.y = (atlas_height - y - h) / atlas_height;
		sprite->m_Range.m_Size.x = w / atlas_width;
		sprite->m_Range.m_Size.y = h / atlas_height;
		sprite->m_Width = (ushort)w;
		sprite->m_Height = (ushort)h;
		m_Sprites.insert(make_pair(n, sprite));
	}
}

void SpriteSet::ForeachAttribute(xml_node<>* node, attr_call func)
{
	for (xml_attribute<> *attr = node->first_attribute();
		attr; attr = attr->next_attribute())
	{
		func((*attr).name(), (*attr).value());
	}
}