#pragma once

#include"GUIDefines.h"
#include"..\SpCommon\Math.h"
#include<functional>

BEGIN_NAMESPACE_GUI

enum class EKeyState
{
	Normal,
	Down,
	Up,
};

struct SMouseState
{
	Vector2			m_MousePosition;
	EKeyState		m_MouseKeyStates[3];
};

typedef std::function<void(const Vector2&)> UIEvent;

class IInteractable
{
public:
	virtual void OnMouseEnter(const Vector2& mousePos) = 0;
	virtual void OnMouseExit(const Vector2& mousePos) = 0;
	virtual void OnMouseOver(const Vector2& mousePos) = 0;
	virtual void OnMouseDown(const Vector2& mousePos) = 0;
	virtual void OnMouseUp(const Vector2& mousePos) = 0;
	virtual void LostFocus(const SMouseState& mouseState) = 0;
	virtual bool Interact(const SMouseState& mouseState) = 0;
	virtual bool IsInteractive() const = 0;
};

END_NAMESPACE_GUI