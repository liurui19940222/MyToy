#include<assert.h>
#include"PhysXApp.h"
#include"SpRendering\MeshFactory.h"
#include"SpRendering\MeshBufferTexcoord.h"
#include"SpRendering\Texture2D.h"
#include"SpCommon\Input.h"

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
	Input::Init(GetModuleHandle(NULL), m_Hwnd);
	InitCamera();
	InitPhysX();
	InitObjects();
}

void PhysXApp::InitCamera()
{
	m_Camera = new RenderCamera();
	m_Camera->LookAt(Vector3(0, 5, 10), Vector3::zero, Vector3::up);
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
	CreateBox(Vector3(0, 10, 0));

	PRigidBodyGameObject plane = make_shared<RigidBodyGameObject>(Vector3::zero, Quaternion::Euler(Vector3(-90, 0, 0)), *m_PhysX, *m_PxScene, *m_PxMaterial, PxPlaneGeometry(), 0.0f, true);
	plane->SetMaterial(make_shared<Material>());
	plane->material().SetShader(Shader::Get("texture"));
	plane->SetMeshBuffer(_MeshFactory->CreateBuffer<MeshBufferTexcoord>(EMeshType::Quad));
	plane->SetScale(Vector3::one * 10);
	m_GameObjects.push_back(plane);
}

void PhysXApp::CreateBox(const Vector3& pos)
{
	PRigidBodyGameObject box = make_shared<RigidBodyGameObject>(Vector3(pos.x, pos.y, pos.z), Quaternion::identity, *m_PhysX, *m_PxScene, *m_PxMaterial, PxBoxGeometry(PxVec3(0.5f, 0.5f, 0.5f)), 1.0f, false);
	box->SetMaterial(make_shared<Material>());
	box->material().SetShader(Shader::Get("texture"));
	box->material().SetMainTexture(Texture2D::Create("../Assets/wooden_case.jpg"));
	box->SetMeshBuffer(_MeshFactory->CreateBuffer<MeshBufferTexcoord>(EMeshType::Cube));
	m_GameObjects.push_back(box);
}

void PhysXApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
	Input::GetState(EngineToolkit::GetGlobalRect(m_Hwnd, m_WindowWidth, m_WindowHeight));
	HandleInput();
	SimulatePhysics(deltaTime);

	for (PGameObject& go : m_GameObjects)
	{
		go->OnUpdate(deltaTime);
	}
}

void PhysXApp::SimulatePhysics(float deltaTime)
{
	static float accumulator = 0.0f;
	static float stepSize = 1.0f / 60.0f;
	accumulator += deltaTime;
	if (accumulator < stepSize)
		return;

	accumulator -= stepSize;
	m_PxScene->simulate(stepSize);
	m_PxScene->fetchResults(true);

	RigidBodyGameObject* rigid = NULL;
	for (PGameObject& go : m_GameObjects)
	{
		if (!IS_TYPE(RigidBodyGameObject, go.get()))
			continue;
		rigid = dynamic_cast<RigidBodyGameObject*>(go.get());
		if (rigid->isStatic())
			continue;
		PxTransform trans = PxShapeExt::getGlobalPose(rigid->GetShape(0));
		rigid->SetPosition(Vector3(trans.p.x, trans.p.y, trans.p.z));
		rigid->SetRotation(Quaternion(trans.q.x, trans.q.y, trans.q.z, trans.q.w));
	}
}

void PhysXApp::OnRender()
{
	GLAppBase::OnRender();

	for (PGameObject& go : m_GameObjects)
	{
		go->OnRender(*m_RI, m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix());
	}
}

void PhysXApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	m_Camera->Perspective(45, width / (float)height, 0.1, 1000);
}

void PhysXApp::OnRelease()
{
	m_PhysX->release();
	m_RI->ShutDown();
	delete m_Camera;
}

void PhysXApp::HandleInput()
{
	if (Input::GetKeyDown(DIK_A))
	{
		CreateBox(Vector3(0, 10, 0));
	}
}