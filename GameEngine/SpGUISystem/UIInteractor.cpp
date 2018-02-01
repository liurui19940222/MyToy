#include "stdafx.h"
#include "UIInteractor.h"

USING_NAMESPACE_GUI;

#define CallDelegates(delegate_map) for (auto it = delegate_map.begin(); it != delegate_map.end(); ++it){\
	it->second(mousePos);}

#define AddListener(delegate_map) 	int id = ++EventIdentity;\
delegate_map.insert(make_pair(id, _event));\
return id;

#define RemoveListener(delegate_map, id) 	auto it = delegate_map.find(id);\
if (it != delegate_map.end())\
delegate_map.erase(it);

int	UIInteractor::EventIdentity(0);

UIInteractor::UIInteractor() : UIWidget(), m_bInteractive(false),
m_InteractorState(EInteractorState::Normal)
{
}

void UIInteractor::OnMouseEnter(const Vector2& mousePos)
{
	CallDelegates(m_EnterEvents);
}

void UIInteractor::OnMouseExit(const Vector2& mousePos)
{
	CallDelegates(m_ExitEvents);
}

void UIInteractor::OnMouseOver(const Vector2& mousePos)
{
	CallDelegates(m_OverEvents);
}

void UIInteractor::OnMouseDown(const Vector2& mousePos)
{
	CallDelegates(m_DownEvents);
}

void UIInteractor::OnMouseUp(const Vector2& mousePos)
{
	CallDelegates(m_UpEvents);
}

int UIInteractor::AddMouseEnterListener(UIEvent _event)
{
	AddListener(m_EnterEvents);
}

int UIInteractor::AddMouseExitListener(UIEvent _event)
{
	AddListener(m_ExitEvents);
}

int UIInteractor::AddMouseOverListener(UIEvent _event)
{
	AddListener(m_OverEvents);
}

int UIInteractor::AddMouseDownListener(UIEvent _event)
{
	AddListener(m_DownEvents);
}

int UIInteractor::AddMouseUpListener(UIEvent _event)
{
	AddListener(m_UpEvents);
}

void UIInteractor::RemoveMouseEnterListener(int id)
{
	RemoveListener(m_EnterEvents, id);
}

void UIInteractor::RemoveMouseExitListener(int id)
{
	RemoveListener(m_ExitEvents, id);
}

void UIInteractor::RemoveMouseOverListener(int id)
{
	RemoveListener(m_OverEvents, id);
}

void UIInteractor::RemoveMouseDownListener(int id)
{
	RemoveListener(m_DownEvents, id);
}

void UIInteractor::RemoveMouseUpListener(int id)
{
	RemoveListener(m_UpEvents, id);
}

bool UIInteractor::Interact(const SMouseState& mouseState)
{
	if (!Overlay(mouseState.m_MousePosition))
		return false;
	if (m_InteractorState == EInteractorState::Normal)
	{
		m_InteractorState = EInteractorState::Over;
		OnMouseEnter(mouseState.m_MousePosition);
	}
	else if (m_InteractorState == EInteractorState::Over)
	{
		if (mouseState.m_MouseKeyStates[0] == EKeyState::Down)
		{
			m_InteractorState = EInteractorState::Press;
			OnMouseDown(mouseState.m_MousePosition);
		}
		if (m_LastMousePos != mouseState.m_MousePosition)
			OnMouseOver(mouseState.m_MousePosition);
	}
	else if (m_InteractorState == EInteractorState::Press)
	{
		if (mouseState.m_MouseKeyStates[0] == EKeyState::Up)
		{
			m_InteractorState = EInteractorState::Over;
			OnMouseUp(mouseState.m_MousePosition);
		}
	}
	m_LastMousePos = mouseState.m_MousePosition;
	return true;
}

bool UIInteractor::IsInteractive() const
{
	return m_bInteractive;
}

void UIInteractor::LostFocus(const SMouseState& mouseState)
{
	if (m_InteractorState != EInteractorState::Normal)
	{
		OnMouseExit(mouseState.m_MousePosition);
	}
	m_InteractorState = EInteractorState::Normal;
}