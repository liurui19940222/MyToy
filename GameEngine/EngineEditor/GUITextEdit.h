#ifndef _TEXT_EDIT_H_
#define _TEXT_EDIT_H_

#include"GUIElement.h"
#include<GameEngine\MessageCenter.h>

enum class EEditValueMode
{
	Number,
	Word,
};

class CGUITextEdit : public CGUIElement, public IReceiver
{
private:
	float timer = 0.0f, flash_interval = 0.5f;
	bool show = true;
	int m_cursorPos;
	wstring m_value;
	CTextOneLineData* m_lineData;
	Vector2 m_offset;
	EEditValueMode m_valueMode;

	Vector2 GetCursorPos();
	bool CheckChar(wchar_t c);

public:
	CGUITextEdit();

	CGUITextEdit* SetEditValueMode(EEditValueMode mode);
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual void OnReceiveMsg(SMessage& message) override;
};

#endif