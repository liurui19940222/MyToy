#include<assert.h>
#include"PhysXApp.h"
#include"SpRendering\MeshFactory.h"
#include"SpRendering\MeshBufferTexcoord.h"
#include"SpRendering\Texture2D.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")

PhysXApp::PhysXApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void PhysXApp::InitGL(HWND hwnd)
{
	GLRenderingStartUpParams params;
	params.m_HWND = hwnd;
	m_RI = new GLRendering();
	m_RI->StartUp(&params);
}

void PhysXApp::OnInitialize()
{
	GLAppBase::OnInitialize();
	SetBackgroundColor(0.3f, 0.5f, 0.5f, 1.0f);

	InitCamera();
	InitPhysX();
	InitObjects();
}

void PhysXApp::InitCamera()
{
	m_ViewMatrix = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);
}

void PhysXApp::InitPhysX()
{
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

void PhysXApp::InitObjects()
{
	PGameObject go1 = make_shared<RigidBodyGameObject>(*m_PhysX, *m_PxScene, *m_PxMaterial, ERigidBodyShape::Box, true);
	go1->SetMaterial(make_shared<Material>());
	go1->material()->SetShader(Shader::Get("texture"));
	go1->material()->SetMainTexture(Texture2D::Create("../Assets/wooden_case.jpg"));
	go1->SetMeshBuffer(_MeshFactory->CreateBuffer<MeshBufferTexcoord>(EMeshType::Cube));
	m_GameObjects.push_back(go1);
}

void PhysXApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);

	for (PGameObject& go : m_GameObjects)
	{
		go->OnUpdate(deltaTime);
	}
}

void PhysXApp::OnRender()
{
	GLAppBase::OnRender();

	for (PGameObject& go : m_GameObjects)
	{
		go->OnRender(*m_RI, m_ViewMatrix, m_ProjMatrix);
	}
}

void PhysXApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	m_ProjMatrix = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
}