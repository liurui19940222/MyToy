#include "stdafx.h"
#include "UIImage.h"

USING_NAMESPACE_GUI;

UIImage::UIImage() : UIView()
{
}

void UIImage::CalcModelMatrix(Matrix4x4& baseMatrix)
{
	static Matrix4x4 translate, scale, rotate;
	translate.MakeTranslate(m_Rect.center + GetOffset());
	scale.MakeScale(m_LocalScale);
	rotate.MakeRotate(m_EulerAngles);
	m_ModelMatrix = baseMatrix * translate * rotate * scale;
	if (m_Type == Slice && m_SliceData)
	{
		for (int i = 0; i < 9; ++i)
		{
			translate.MakeTranslate(m_SliceData->m_RectList[i].center + GetOffset());
			m_SliceData->m_ModelMatrices[i] = baseMatrix * translate * rotate * scale;
		}
	}
}

const Matrix4x4& UIImage::GetGlobalModelMatrix() const
{
	if (m_Type == Slice && m_SliceData)
	{
		return m_ModelMatrix;
	}
	return UIView::GetGlobalModelMatrix();
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
	if (!sprite)
		return;
	if (setToRawSize)
		SetRawSize();
	if (m_Type == Slice && !sprite->m_Border.equaltozero())
	{
		BuildSliceData();
	}
}

void UIImage::SetRawSize()
{
	if (m_Sprite)
	{
		SetWidth((float)m_Sprite->m_Width);
		SetHeight((float)m_Sprite->m_Height);
	}
}

void UIImage::SetImageType(Type type)
{
	m_Type = type;
	if (type != Slice)
		return;

	BuildSliceData();
}

void UIImage::BuildSliceData()
{
	if (!m_Sprite)
		return;
	if (!m_SliceData)
	{
		m_SliceData = make_shared<SliceData>();
		m_SliceData->m_TexcoordRanges.resize(9);
		m_SliceData->m_Colors.resize(9);
		m_SliceData->m_RectList.resize(9);
		m_SliceData->m_ModelMatrices.resize(9);
	}
	for (int i = 0; i < 9; ++i)
	{
		m_SliceData->m_Colors[i] = GetColor();
	}
	m_Sprite->Slice(m_SliceData->m_TexcoordRanges);
	m_Rect.Slice(m_Sprite->m_Border, m_SliceData->m_RectList);
}

void UIImage::MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
	vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices)
{
	if (m_Type == Slice && m_SliceData)
	{
		texcoordRanges.insert(texcoordRanges.begin() + texcoordRanges.size(), m_SliceData->m_TexcoordRanges.begin(), m_SliceData->m_TexcoordRanges.end());
		colors.insert(colors.begin() + colors.size(), m_SliceData->m_Colors.begin(), m_SliceData->m_Colors.end());
		rectList.insert(rectList.begin() + rectList.size(), m_SliceData->m_RectList.begin(), m_SliceData->m_RectList.end());
		modelMatrices.insert(modelMatrices.begin() + modelMatrices.size(), m_SliceData->m_ModelMatrices.begin(), m_SliceData->m_ModelMatrices.end());
	}
	else
	{
		UIView::MakeData(texcoordRanges, colors, rectList, modelMatrices);
	}
}