#pragma once

#include"SpCommon\EngineDefine.h"
#include"SpCommon\Object.hpp"
#include"GUIDefines.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIWidget) class UIWidget : public Object
{
	friend class UISystem;

public:
	virtual ~UIWidget();

	UIWidget* SetWidth(float width);
	UIWidget* SetHeight(float height);
	UIWidget* SetRect(const SRect2D& rect);
	UIWidget* SetLocalPosition(const Vector2& pos);
	UIWidget* SetGlobalPosition(const Vector2& pos);
	UIWidget* AddChild(PUIWidget widget);
	UIWidget* RemoveChild(PUIWidget widget);

	bool Overlay(const Vector2& point) const;

	inline float GetWidth() const { return m_Width; }
	inline float GetHeight() const { return m_Height; }
	inline SRect2D GetRect() const { return m_Rect; }
	inline Vector2 GetLocalPosition() const { return m_Rect.center; }
	inline Vector2 GetGlobalPosition() const {
		if (m_Parent.get())
			return m_Parent->GetGlobalPosition() + m_Rect.center;
		return m_Rect.center;
	}
	inline Vector2 GetHalfSize() const { return m_Rect.halfSize; }
	inline Vector2 GetSize() const { return m_Rect.halfSize * 2; }

protected:
	SRect2D					m_Rect;
	float					m_Width;
	float					m_Height;
	PUIWidget				m_Parent;
	UISystem*				m_System;
	vector<PUIWidget>		m_Childreen;
};

END_NAMESPACE_GUI

