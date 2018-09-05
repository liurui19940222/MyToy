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

	inline SpritePtr GetSprite() const { return m_Primitives.empty() ? SpritePtr() : m_Primitives[0]->m_Sprite; }
	virtual void MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
		vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices) override;
protected:
	virtual uint GetTextureId() override;
	virtual TexturePtr GetTexture() override;
	virtual const Matrix4x4& GetGlobalModelMatrix() const override;
};

END_NAMESPACE_GUI