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

const Matrix4x4& UILabel::GetGlobalModelMatrix() const
{
	return m_ModelMatrix;
}

void UILabel::MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
	vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices)
{
	BuildInstanceData();
	texcoordRanges.insert(texcoordRanges.begin() + texcoordRanges.size(), m_TexcoordRanges.begin(), m_TexcoordRanges.end());
	colors.insert(colors.begin() + colors.size(), m_Colors.begin(), m_Colors.end());
	rectList.insert(rectList.begin() + rectList.size(), m_RectList.begin(), m_RectList.end());
	modelMatrices.insert(modelMatrices.begin() + modelMatrices.size(), m_ModelMatrices.begin(), m_ModelMatrices.end());
}