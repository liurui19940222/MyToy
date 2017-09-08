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
	return WS_OVERLAPPEDWINDOW;
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

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case IDM_WORLD_CREATEEMPTY:
			CMaterial* mat = _Maker->Instantiate<CMaterial>();
			mat->SetShader(CShader::Get("texture"));
			mat->SetColor(Color(CMath::Random(), CMath::Random(), CMath::Random(), 1.0f));
			CGameObject* go = _Maker->Instantiate("NewGameObject");
			go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))->SetMaterial(mat);
			go->SetLocalPosition(Vector3(CMath::Random() * 10.0f * CMath::Random(-1.0f, 1.0f), CMath::Random() * 3, CMath::Random() * 10.0f * CMath::Random(-1.0f, 1.0f)));
			break;
		}
		break;
	case WM_MOVE:

		break;
	default:
		break;
	}

	return CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}