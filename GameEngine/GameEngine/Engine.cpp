#include"Engine.h"
#include"EditorTool.h"
#include"Application.h"
#include"Input.h"
#include"Time.h"
#include"Debug.h"
#include"GUISystem.h"
#include"GameObject.h"
#include"Config.h"
#include"EngineSetting.h"

using namespace guisystem;

void CEngine::InitEngine(HINSTANCE instance, HWND hwnd)
{
	glewInit();
	CDebug::Init(hwnd);
	CInput::Init(instance, hwnd);
	CTime::InitTime();
	CTime::SetTargetFrameCount(60);
	CEngineSetting::Init();
	m_cameras.SetComparator(CompareCamera);
	m_mainCamera = _Maker->Instantiate("MainCamera")->AddComponent<CCamera>();
	m_mainCamera->gameObject->SetLocalPosition(Vector3(0, 4, -10));
	m_mainCamera->Perspective(54.0f, (GLfloat)_Application->GetWindowWidth() / (GLfloat)_Application->GetWindowHeight(), 1.0f, 1000.0f);
	m_mainCamera->LayerMask() = Layer::Default;
	m_mainCamera->UpdateViewMatrix();

	m_uiCamera = _Maker->Instantiate("UICamera")->AddComponent<CCamera>();
	m_uiCamera->gameObject->SetLocalPosition(Vector3(0, 0, 10));
	m_uiCamera->gameObject->SetLocalEulerAngles(Vector3(0, 180, 0));
	m_uiCamera->SetCameraClearFlag(ECameraClearFlag::DontClear);
	m_uiCamera->SetDepth(99)->LayerMask() = Layer::Overlay2D;
	m_uiCamera->Ortho((GLfloat)_Application->GetWindowHeight() * 0.5f, (GLfloat)_Application->GetWindowWidth() / (GLfloat)_Application->GetWindowHeight());
	m_uiCamera->UpdateViewMatrix();
}

void CEngine::SetupProjection(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	_GUISystem->InitGUI((float)width, (float)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_cameras.Count() > 0)
	{
		m_cameras.Foreach([&width, &height](CCamera* camera) {
			if (camera->GetRenderTexture() == NULL)
			{
				if(camera->GetProjectionType() == EProjectionType::Perspective)
					camera->Perspective(camera->GetFov(), (float)width / (float)height, camera->GetNear(), camera->GetFar());
				else if(camera->GetProjectionType() == EProjectionType::Ortho)
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

void CEngine::BeginOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, CApplication::GetInstance()->GetWindowWidth(), 0, CApplication::GetInstance()->GetWindowHeight(), -1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CEngine::EndOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	SetupProjection(CApplication::GetInstance()->GetWindowWidth(), CApplication::GetInstance()->GetWindowHeight());
}

void CEngine::Update()
{
	CInput::GetState();
	_GUISystem->OnUpdate();
	CheckShortcuts();

	_Maker->ForeachGameObject([](CGameObject* go, int depth) {
		go->OnUpdate();
	});
}

void CEngine::Render()
{
	m_cameras.ForeachInverse([this](CCamera* camera) {
		camera->BeginOneFrame();
		IRenderer* renderer = NULL;
		_Maker->ForeachGameObject([this, &renderer, &camera](CGameObject* go, int depth, Matrix4x4& mat) {
			if ((renderer = go->GetRenderer()) != NULL && camera->LayerMask() & go->GetLayer())
			{
				renderer->Render(mat, camera->m_viewMat, camera->m_projectionMat);
				if (CEngineSetting::DrawGizmos) renderer->RenderDebug(mat);
			}
		});
		camera->EndTheFrame();
	});

	if (CEngineSetting::DrawGrid) CEditorTool::DrawGrid(_MainCameraGo->GetLocalPosition(), Vector3(0.0f, 0.0f, 0.0f), Color::grey);
}

void CEngine::Quit()
{
	_GUISystem->Quit();
	CInput::ShutDown();
	delete m_mainCamera;

	_Maker->ForeachGameObject([](CGameObject* go, int depth) {
		go->OnRelease();
	});
}

void CEngine::CheckShortcuts()
{
	if (CInput::GetKey(DIK_LALT) && CInput::GetKeyDown(DIKEYBOARD_RETURN))
	{
		_Application->ToggleFullOrWindow();
	}
}

CCamera* CEngine::GetCamera()
{
	return m_mainCamera;
}

void CEngine::AddCamera(CCamera* camera)
{
	m_cameras.Enqueue(camera);
}

void CEngine::RemoveCamera(CCamera* camera)
{
	m_cameras.Remove(camera);
}

int CompareCamera(CCamera* a, CCamera* b)
{
	if (a->GetDepth() > b->GetDepth())
		return 1;
	else if (a->GetDepth() < b->GetDepth())
		return -1;
	return 0;
}