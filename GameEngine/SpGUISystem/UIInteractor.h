#pragma once

#include"GUIDefines.h"
#include"IInteractable.h"
#include"UIWidget.h"
#include<functional>
#include<map>

BEGIN_NAMESPACE_GUI

enum class EInteractorState
{
	Normal,
	Over,
	Press,
};

class UIInteractor : public UIWidget, public IInteractable
{
	GENERATE_UI_BODY()
public:
	UIInteractor();

	virtual void OnMouseEnter(const Vector2& mousePos) override;
	virtual void OnMouseExit(const Vector2& mousePos) override;
	virtual void OnMouseOver(const Vector2& mousePos) override;
	virtual void OnMouseDown(const Vector2& mousePos) override;
	virtual void OnMouseUp(const Vector2& mousePos) override;
	virtual bool Interact(const SMouseState& mouseState) override;
	virtual void LostFocus(const SMouseState& mouseState) override;
	virtual bool IsInteractive() const override;

	int AddMouseEnterListener(UIEvent _event);
	int AddMouseExitListener(UIEvent _event);
	int AddMouseOverListener(UIEvent _event);
	int AddMouseDownListener(UIEvent _event);
	int AddMouseUpListener(UIEvent _event);
	void RemoveMouseEnterListener(int id);
	void RemoveMouseExitListener(int id);
	void RemoveMouseOverListener(int id);
	void RemoveMouseDownListener(int id);
	void RemoveMouseUpListener(int id);

	inline UIWidget* SetInteract(bool b) { m_bInteractive = b; return this; }

private:
	static int					EventIdentity;
	bool						m_bInteractive;
	Vector2						m_LastMousePos;
	EInteractorState			m_InteractorState;
	map<int, UIEvent>			m_EnterEvents;
	map<int, UIEvent>			m_ExitEvents;
	map<int, UIEvent>			m_OverEvents;
	map<int, UIEvent>			m_DownEvents;
	map<int, UIEvent>			m_UpEvents;
};

END_NAMESPACE_GUI