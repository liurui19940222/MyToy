#include"Engine.h"
#include"EditorTool.h"
#include"Application.h"
#include"Time.h"
#include"SpCommon\Debug.h"
#include"GUISystem.h"
#include"GameObject.h"
#include"Config.h"
#include"EngineSetting.h"
#include"LightComponent.h"
#include"..\SpCommon\Input.h"

using namespace guisystem;

void CEngine::InitEngine(HINSTANCE instance, HWND hwnd, float clientWidth, float clientHeight)
{
	glewExperimental = GL_TRUE;
	glewInit();
	Debug::Init(hwnd);
	CInput::Init(instance, hwnd);
	CTime::InitTime();
	CTime::SetTargetFrameCount(60);
	CEngineSetting::Init();
	Light::SetShadowMapSize(1024, 1024);
	m_hInstance = instance;
	WindowHandle = hwnd;

	m_clientWidth = clientWidth;
	m_clientHeight = clientHeight;
	m_cameras.SetComparator(CompareCamera);
	CCamera* mainCamera = _Maker->Instantiate(L"MainCamera")->AddComponent<CCamera>();
	mainCamera->gameObject->SetTag("MainCamera");
	mainCamera->gameObject->SetLocalPosition(Vector3(0, 4, -10));
	mainCamera->Perspective(60.0f, clientWidth / clientHeight, 1.0f, 1000.0f);
	mainCamera->LayerMask() = Layer::Default;
	mainCamera->UpdateViewMatrix();

	CCamera* uiCamera = _Maker->Instantiate(L"UICamera")->AddComponent<CCamera>();
	uiCamera->gameObject->SetTag("UICamera");
	uiCamera->gameObject->SetLocalPosition(Vector3(0, 0, 10));
	uiCamera->gameObject->SetLocalEulerAngles(Vector3(0, 180, 0));
	uiCamera->SetCameraClearFlag(ECameraClearFlag::DontClear);
	uiCamera->SetDepth(99);
	uiCamera->LayerMask() = Layer::Overlay2D;
	uiCamera->Ortho(clientHeight * 0.5f, clientWidth / clientHeight);
	uiCamera->UpdateViewMatrix();

	_Maker->CreateDirectionalLight();
}

void CEngine::SetupProjection(int width, int height)
{
	if (height == 0)
		height = 1;

	m_clientWidth = (float)width;
	m_clientHeight = (float)height;
	_GUISystem->InitGUI((float)width, (float)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_cameras.Count() > 0)
	{
		m_cameras.Foreach([&width, &height](CCamera* camera) {
			if (camera->GetRenderTexture() == NULL)
			{
				if (camera->GetProjectionType() == EProjectionType::Perspective)
					camera->Perspective(camera->GetFov(), (float)width / (float)height, camera->GetNear(), camera->GetFar());
				else if (camera->GetProjectionType() == EProjectionType::Ortho)
					camera->Ortho((float)height * 0.5f, (float)width / (float)height);
			}
		});
	}
}

void CEngine::SetupPixelFormat(HDC hDC)
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

void CEngine::SetupRenderContext(HWND hwnd)
{
	m_hdc = GetDC(hwnd);
	_Engine->SetupPixelFormat(m_hdc);
	m_hrc = wglCreateContext(m_hdc);
	MakeRenderContext();
}

void CEngine::MakeRenderContext()
{
	wglMakeCurrent(m_hdc, m_hrc);
}

void CEngine::BeginOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, CApplication::GetInstance()->GetWindowWidth(), 0, CApplication::GetInstance()->GetWindowHeight(), -1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void CEngine::EndOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	SetupProjection(CApplication::GetInstance()->GetWindowWidth(), CApplication::GetInstance()->GetWindowHeight());
	glEnable(GL_DEPTH_TEST);
}

void CEngine::Update()
{
	CInput::GetState(ClientRect);
	_GUISystem->OnUpdate();
	CheckShortcuts();

	_Maker->ForeachGameObject([](CGameObject* go, int depth) {
		go->OnUpdate();
	});

	CTaskManager::OnUpdate();
}

void CEngine::Render()
{
	if (m_cameras.Count() < 1)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	PLight light = Light::PrepareLight();
	LightComponent::RenderShadowMap((LightComponent*)light.get());

	m_cameras.ForeachInverse([this](CCamera* camera) {
		camera->BeginOneFrame();
		IRenderer* renderer = NULL;
		_Maker->ForeachGameObject([this, &renderer, &camera](CGameObject* go, int depth, Matrix4x4& mat) {
			if (go->IsActive() && (renderer = go->GetRenderer()) != NULL && camera->LayerMask() & go->GetLayer())
			{
				renderer->Render(mat, camera->m_viewMat, camera->m_projectionMat);
				if (CEngineSetting::DrawGizmos) renderer->RenderDebug(mat);
			}
		});
		camera->EndTheFrame();
	});

	if (_MainCameraGo)
		if (CEngineSetting::DrawGrid) CEditorTool::DrawGrid(_MainCameraGo->GetLocalPosition(), Vector3(0.0f, 0.0f, 0.0f), Color::grey);
}

void CEngine::Quit()
{
	_GUISystem->Quit();
	CInput::ShutDown();
	_Maker->ForeachGameObject([](CGameObject* go, int depth) {
		//go->OnRelease();
	});
	wglMakeCurrent(m_hdc, NULL);
	wglDeleteContext(m_hrc);
}

void CEngine::CheckShortcuts()
{
	if (CInput::GetKey(DIK_LALT) && CInput::GetKeyDown(DIKEYBOARD_RETURN))
	{
		_Application->ToggleFullOrWindow();
	}
}

void CEngine::AddCamera(CCamera* camera)
{
	m_cameras.Enqueue(camera);
}

void CEngine::RemoveCamera(CCamera* camera)
{
	m_cameras.Remove(camera);
}

void CEngine::UpdateClientRect()
{
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(m_hwnd, &p);
	m_clientRect.left = p.x;
	m_clientRect.right = p.x + m_clientWidth;
	m_clientRect.top = p.y;
	m_clientRect.bottom = p.y + m_clientHeight;
}

int CompareCamera(CCamera* a, CCamera* b)
{
	if (a->GetDepth() > b->GetDepth())
		return 1;
	else if (a->GetDepth() < b->GetDepth())
		return -1;
	return 0;
}