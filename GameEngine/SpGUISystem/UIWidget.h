#pragma once
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Object.h"
#include"GUIDefines.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIWidget) class UIWidget : public Object
{
	GENERATE_UI_BODY()
public:
	UIWidget();
	virtual ~UIWidget();
	UIWidget* SetWidth(float width);
	UIWidget* SetHeight(float height);
	UIWidget* SetRect(const SRect2D& rect);
	UIWidget* SetLocalPosition(const Vector2& pos);
	UIWidget* AddChild(UIWidgetPtr widget);
	UIWidget* RemoveChild(UIWidgetPtr widget);
	UIWidget* SetAlignment(EAlignment alignment);
	bool Overlay(const Vector2& point);
	inline float GetWidth() const { return m_Width; }
	inline float GetHeight() const { return m_Height; }
	inline SRect2D GetRect() const { return m_Rect; }
	inline Vector2 GetLocalPosition() const { return m_Rect.center; }
	inline Vector2 GetHalfSize() const { return m_Rect.halfSize; }
	inline Vector2 GetSize() const { return m_Rect.halfSize * 2; }
	inline UIWidget* SetScale(const Vector2& scale) { m_LocalScale = scale; return this; }
	inline Vector2 GetScale() const { return m_LocalScale; }
	inline UIWidget* SetPivot(const Vector2& pivot) { m_Pivot = pivot; return this; }
	inline Vector3 GetEulerAngles() const { return m_EulerAngles; }
	inline UIWidget* SetEulerAngles(const Vector3& euler) { m_EulerAngles = euler; return this; }
	inline Vector3 ExtractWorldPosition() { return Vector3(m_ModelMatrix[3][0], 
		m_ModelMatrix[3][1], m_ModelMatrix[3][2]); }

protected:
	Vector2 GetOffset() const;
	virtual void CalcModelMatrix(Matrix4x4& baseMatrix);

	EAlignment				m_Alignment;
	Vector2					m_Pivot;
	Vector2					m_LocalScale;
	Vector3					m_EulerAngles;
	SRect2D					m_Rect;
	float					m_Width;
	float					m_Height;
	Matrix4x4				m_ModelMatrix;
	UIWidget*				m_Parent;
	UISystem*				m_System;
	vector<UIWidgetPtr>		m_Childreen;
};

END_NAMESPACE_GUI

