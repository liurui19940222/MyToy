#include "GLAppBase.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

GLAppBase* GLAppBase::AppInstance(NULL);

GLAppBase::GLAppBase(const wchar_t* className, const wchar_t* title, int width, int height)
	: m_ClassName(className), m_WindowName(title), m_WindowWidth(width), m_WindowHeight(height)
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
		lastFrameTime = GetTickCount();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		OnUpdate(deltaTime);
		OnRender();
		SwapBuffers(hdc);
		Sleep(16);
		deltaTime = (GetTickCount() - lastFrameTime) * 0.001f;
	}
	OnRelease();
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLAppBase::OnWindowSizeChanged(int width, int height)
{
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
		float height = HIWORD(lParam);
		float width = LOWORD(lParam);
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