#include"RelationshipTest.h"
#include"TextTest.h"
#include"ImageTest.h"
#include"TestApp.h"
#include"ColladaTest.h"
#include"TestTest.h"
#include"LightTest.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_Application->CreateApp(hInstance, hPrevInstance, lpCmdLine, nShowCmd, new CColladaTest);
	return _Application->GameLoop();
}