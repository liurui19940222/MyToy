#include"TestApp.h"
#include"InstanceDraw.h"
#include"UITest.h"
#include"TextApp.h"
#include<iostream>
#include"SpCommon\Math.h"

GLAppBase* CreateApp()
{
	BVector4 b;
	int size = sizeof(b);
	bool b1 = b.equaltozero();
	b.x = 1;
	bool b2 = b.equaltozero();
	//return new TestApp(L"CLASSNAME", L"TestApp", 800, 600);
	//return new InstanceDraw(L"CLASSNAME", L"TestApp", 800, 600);
	return  new UITest(L"CLASSNAME", L"TestApp", 800, 600);
	//return new TextApp(L"CLASSNAME", L"TestApp", 800, 600);
}