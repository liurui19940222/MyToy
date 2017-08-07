#include"RelationshipTest.h"
#include"TextTest.h"
#include"ImageTest.h"
#include"GUITest.h"
#include"TestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application->CreateApp(hInstance, hPrevInstance, lpCmdLine, nShowCmd, new CTestApp);

	return Application->GameLoop();
}