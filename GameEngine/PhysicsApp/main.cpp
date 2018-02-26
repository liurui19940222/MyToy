#include"PhysXApp.h"

GLAppBase* CreateApp()
{
	return new PhysXApp(L"CLASSNAME", L"TestApp", 800, 600);
}