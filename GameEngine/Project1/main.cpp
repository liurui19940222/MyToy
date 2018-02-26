#include<iostream>
#include<vector>
#include"ArrayList.h"
#include<physx_3.2\PxPhysicsAPI.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")

using namespace std;
using namespace my_collection;
using namespace physx;

int main()
{
	PxDefaultAllocator* allocator = new PxDefaultAllocator();
	PxDefaultErrorCallback* errorCallback = new PxDefaultErrorCallback();
	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, *errorCallback);
	system("pause");
	return 0;
}