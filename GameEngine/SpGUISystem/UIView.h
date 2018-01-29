#pragma once

#include"UIWidget.h"
#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIView) class UIView : public UIWidget
{
	friend class UISystem;
public:

	inline void SetMaterial(PMaterial mat) { m_Material = mat; }
	inline int GetMaterialId() { return m_Material ? m_Material->GetInstanceId() : 0; }
	inline PMaterial GetMaterial() const { return m_Material; }

protected:
	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual TexcoordRange GetTexcoordRange() 
	{
		static TexcoordRange range{ Vector2::zero, Vector2::one };
		return range;
	}

private:
	PMaterial			m_Material;
};

END_NAMESPACE_GUI