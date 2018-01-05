#include"MainWindow.h"
#include"resource.h"
#include<SpGameEngine\Engine.h>
#include<SpGameEngine\MeshFactory.h>

CMainWindow::CMainWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CWindow()
{
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CMainWindow::GetType()
{
	return EWindowType::Main;
}

DWORD CMainWindow::GetStyle()
{
	return WS_DLGFRAME | WS_SYSMENU;
}

HMENU CMainWindow::GetMenu()
{
	return LoadMenu(NULL, MAKEINTRESOURCE(IDM_MENU_ROOT));
}

wchar_t* CMainWindow::GetTitle()
{
	return L"Editor";
}

LRESULT CALLBACK CMainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
	static HGLRC hRC;
	static HDC hDC;
	static POINT p{ 0, 0 };
	static RECT rect;
	static int height, width;

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		_Editor->UpdateSize();
		break;
	case WM_MOVE:
		_Editor->UpdatePosition();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case IDM_WORLD_CREATEEMPTY:
			{
				_Engine->MakeRenderContext();
				CGameObject* go = _Maker->Instantiate();
				CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
			}
			break;
		case IDM_WORLD_CREATECUBE:
			{
				_Engine->MakeRenderContext();
				CGameObject* go = _Maker->CreateCube();
				CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
			}
			break;
		case IDM_WORLD_CREATEQUAD:
			{
				_Engine->MakeRenderContext();
				CGameObject* go = _Maker->CreateQuad();
				CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
			}
			break;
		case IDM_WORLD_CREATE_DIR_LIGHT:
			{
				_Engine->MakeRenderContext();
				CGameObject* go = _Maker->CreateDirectionalLight();
				CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
			}
			break;
		case IDM_WORLD_CREATE_POINT_LIGHT:
			{
				_Engine->MakeRenderContext();
				CGameObject* go = _Maker->CreatePointLight();
				CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_CREATED, this, go });
			}
			break;
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE)
			return 0;
		return 0;
	case WM_KEYDOWN:
		CMessageCenter::Send(SMessage{ MSG_ON_KEYDOWN, this, (void*)wParam });
		break;
	case WM_IME_COMPOSITION:
	case WM_SYSCHAR:
	case WM_IME_CHAR:
		return 0;
	case WM_CHAR:
		CMessageCenter::Send(SMessage{ MSG_ON_INPUT_A_CHAR, this, (void*)wParam });
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}