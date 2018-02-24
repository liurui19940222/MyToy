#include "stdafx.h"
#include "UIImage.h"

USING_NAMESPACE_GUI;

UIImage::UIImage() : UIView()
{
}

TexcoordRange UIImage::GetTexcoordRange()
{
	if (m_Sprite) return m_Sprite->m_Range;
	return UIView::GetTexcoordRange();
}

uint UIImage::GetTextureId()
{
	if (m_Sprite) return m_Sprite->m_Texture->GetTextureId();
	return UIView::GetTextureId();
}

PTexture UIImage::GetTexture()
{
	return m_Sprite ? m_Sprite->m_Texture : UIView::GetTexture();
}

void UIImage::SetSprite(PSprite sprite, bool setToRawSize) 
{
	m_Sprite = sprite; 
	if (setToRawSize)
		SetRawSize();
}

void UIImage::SetRawSize() 
{ 
	if (m_Sprite) 
	{ 
		SetWidth((float)m_Sprite->m_Width);
		SetHeight((float)m_Sprite->m_Height);
	}
}