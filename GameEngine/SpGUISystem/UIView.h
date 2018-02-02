#pragma once

#include"UIWidget.h"
#include"UIInteractor.h"
#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIView) class UIView : public UIInteractor
{
	GENERATE_UI_BODY()
public:
	UIView();
	inline void SetMaterial(PMaterial mat) { m_Material = mat; }
	inline int GetMaterialId() { return m_Material ? m_Material->GetInstanceId() : 0; }
	inline void SetColor(const Color& color) { m_Color = color; }
	inline Color GetColor() const { return m_Color; }

protected:
	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual TexcoordRange GetTexcoordRange();
	virtual uint GetTextureId();
	virtual PTexture GetTexture();
	virtual PMaterial GetMaterial() const;

private:
	Color				m_Color;
	PMaterial			m_Material;
};

END_NAMESPACE_GUI