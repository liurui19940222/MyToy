#include"PhysXApp.h"
#include<assert.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")

PhysXApp::PhysXApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void PhysXApp::OnInitialize()
{
	GLAppBase::OnInitialize();
	SetBackgroundColor(0.3f, 0.5f, 0.5f, 1.0f);

	//初始化physX SDK
	m_Allocator = new PxDefaultAllocator();
	m_ErrorCallback = new PxDefaultErrorCallback();
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());
	if (!PxInitExtensions(*m_PhysX))
	{
		fatalError("PxInitExtensions failed!");
	}
	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, m_PhysX->getFoundation(), PxCookingParams());
	if (!m_Cooking)
	{
		fatalError("PxCreateCooking failed!");
	}
	//初始化physX Scene
	PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	if (!sceneDesc.cpuDispatcher)//调度员不存在  
	{
		PxDefaultCpuDispatcher *cpu_dispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = cpu_dispatcher;
	}
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_PxScene = m_PhysX->createScene(sceneDesc);
	if (!m_PxScene)
	{
		fatalError("createScene failed!");
	}
	//初始化physX 对象
	m_PxMaterial = m_PhysX->createMaterial(0.5f, 0.5f, 0.1f);
}

void PhysXApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
}

void PhysXApp::OnRender()
{
	GLAppBase::OnRender();
}
