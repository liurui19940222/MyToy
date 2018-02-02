#include "Sprite.h"

USING_NAMESPACE_ENGINE;

PSprite Sprite::CreateSprite(PTexture texture, TexcoordRange range)
{
	PSprite sprite = make_shared<Sprite>();
	sprite->m_Texture = texture;
	sprite->m_Range = range;
	return sprite;
}