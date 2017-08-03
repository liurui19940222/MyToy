#include"Engine.h"
#include"EditorTool.h"
#include"Application.h"
#include"Input.h"
#include"Time.h"
#include"Debug.h"
#include"GUISystem.h"
#include"GameObject.h"

using namespace guisystem;

CEngine::CEngine() { }

CEngine::~CEngine() { }

void CEngine::InitEngine(HINSTANCE instance, HWND hwnd)
{
	glewInit();
	CDebug::Init(hwnd);
	CInput::Init(instance, hwnd);
	CTime::InitTime();
	CTime::SetTargetFrameCount(60);
	m_cameras.SetComparator(CompareCamera);
	m_camera = CreateGameObject("MainCamera")->AddComponent<CCamera>();
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
	GUISystem->InitGUI(width, height);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

	ForeachGameObject([](CGameObject* go, int depth) {
		go->OnUpdate();
	});
}

void CEngine::Render()
{
	m_cameras.Foreach([this](CCamera* camera) {
		camera->BeginOneFrame();
		vector<CGameObject*>::iterator it = m_gameObjects.begin();

		ForeachGameObject([this](CGameObject* go, int depth) {
			go->OnRender();
			if (drawDebug) go->OnDrawDebug();
		});
		camera->EndTheFrame();
	});

	if (drawGrid) CEditorTool::DrawGrid(MainCameraGo->GetPosition(), Vector3(0, 0, 0), Color(0, 0, 0.8, 1));
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

	ForeachGameObject([](CGameObject* go, int depth) {
		go->OnRelease();
	});
}

CCamera* CEngine::GetCamera()
{
	return m_camera;
}

void CEngine::Destroy(Object* obj)
{
	m_objects.erase(obj->GetInstanceId());
	obj->OnRelease();
	delete(obj);
}

CBitImage* CEngine::CreateImage(char* filename)
{
	CBitImage* file = new CBitImage(filename);
	return file;
}

void CEngine::ReleaseImage(CBitImage* image)
{
	if (image)
	{
		image->Release();
		delete(image);
	}
}

CGameObject* CEngine::CreateGameObject()
{
	return CreateGameObject("NewGameObject");
}

CGameObject* CEngine::CreateGameObject(string name)
{
	CGameObject* go = new CGameObject(name);
	AddGameObject(go);
	go->OnStart();
	return go;
}

void CEngine::AddGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			return;
		}
		it++;
	}
	m_gameObjects.push_back(go);
}

void CEngine::RemoveGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			m_gameObjects.erase(it);
			return;
		}
		it++;
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

void CEngine::DestroyGameObject(CGameObject* go)
{
	CGameObject* temp = NULL;
	for (int i = go->childs.size() - 1; i >= 0; --i)
	{
		temp = go->childs[i];
		go->RemoveChild(go->childs[i]);
		DestroyGameObject(temp);
	}

	if (go->parent)
		go->parent->RemoveChild(go);
	RemoveGameObject(go);
	go->OnRelease();
	delete go;
}

void CEngine::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObjectR(go, callback, 0);
}

void CEngine::ForeachGameObjectR(ForeachGoCallbackR callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (!ForeachGameObjectR(*it++, callback, 0))
			break;
	}
}

void CEngine::ForeachGameObject(CGameObject* go, ForeachGoCallback callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0);
}

void CEngine::ForeachGameObject(ForeachGoCallback callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it++, callback, 0);
	}
}

bool CEngine::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth)
{
	if (!callback(go, depth)) return false;
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		if (!ForeachGameObjectR(*it, callback, depth + 1))
			return false;
	}
	return true;
}

void CEngine::ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth)
{
	callback(go, depth);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1);
	}
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