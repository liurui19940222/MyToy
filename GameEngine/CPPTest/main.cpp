#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include"..\SpCommon\Math.h"
#include"..\SpRendering\GLRendering.h"
#include"..\SpRendering\Material.h"
#include"..\SpRendering\MeshBuffer.h"
#include"..\SpRendering\MeshFactory.h"
#include"..\SpRendering\Texture2D.h"

const int width = 800;
const int height = 600;
bool isExiting = false;
GLRendering* rendering;
HWND m_Hwnd;
RenderingObject obj;
Matrix4x4 modelMat;
Matrix4x4 viewMat;
Matrix4x4 projectionMat;
#define CLASS_NAME L"NAME"

bool isCreated = false;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{	
		GLRenderingStartUpParams params;
		params.m_HWND = hWnd;
		params.m_ScreenWidth = width;
		params.m_ScreenHeight = height;
		rendering = new GLRendering();
		rendering->StartUp(&params);

		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//gluPerspective(45, width / (float)height, 0.1, 1000);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

		obj.material = new CMaterial();
		obj.material->SetShader(CShader::Get("texture"))->SetMainTexture(CTexture2D::Create("D:\\GitHub\\MyToy\\GameEngine\\SpGameEngine\\textures\\shake.png"));
		obj.mesh = _MeshFactory->SharedBuffer(EMeshType::Cube);

		isCreated = true;
	}
	break;
	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:

		break;
	case WM_MOVE:

		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CreateApp()
{
	HINSTANCE mHIntance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass;
	RECT       windowRect;
	HWND hwnd;

	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

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
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return;

	int x = (GetSystemMetrics(0) - width) >> 1;
	int y = (GetSystemMetrics(1) - height) >> 1;

	m_Hwnd = hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		L"TestApp",
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

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	modelMat = Matrix4x4::Rotate(0, GetTickCount() / 10, 0);

	rendering->Render(&obj, modelMat, viewMat, projectionMat);


	//glBegin(GL_TRIANGLES);
	//glVertex3f(0, 1, 0);
	//glVertex3f(-1, -1, 0);
	//glVertex3f(1, -1, 0);
	//glEnd();
}

int GameLoop()
{
	MSG msg;
	HDC hdc = GetDC(m_Hwnd);
	while (!isExiting)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (isCreated)
		{
			Render();
			SwapBuffers(hdc);
		}
	}

	return 0;
}

int main()
{
	CreateApp();



	modelMat = Matrix4x4::Identity();
	viewMat = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);



	GameLoop();
	return 0;
}