#include"GUITextEdit.h"
#include"GUIManager.h"
#include<GameEngine\MessageDef.h>
#include<GameEngine\TaskManager.h>
#include<GameEngine\Time.h>
#include<GameEngine\Converter.h>
#include<GameEngine\Input.h>
#include<windowsx.h>

CGUITextEdit::CGUITextEdit() : CGUIElement(), m_cursorPos(0)
{
	SetFill(true)->SetCollide(true)->SetFillColor(Color::white);
}

void CGUITextEdit::OnUpdate()
{
	CGUIElement::OnUpdate();
	if (m_valueMode == EEditValueMode::Number && m_value.size() > 0 && m_haveFocus)
	{
		float s = CInput::GetAxis("Scroll") * CTime::deltaTime * 0.4f;
		if (s != 0.0f)
		{
			float value = CConverter::ToValue<float>(m_value);
			if (CInput::GetKey(DIK_LCONTROL))
				s *= 2;
			if (CInput::GetKey(DIK_LSHIFT))
				s *= 10;
			SetValueWithNumber(s + value);
			if (m_submitCallback) m_submitCallback(m_value);
		}
	}
}

void CGUITextEdit::OnRender()
{
	CRawRenderer* renderer = m_manager->GetRenderer();
	renderer->SetRenderMode(ERenderMode::Wireframe);
	CGUIElement::OnRender();
	renderer->SetRenderMode(ERenderMode::Fill);
	EnableClip(true);
	renderer->DrawString(m_value, m_rect * 0.95f, m_position - m_offset, Color::white, 13, EAlignment::LEFT_MIDDLE);
	EnableClip(false);
	m_lineData = renderer->GetLineData(0);

	if (m_haveFocus)
	{
		Vector2 pos = GetCursorPos();
		timer += CTime::deltaTime;
		if (show)
		{
			if (timer >= flash_interval)
			{
				timer = 0;
				show = false;
			}
			else renderer->DrawLine(Vector3(pos.x, pos.y + m_rect.half_size_y - 2, 0.0f), Vector3(pos.x, pos.y - m_rect.half_size_y + 2, 0.0f), Color::white, 1);
		}
		else
		{
			if (timer >= flash_interval)
			{
				timer = 0;
				show = true;
			}
		}
	}
}

void CGUITextEdit::OnGotFocus()
{
	CMessageCenter::Register(MSG_ON_INPUT_A_CHAR, this);
	CMessageCenter::Register(MSG_ON_KEYDOWN, this);
	SetFillColor(Color::green);
	m_cursorPos = m_value.size();
}

void CGUITextEdit::OnLostFocus()
{
	CMessageCenter::Unregister(MSG_ON_INPUT_A_CHAR, this);
	CMessageCenter::Unregister(MSG_ON_KEYDOWN, this);
	SetFillColor(Color::white);

	if (m_valueMode == EEditValueMode::Number)
	{
		if (m_value.size() == 0)
			m_value = L"0";
		else
			SetValueWithNumber(CConverter::ToValue<float>(m_value));
	}
	if (m_submitCallback) m_submitCallback(m_value);
}

void CGUITextEdit::OnReceiveMsg(SMessage& message)
{
	if (message.m_msgType == MSG_ON_INPUT_A_CHAR)
	{
		wchar_t code = (wchar_t)message.m_body;
		if (CheckChar(code))
		{
			m_value.insert(m_value.begin() + m_cursorPos, (wchar_t)message.m_body);
			m_cursorPos++;
		}
	}
	else if (message.m_msgType == MSG_ON_KEYDOWN)
	{
		switch ((int)message.m_body)
		{
		case VK_BACK:
			if (m_value.size() > 0)
			{
				if (m_cursorPos > 0)
				{
					m_value.erase(m_cursorPos - 1, 1);
					m_cursorPos--;
				}
			}
			break;
		case VK_DELETE:
			if (m_cursorPos < m_value.size())
			{
				m_value.erase(m_cursorPos, 1);
			}
			break;
		case VK_RETURN:
			CTaskManager::AddTask(STask([this](void* param) {
				m_haveFocus = false;
				OnLostFocus();
			}, NULL, 0));
			break;
		case VK_LEFT:
			m_cursorPos = CMath::Clamp(m_cursorPos - 1, 0, (int)m_value.size());
			show = true;
			timer = 0;
			break;
		case VK_RIGHT:
			m_cursorPos = CMath::Clamp(m_cursorPos + 1, 0, (int)m_value.size());
			show = true;
			timer = 0;
			break;
		}
	}
}

Vector2 CGUITextEdit::GetCursorPos()
{
	Vector2 leftPos(m_position.x - m_rect.half_size_x + 2, m_position.y);
	if (m_cursorPos > 0 && m_lineData && m_lineData->primitives.size() >= m_cursorPos)
	{
		float startPos = m_lineData->primitives[0]->position.x;
		leftPos.x += m_lineData->primitives[m_cursorPos - 1]->position.x - startPos + m_lineData->primitives[m_cursorPos - 1]->advance_x;
		if (leftPos.x > m_rect.center_x + m_rect.half_size_x)
		{
			m_offset.x = leftPos.x - (m_rect.center_x + m_rect.half_size_x);
			leftPos.x = m_rect.center_x + m_rect.half_size_x - 2;
		}
		else m_offset.x = 0;
	}
	return leftPos;
}

CGUITextEdit* CGUITextEdit::SetEditValueMode(EEditValueMode mode)
{
	m_valueMode = mode;
	return this;
}

CGUITextEdit* CGUITextEdit::SetOnSubmitListener(OnSubmit callback)
{
	m_submitCallback = callback;
	return this;
}

void CGUITextEdit::SetValue(const wstring& str)
{
	m_value = str;
	m_cursorPos = str.size();
}

void CGUITextEdit::SetValueWithNumber(float f)
{
	if (f - (int)f < CMath::EPSILON)
		m_value = CConverter::ToWString((int)f);
	else
		m_value = CConverter::ToWString(f);
}

wstring CGUITextEdit::GetValue()
{
	return m_value;
}

bool CGUITextEdit::CheckChar(wchar_t c)
{
	if (c == VK_BACK || c == VK_RETURN)
		return false;
	if (m_valueMode == EEditValueMode::Number)
	{
		if (c >= L'0' && c <= L'9') return true;
		if (c == L'-' && m_value.size() == 0) return true;
		if (c == L'.' && m_value.find(c) == -1) return true;
		return false;
	}
	return true;
}