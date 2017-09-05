#include"EditorWindow.h"
#include"resource.h"

void CEditorWindow::GetApplicationInfo(SApplicationInfo* info)
{
	info->windowWidth = 800;
	info->windowHeight = 600;
	info->isFullScreen = false;
	info->windowBits = 32;
	info->appName = L"GameEngineTest";
	info->className = L"MainClass";
	info->menuId = IDM_MENU_ROOT;
}