#include"MainWindow.h"
#include"resource.h"
#include<GameEngine\Engine.h>
#include<GameEngine\MeshFactory.h>

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
			_Engine->MakeRenderContext();
			CMaterial* mat = _Maker->Instantiate<CMaterial>();
			mat->SetShader(CShader::Get("light"));
			mat->SetColor(Color::white);
			CGameObject* go = _Maker->Instantiate("NewGameObject");
			go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))->SetMaterial(mat);
			break;
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE)
			return 0;
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}