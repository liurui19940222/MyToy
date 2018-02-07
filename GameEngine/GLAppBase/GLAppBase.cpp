#include"GLAppBase.h"
#include<stdio.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

GLAppBase* GLAppBase::AppInstance(NULL);

GLAppBase::GLAppBase(const wchar_t* className, const wchar_t* title, int width, int height)
	: m_ClassName(className), m_WindowName(title), m_WindowWidth(width), m_WindowHeight(height), 
	m_BackgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
	
}

void GLAppBase::CreateMainWindow()
{
	HINSTANCE mHIntance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass;
	RECT       windowRect;
	HWND hwnd;

	windowRect.left = (long)0;
	windowRect.right = (long)m_WindowWidth;
	windowRect.top = (long)0;
	windowRect.bottom = (long)m_WindowHeight;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MessageHandle;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = mHIntance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = m_ClassName;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return;

	int x = (GetSystemMetrics(0) - m_WindowWidth) >> 1;
	int y = (GetSystemMetrics(1) - m_WindowHeight) >> 1;

	m_Hwnd = hwnd = CreateWindowEx(NULL,
		m_ClassName,
		m_WindowName,
		WS_OVERLAPPEDWINDOW,
		x, y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		mHIntance,
		NULL);

	if (!hwnd)
		return;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
}

void GLAppBase::InitGL(HWND hwnd)
{
	m_HDC = GetDC(hwnd);
	SetupPixelFormat(m_HDC);
	m_HRC = wglCreateContext(m_HDC);
	MakeRenderContext();
	glewExperimental = GL_TRUE;
	glewInit();
}

void GLAppBase::MakeRenderContext()
{
	wglMakeCurrent(m_HDC, m_HRC);
}

void GLAppBase::SetupPixelFormat(HDC hDC)
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

void GLAppBase::Run()
{
	MSG msg;
	HDC hdc = GetDC(m_Hwnd);
	bool isExiting = false;
	long lastFrameTime = GetTickCount();
	float deltaTime = 0.0f;
	while (!isExiting)
	{
		lastFrameTime = GetTickCount();				  //记录一帧开始的时间
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //处理Windows消息
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnUpdate(deltaTime);						  //更新所有逻辑	
		ComputeFPS();								  //计算FPS
		OnRender();									  //渲染
		DrawInfo();									  
		SwapBuffers(hdc);						      //交换缓冲区
		deltaTime = (GetTickCount() - lastFrameTime) * 0.001f;   //用当前时间-开始这一帧时的时间得到时间差，然后转成秒
		m_TimeSinceStarUp += deltaTime;				  
	}
	OnRelease();
}

void GLAppBase::SetBackgroundColor(float r, float g, float b, float a)
{
	m_BackgroundColor[0] = r;
	m_BackgroundColor[1] = g;
	m_BackgroundColor[2] = b;
	m_BackgroundColor[3] = a;
}

void GLAppBase::DrawString(const char* str)
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { 

		isFirstCall = 0;

		lists = glGenLists(MAX_CHAR);

		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void GLAppBase::BeginOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_WindowWidth, 0, m_WindowHeight, -1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
}

void GLAppBase::EndOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, m_WindowWidth / (float)m_WindowHeight, 0.01f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void GLAppBase::ComputeFPS()
{
	static float updateInterval = 0.5f;
	static float lastUpdateTime = 0.0f;
	static float frame = 0.0f;
	frame++;
	if (m_TimeSinceStarUp - lastUpdateTime > updateInterval)
	{
		m_FPS = frame / (m_TimeSinceStarUp - lastUpdateTime);
		lastUpdateTime = m_TimeSinceStarUp;
		frame = 0;
	}
}

void GLAppBase::DrawInfo()
{
	BeginOrtho();
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos2f(0.0f, m_WindowHeight - 10);
	char text[128];
	sprintf_s(text, "FPS:%d", (int)m_FPS);
	DrawString(text);
	EndOrtho();
}

void GLAppBase::OnInitialize() 
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

void GLAppBase::OnUpdate(float deltaTime) 
{

}

void GLAppBase::OnRender() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]);
}

void GLAppBase::OnWindowSizeChanged(int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, width / (float)height, 0.01f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GLAppBase::OnRelease() 
{
	
}

LRESULT CALLBACK GLAppBase::MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		AppInstance->InitGL(hWnd);
		AppInstance->OnInitialize();
	}
	break;
	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		int height = HIWORD(lParam);
		int width = LOWORD(lParam);
		glViewport(0, 0, width, height);
		AppInstance->OnWindowSizeChanged(width, height);
	}
	break;
	case WM_MOVE:

		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int main()
{
	GLAppBase* app = GLAppBase::AppInstance = CreateApp();
	app->CreateMainWindow();
	app->Run();
	return 0;
}