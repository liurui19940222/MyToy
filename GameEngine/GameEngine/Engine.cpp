#include"Engine.h"
#include"EditorTool.h"
#include"Application.h"
#include"Input.h"
#include"Time.h"
#include"Debug.h"

CEngine::CEngine() : clearColor(0, 0, 0, 0) { }

CEngine::~CEngine(){ }

void CEngine::InitEngine(HINSTANCE instance, HWND hwnd)
{
	CDebug::Init(hwnd);
	CInput::Init(instance, hwnd);
	CTime::InitTime();
	CTime::SetTargetFrameCount(60);
	m_camera = new CCamera;
	m_camera->SetPosition(Vector3(0, 4, -10));
}

void CEngine::SetupProjection(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

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

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

	map<int, CGameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		(*it++).second->OnUpdate();
	}
}

void CEngine::Render()
{
	m_camera->OnRender();
	if (drawGrid)
		CEditorTool::DrawGrid(MainCamera->GetPosition(), Vector3(0, 0, 0), Color(0, 0, 0.8, 1));

	map<int, CGameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		(*it++).second->OnRender();
	}
}

void CEngine::Quit()
{
	CInput::ShutDown();
	delete m_camera;

	map<int, CGameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		(*it++).second->OnRelease();
	}
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
	CGameObject* go = new CGameObject;
	m_gameObjects.insert(make_pair(go->GetInstanceId(), go));
	go->OnStart();
	return go;
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
	m_gameObjects.erase(go->GetInstanceId());
	go->OnRelease();
	delete go;
}

CEngine* CEngine::SetDrawGrid(bool drawGrid)
{
	this->drawGrid = drawGrid;
	return this;
}

CEngine* CEngine::SetClearColor(Color clearColor)
{
	this->clearColor = clearColor;
	return this;
}