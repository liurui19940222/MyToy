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