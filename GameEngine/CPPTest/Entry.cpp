#include"TestApp.h"
#include"InstanceDraw.h"
#include"UITest.h"
#include"TextApp.h"
#include<iostream>

GLAppBase* CreateApp()
{
	//return new TestApp(L"CLASSNAME", L"TestApp", 800, 600);
	//return new InstanceDraw(L"CLASSNAME", L"TestApp", 800, 600);
	return  new UITest(L"CLASSNAME", L"TestApp", 800, 600);
	//return new TextApp(L"CLASSNAME", L"TestApp", 800, 600);
}