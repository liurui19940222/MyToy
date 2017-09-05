#include<Windows.h>
#include<GameEngine\Application.h>
#include<GameEngine\Math.h>
#include"EditorWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_Application->CreateApp(hInstance, hPrevInstance, lpCmdLine, nShowCmd, new CEditorWindow);
	return _Application->GameLoop();
}