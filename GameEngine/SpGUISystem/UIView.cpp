#include "UIView.h"
#include "UISystem.h"

USING_NAMESPACE_GUI;

UIView::UIView() : UIInteractor() , m_Color(Color::white) {}

TexcoordRange UIView::GetTexcoordRange()
{
	return TexcoordRange::full;
}

uint UIView::GetTextureId()
{
	return 0;
}

PTexture UIView::GetTexture()
{
	return m_System->GetSharedTexture();
}

PMaterial UIView::GetMaterial() const 
{ 
	return m_Material ? m_Material : m_System->GetSharedMaterial(); 
}

const Matrix4x4& UIView::GetGlobalModelMatrix() const
{
	return Matrix4x4::identity;
}

void UIView::MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
	vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices)
{
	texcoordRanges.push_back(GetTexcoordRange());
	colors.push_back(GetColor());
	rectList.push_back(m_Rect);
	modelMatrices.push_back(m_ModelMatrix);
}