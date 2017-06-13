#include"RelationshipTest.h"
#include"TextTest.h"
#include"ImageTest.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application->CreateApp(hInstance, hPrevInstance, lpCmdLine, nShowCmd, new CImageTest);

	return Application->GameLoop();
}