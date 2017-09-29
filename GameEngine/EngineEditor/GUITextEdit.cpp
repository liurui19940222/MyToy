#include"GUITextEdit.h"
#include"GUIManager.h"
#include<GameEngine\MessageDef.h>
#include<GameEngine\TaskManager.h>
#include<windowsx.h>

CGUITextEdit::CGUITextEdit() : CGUIElement()
{
	SetFill(true)->SetCollide(true)->SetFillColor(Color::white);
}

void CGUITextEdit::OnUpdate()
{
	CGUIElement::OnUpdate();
}

void CGUITextEdit::OnRender()
{
	CRawRenderer* renderer = m_manager->GetRenderer();
	renderer->SetRenderMode(ERenderMode::Wireframe);
	CGUIElement::OnRender();
	renderer->SetRenderMode(ERenderMode::Fill);
	renderer->DrawString(m_value, m_rect, m_position, Color::white, 13, EAlignment::LEFT_MIDDLE);
}

void CGUITextEdit::OnGotFocus()
{
	CMessageCenter::Register(MSG_ON_INPUT_A_CHAR, this);
	CMessageCenter::Register(MSG_ON_KEYDOWN, this);
	SetFillColor(Color::purple);
}

void CGUITextEdit::OnLostFocus()
{
	CMessageCenter::Unregister(MSG_ON_INPUT_A_CHAR, this);
	CMessageCenter::Unregister(MSG_ON_KEYDOWN, this);
	SetFillColor(Color::white);
}

void CGUITextEdit::OnReceiveMsg(SMessage& message)
{
	if (message.m_msgType == MSG_ON_INPUT_A_CHAR)
	{
		wchar_t code = (wchar_t)message.m_body;
		if (code == VK_BACK || code == VK_RETURN) return;
		m_value += (wchar_t)message.m_body;
		CDebug::Log(m_value.c_str());
	}
	else if (message.m_msgType == MSG_ON_KEYDOWN)
	{
		switch ((int)message.m_body)
		{
		case VK_BACK:
			if (m_value.size() > 0)
			{
				m_value = m_value.substr(0, m_value.size() - 1);
				CDebug::Log(m_value.c_str());
			}
			break;
		case VK_RETURN:
			CTaskManager::AddTask(STask([this](void* param) {
				m_haveFocus = false;
				OnLostFocus();
			}, NULL, 0));
			break;
		}
	}
}
