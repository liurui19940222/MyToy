#include"EditorWindow.h"
#include"resource.h"
#include<GameEngine\MeshFactory.h>

void CEditorWindow::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 6.8, 10));

	go = _Maker->Instantiate("Cube");
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetShader(CShader::Get("texture"));
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);

	_MainCameraGo->LookAt(go->GetLocalPosition());
}

void CEditorWindow::OnUpdate()
{
	CEditorTool::WatchTarget(*_MainCameraGo, go->GetLocalPosition());
}

void CEditorWindow::GetApplicationInfo(SApplicationInfo* info)
{
	info->windowWidth = 800;
	info->windowHeight = 600;
	info->isFullScreen = false;
	info->windowBits = 32;
	info->appName = L"GameEngineTest";
	info->className = L"MainClass";
	info->menuId = IDM_MENU_ROOT;
	info->wndProc = WndProc;
}

void HandleMenu(int menuId)
{
	switch (menuId)
	{
	case IDM_FILE_EXIT:
		PostQuitMessage(0);
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		HandleMenu(LOWORD(wParam));
		break;
	}
	return 0;
}