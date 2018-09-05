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
	inline void SetMaterial(MaterialPtr mat) { m_Material = mat; }
	inline int GetMaterialId() { return m_Material ? m_Material->GetInstanceId() : 0; }
	inline void SetColor(const Color& color) { m_Color = color; SECURITY(m_Material)->SetColor(color); }
	inline Color GetColor() const { return m_Color; }

protected:
	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual TexcoordRange GetTexcoordRange();
	virtual uint GetTextureId();
	virtual TexturePtr GetTexture();
	virtual MaterialPtr GetMaterial() const;
	virtual const Matrix4x4& GetGlobalModelMatrix() const;
	virtual void MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
		vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices);

private:
	Color				m_Color;
	MaterialPtr			m_Material;
};

END_NAMESPACE_GUI