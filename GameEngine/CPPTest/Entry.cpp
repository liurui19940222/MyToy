#include"TestApp.h"
#include"InstanceDraw.h"
#include"UITest.h"
#include"TextApp.h"
#include"ModelApp.h"
#include<iostream>
#include"SpCommon\Math.h"
//#include<physx_3.2\PxPhysicsAPI.h>

//#pragma comment(lib, "legacy_stdio_definitions.lib")

//using namespace physx;

GLAppBase* CreateApp()
{
	//PxDefaultAllocator* allocator = new PxDefaultAllocator();
	//PxDefaultErrorCallback* errorCallback = new PxDefaultErrorCallback();
	//PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, *errorCallback);
	return new TestApp(L"CLASSNAME", L"TestApp", 800, 600);
	//return new InstanceDraw(L"CLASSNAME", L"TestApp", 800, 600);
	//return new UITest(L"CLASSNAME", L"TestApp", 800, 600);
	//return new TextApp(L"CLASSNAME", L"TestApp", 800, 600);
	//return new ModelApp(L"CLASSNAME", L"TestApp", 800, 600);
}