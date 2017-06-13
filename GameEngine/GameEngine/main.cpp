#include"RelationshipTest.h"
#include"TextTest.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application->CreateApp(hInstance, hPrevInstance, lpCmdLine, nShowCmd, new CTextTest);

	return Application->GameLoop();
}