#include"Engine.h"
#include"EditorTool.h"
#include"Application.h"
#include"Input.h"
#include"Time.h"
#include"Debug.h"
#include"GUISystem.h"
#include"GameObject.h"

using namespace guisystem;

void CEngine::InitEngine(HINSTANCE instance, HWND hwnd)
{
	glewInit();
	CDebug::Init(hwnd);
	CInput::Init(instance, hwnd);
	CTime::InitTime();
	CTime::SetTargetFrameCount(60);
	m_cameras.SetComparator(CompareCamera);
	m_camera = Maker->Instantiate("MainCamera")->AddComponent<CCamera>();
	m_camera->gameObject->SetPosition(Vector3(0, 4, -10));
	m_camera->Perspective(54.0f, (GLfloat)Application->GetWindowWidth() / (GLfloat)Application->GetWindowHeight(), 1.0f, 1000.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void CEngine::SetupProjection(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	GUISystem->InitGUI((float)width, (float)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_camera != NULL)
	{
		m_camera->Perspective(54.0f, (float)width / (float)height, 1.0f, 1000.0f);
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
	GUISystem->OnUpdate();

	Maker->ForeachGameObject([](CGameObject* go, int depth) {
		go->OnUpdate();
	});
}

void CEngine::Render()
{
	m_cameras.Foreach([this](CCamera* camera) {
		camera->BeginOneFrame();
		Maker->ForeachGameObject([this](CGameObject* go, int depth) {
			go->OnRender();
			if (drawDebug) go->OnDrawDebug();
		});
		camera->EndTheFrame();
	});

	if (drawGrid) CEditorTool::DrawGrid(MainCameraGo->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.8f, 1.0f));
	//BeginOrtho();
	//GUISystem->OnRender();
	//if (drawDebug) GUISystem->OnDrawDebug();
	//EndOrtho();
}

void CEngine::Quit()
{
	GUISystem->Quit();
	CInput::ShutDown();
	delete m_camera;

	Maker->ForeachGameObject([](CGameObject* go, int depth) {
		go->OnRelease();
	});
}

CCamera* CEngine::GetCamera()
{
	return m_camera;
}

void CEngine::AddCamera(CCamera* camera)
{
	m_cameras.Enqueue(camera);
}

void CEngine::RemoveCamera(CCamera* camera)
{
	m_cameras.Remove(camera);
}

CEngine* CEngine::SetDrawGrid(bool drawGrid)
{
	this->drawGrid = drawGrid;
	return this;
}

CEngine* CEngine::SetDrawDebug(bool drawDebug)
{
	this->drawDebug = drawDebug;
	return this;
}

int CompareCamera(CCamera* a, CCamera* b)
{
	if (a->GetDepth() > b->GetDepth())
		return 1;
	else if (a->GetDepth() < b->GetDepth())
		return -1;
	return 0;
}