#include "stdafx.h"
#include "UILabel.h"

USING_NAMESPACE_GUI;

UILabel::UILabel() : UIView(), FontMeshGenerator()
{
	SetPixelScale(1.0f);
	SetMaterial(GetDefaultMaterial());
}

uint UILabel::GetTextureId()
{
	if (!m_Primitives.empty()) return GetSprite()->m_Texture->GetTextureId();
	return UIView::GetTextureId();
}

PTexture UILabel::GetTexture()
{
	return m_Primitives.empty() ? UIView::GetTexture() : GetSprite()->m_Texture;
}