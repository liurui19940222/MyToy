#pragma once

#include"SpCommon\EngineDefine.h"
#include"SpCommon\Object.hpp"

namespace spgameengine::guisystem
{
	SMART_CLASS(UIWidget) class UIWidget
	{
	public:
		UIWidget();
		~UIWidget();

		UIWidget* SetWidth(float width);
		UIWidget* SetHeight(float height);
		UIWidget* SetRect(const SRect2D& rect);
		UIWidget* SetLocalPosition(const Vector2& pos);
		UIWidget* SetGlobalPosition(const Vector2& pos);
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

	private:
		SRect2D					m_Rect;
		float					m_Width;
		float					m_Height;
		PUIWidget				m_Parent;
		vector<PUIWidget>		m_Childreen;
	};
}

