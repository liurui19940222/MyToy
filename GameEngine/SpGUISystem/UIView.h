#pragma once

#include"UIWidget.h"
#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIView) class UIView : public UIWidget
{
	friend class GUISystem;
public:
	UIView();
	virtual ~UIView();

protected:
	virtual void OnUpdate() {}
	virtual void OnRender() {}

private:
	GUISystem*			m_System;
	PMaterial			m_Material;
	PMeshBuffer			m_Buffer;
};

END_NAMESPACE_GUI