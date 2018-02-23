#pragma once

#include"GUIDefines.h"
#include"UIView.h"
#include"SpRendering\FontMeshGenerator.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UILabel) class UILabel : public UIView, public FontMeshGenerator
{
	GENERATE_UI_BODY()
public:
	UILabel();

	inline PSprite GetSprite() const { return m_Primitives.empty() ? PSprite() : m_Primitives[0]->m_Sprite; }
protected:
	virtual uint GetTextureId() override;
	virtual PTexture GetTexture() override;
};

END_NAMESPACE_GUI