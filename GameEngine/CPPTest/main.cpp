#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include"SpRendering/GLRendering.h"
#include"SpRendering/Material.h"
#include"SpRendering/MeshBuffer.h"
#include"SpRendering/MeshFactory.h"
#include"SpRendering/Texture2D.h"
#include"SpRendering/FontRenderer.h"

const int width = 800;
const int height = 600;
bool isExiting = false;
GLRendering* rendering;
CFontRenderer* font;
HWND m_Hwnd;
RenderingObject obj;
Matrix4x4 textMat;
Matrix4x4 modelMat;
Matrix4x4 viewMat;
Matrix4x4 projectionMat;

const int canvas_w = 512;
const int canvas_h = 512;
uint pixels[canvas_w * canvas_h];
CTexture2D* canvasTexture;

#define FONT_PATH "D:/GitHub/MyToy/GameEngine/SpGameEngine/fonts/msyh.ttf"
#define SHOW_TEXT L"/* handle to face object2018北美车展：新款MINI家族正式发布"

#define CLASS_NAME L"NAME"

bool isCreated = false;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

void my_draw_bitmap(FT_Bitmap* bm, FT_Int left, FT_Int top)
{
	int x, y;
	for (int i = 0; i < bm->width; ++i)
	{
		for (int j = 0; j < bm->rows; ++j)
		{
			x = left + i;
			y = (top + j);
			if (x < 0 || x >= canvas_w || y < 0 || y >= canvas_h)
				continue;
			float grey = bm->buffer[j * bm->width + i];
			pixels[y * canvas_w + x] = _RGBA32((int)grey, (int)grey, (int)grey, 255);
		}
	}
}

void CreateText(const wchar_t* text)
{
	FT_Library  library;   /* handle to library     */
	FT_Face     face;      /* handle to face object */

	int error = FT_Init_FreeType(&library);
	if (error) {  }

	error = FT_New_Face(library,
		FONT_PATH,
		0,
		&face);
	if (error == FT_Err_Unknown_File_Format)
	{
		//... the font file could be opened and read, but it appears
		//	... that its font format is unsupported
	}
	else if (error)
	{
		// another error code means that the font file could not
		//	... be opened or read, or that it is broken...
	}
	error = FT_Set_Char_Size(
		face,    /* handle to face object           */
		0,       /* char_width in 1/64th of points  */
		10 * 64,   /* char_height in 1/64th of points */
		0,     /* horizontal device resolution    */
		128);   /* vertical device resolution      */

	FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
	int           pen_x, pen_y, n;
	pen_x = 0;
	pen_y = 30;

	for (n = 0; n < lstrlenW(text); n++)
	{
		FT_UInt  glyph_index;


		/* retrieve glyph index from character code */
		glyph_index = FT_Get_Char_Index(face, text[n]);

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue;  /* ignore errors */

					   /* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			continue;

		/* now, draw to our target surface */
		my_draw_bitmap(&slot->bitmap,
			pen_x + slot->bitmap_left,
			pen_y - slot->bitmap_top);

		/* increment pen position */
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6; /* not useful for now */
	}

	//memset(pixels, 255, sizeof(uint) * canvas_w * canvas_h);
	canvasTexture = CTexture2D::Create((UCHAR*)pixels, canvas_w, canvas_h);
	canvasTexture->SetWrapMode(ETexWrapMode::Clamp);
}

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

		CreateText(SHOW_TEXT);

		CTrueTypeFont* f = FontManager->LoadFont(1, FONT_PATH);
		font = new CFontRenderer();
		font->SetFont(f)->SetColor(Color::white)->SetFontSize(24)->SetTextAlignment(EAlignment::CENTER_MIDDLE)->SetRenderType(ERenderType::Smart)->SetTextRect(SRect2D(0,0,50, 50))->SetText(SHOW_TEXT);

		obj.material = new CMaterial();
		obj.material->SetShader(CShader::Get("texture"))->SetMainTexture(/*CTexture2D::Create("D://GitHub//MyToy//GameEngine//SpGameEngine//textures//shake.png")*/canvasTexture);
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

	modelMat = Matrix4x4::Rotate(0, 0, 180) * Matrix4x4::Scale(Vector3::one * 5);
	//modelMat = Matrix4x4::Rotate(0, GetTickCount() / 10, 0);
	textMat = Matrix4x4::Identity();

	rendering->Render(&obj, modelMat, viewMat, projectionMat);
	font->OnRender(textMat, viewMat, projectionMat);
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