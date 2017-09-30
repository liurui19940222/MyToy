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
	CMessageCenter::Register(MSG_ON_GAMEOBJECT_CREATED, this);
	CMessageCenter::Register(MSG_ON_GAMEOBJECT_DESTORYED, this);
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(5);

	m_tree.SetManager(&m_gui);

	m_contextMenu->Append(CMENU_ID_CREATE_EMPTY, L"Create Empty", [this](int id) {
		_Engine->MakeRenderContext();
		CGameObject* parent = NULL;
		if (m_tree.GetSelectedNode()) 
			parent = _Maker->FindGameObject(m_tree.GetSelectedNode()->value());
		CGameObject* go = _Maker->Instantiate();
		go->SetParent(parent);
		CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
	});

	m_contextMenu->Append(CMENU_ID_CREATE_CUBE, L"Create Cube", [this](int id) {
		_Engine->MakeRenderContext();
		CGameObject* parent = NULL;
		if (m_tree.GetSelectedNode())
			parent = _Maker->FindGameObject(m_tree.GetSelectedNode()->value());
		CGameObject* go = _Maker->CreateCube();
		go->SetParent(parent);
		CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
	});

	m_contextMenu->Append(CMENU_ID_CREATE_QUAD, L"Create Quad", [this](int id) {
		_Engine->MakeRenderContext();
		CGameObject* parent = NULL;
		if (m_tree.GetSelectedNode())
			parent = _Maker->FindGameObject(m_tree.GetSelectedNode()->value());
		CGameObject* go = _Maker->CreateQuad();
		go->SetParent(parent);
		CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
	});

	m_contextMenu->Append(CMENU_ID_GO_DESTORYED, L"Delete", [this](int id) {
		if (m_tree.GetSelectedNode())
		{
			CGameObject* go = NULL;
			go = _Maker->FindGameObject(m_tree.GetSelectedNode()->value());
			if (go)
			{
				int instanceId = go->GetInstanceId();
				_Maker->Destroy(go);
				CTaskManager::AddTask(STask([](void* param) {
					CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_DESTORYED, NULL, param });
				}, (void*)instanceId, 0));
			}
		}
	});
}

void CWorldTreeWindow::OnClose()
{
	CChannel::OnClose();
	CMessageCenter::Unregister(MSG_ON_ENGINE_INIT_COMPLETE, this);
	CMessageCenter::Unregister(MSG_ON_GAMEOBJECT_CREATED, this);
	CMessageCenter::Unregister(MSG_ON_GAMEOBJECT_DESTORYED, this);
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
		m_tree.Clear();
		auto func = std::bind(&CWorldTreeWindow::OnForeachGameObjects, this, std::placeholders::_1, std::placeholders::_2);
		_Maker->ForeachAllGameObject(func);
	}
	else if (message.m_msgType == MSG_ON_GAMEOBJECT_CREATED)
	{
		OnGameObjectCreated((CGameObject*)message.m_body);
	}
	else if (message.m_msgType == MSG_ON_GAMEOBJECT_DESTORYED)
	{
		OnGameObjectDestoryed((int)message.m_body);
	}
}

void CWorldTreeWindow::OnForeachGameObjects(CGameObject* go, int depth)
{
	OnGameObjectCreated(go);
}

void CWorldTreeWindow::OnGameObjectCreated(CGameObject* go)
{
	if (go->GetParent() == NULL)
	{
		CGUITree<int>* t1 = &m_tree;
		m_tree.AddNode(go->GetName(), go->GetInstanceId());
	}
	else
	{
		CGUITreeNode<int>* parent = m_tree.FindNode(go->GetParent()->GetInstanceId());
		if (parent != NULL)
		{
			parent->AddNode(go->GetName(), go->GetInstanceId());
		}
	}
}

void CWorldTreeWindow::OnGameObjectDestoryed(int instanceId)
{
	m_tree.DeleteNode(instanceId);
	m_gui.UpdateLayout();
}