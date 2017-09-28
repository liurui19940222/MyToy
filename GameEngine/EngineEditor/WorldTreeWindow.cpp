#include"WorldTreeWindow.h"
#include"GUIMesh.h"
#include<GameEngine\Converter.h>
#include<GameEngine\Input.h>

CWorldTreeWindow::CWorldTreeWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	m_caption = L"WorldTree";
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CWorldTreeWindow::GetType()
{
	return EWindowType::WorldTree;
}

DWORD CWorldTreeWindow::GetStyle()
{
	return WS_CHILD;
}

LRESULT CALLBACK CWorldTreeWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChannel::WindowProc(hWnd, uMsg, wParam, lParam);
	static HDC hdc;
	static int height, width;
	static PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(m_hwnd, &ps);
		EndPaint(m_hwnd, &ps);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CGUIElement* CWorldTreeWindow::CreateElement(const Color& color)
{
	CGUIElement* ui = m_gui.Create<CGUIElement>();
	ui->SetFill(true)->SetFillColor(color)->SetCollide(true);
	return ui;
}

CGUIGroup* CWorldTreeWindow::CreateGroup(const wstring& name)
{
	CGUIGroup* group3 = m_gui.Create<CGUIGroup>();
	group3->SetText(name);
	return group3;
}

void CWorldTreeWindow::OnCreate()
{
	CChannel::OnCreate();
	CMessageCenter::Register(MSG_ON_ENGINE_INIT_COMPLETE, this);
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(20);

	m_tree.SetManager(&m_gui);
	CGUITreeNode<int>* root = m_tree.AddNode(L"MainCamera", -1);
	for (int i = 0; i < 2; ++i)
	{
		m_tree[0].AddNode(CConverter::FormatWString(L"node%d", i), i);
		m_tree[0][i].AddNode(CConverter::FormatWString(L"child node 0 %d"), i);
	}
}

void CWorldTreeWindow::OnClose()
{
	CChannel::OnClose();
	CMessageCenter::Unregister(MSG_ON_ENGINE_INIT_COMPLETE, this);
}

void CWorldTreeWindow::OnDraw()
{
	CChannel::OnDraw();
}

void CWorldTreeWindow::OnReceiveMsg(SMessage& message)
{
	CChannel::OnReceiveMsg(message);
	if (message.m_msgType == MSG_ON_ENGINE_INIT_COMPLETE)
	{

	}
	else if (message.m_msgType == MSG_ON_GAMEOBJECT_CREATED)
	{

	}
	else if (message.m_msgType == MSG_ON_GAMEOBJECT_DESTORYED)
	{

	}
}