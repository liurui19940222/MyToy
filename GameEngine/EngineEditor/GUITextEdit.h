#ifndef _TEXT_EDIT_H_
#define _TEXT_EDIT_H_

#include"GUIElement.h"
#include<GameEngine\MessageCenter.h>

class CGUITextEdit : public CGUIElement, public IReceiver
{
private:
	wstring m_value;

public:
	CGUITextEdit();

	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual void OnReceiveMsg(SMessage& message) override;
};

#endif