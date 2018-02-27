#include "TextApp.h"
#include "SpRendering\MeshFactory.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

using namespace std;

TextApp::TextApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void TextApp::InitGL(HWND hwnd)
{
	GLRenderingStartUpParams params;
	params.m_HWND = hwnd;
	m_RI = new GLRendering();
	m_RI->StartUp(&params);
}

void TextApp::OnInitialize()
{
	GLAppBase::OnInitialize();
	Color color = Color::Hex(0x314D79FF);
	SetBackgroundColor(color.r, color.g, color.b, color.a);
	Input::Init(GetModuleHandle(NULL), m_Hwnd);

	m_Buffer = _MeshFactory->CreateBuffer<MeshBufferTexcoord>(EMeshType::Quad);
	m_Material = make_shared<Material>();
	m_Material->SetShader(Shader::Get("texture"));
	m_Material->SetMainTexture(Texture2D::Create("../Assets/shake.png", ETexWrapMode::Clamp, ETexFilterMode::MipmapLinear, ETexEnvMode::Replace, true));

	PTrueTypeFont f = _FontManager->LoadFont(1, FONT_PATH);
	font = new FontRenderer(m_RI);
	font->SetFont(f);
	font->SetColor(Color::white);
	font->SetEffect(EFontEffect::None);
	font->SetEffectVector(Vector3(1, -1, 0));
	font->SetEffectColor(Color::black);
	font->SetFontSize(font_size);
	font->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	//font->SetTextRect(SRect2D(0, 0, 4, 1));
	font->SetText(SHOW_TEXT);
	font->SetIntervalX(font_interval_x);
	font->SetIntervalY(font_interval_y);
	font->SetPixelScale(1.0f);
	//font->SetFont(f);
	//font->SetFontSize(font_size);
	//font->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	font->SetTextRect(SRect2D(0, 0, 400, 100));
	//font->SetText(SHOW_TEXT);
	//font->SetIntervalX(0);
	//font->SetIntervalY(0);
	//font->SetPixelScale(1.0f);
	font->BuildInstanceData(/*Matrix4x4::Translate(Vector3(0, -0, 0)) * Matrix4x4::Scale(Vector3::one * 0.01f)*/);

	viewMat = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);
	font->BuildInstanceData();
	m_Texture = (f->GetAtlases(font_size))[0]->m_Texture;
}

void TextApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
}

void TextApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
	m_Angle += deltaTime * 200;

	Input::GetState(RECT{});

	if (Input::GetKeyDown(DIK_NUMPAD1))
	{
		font->SetTextAlignment(EAlignment::LEFT_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD2))
	{
		font->SetTextAlignment(EAlignment::CENTER_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD3))
	{
		font->SetTextAlignment(EAlignment::RIGHT_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD4))
	{
		font->SetTextAlignment(EAlignment::LEFT_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD5))
	{
		font->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD6))
	{
		font->SetTextAlignment(EAlignment::RIGHT_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD7))
	{
		font->SetTextAlignment(EAlignment::LEFT_TOP);
	}
	if (Input::GetKeyDown(DIK_NUMPAD8))
	{
		font->SetTextAlignment(EAlignment::CENTER_TOP);
	}
	if (Input::GetKeyDown(DIK_NUMPAD9))
	{
		font->SetTextAlignment(EAlignment::RIGHT_TOP);
	}

	if (Input::GetKeyDown(DIK_1))
	{
		font->SetColor(Color::orange);
	}
	if (Input::GetKeyDown(DIK_2))
	{
		font->SetColor(Color::cyan);
	}
	if (Input::GetKeyDown(DIK_3))
	{
		font->SetColor(Color::green);
	}
	if (Input::GetKeyDown(DIK_4))
	{
		font->SetColor(Color::red);
	}

	float h = Input::GetAxis("Horizontal");
	float v = Input::GetAxis("Vertical");
	if (h != 0.0f)
	{
		font_interval_x += h * 0.01f;
		font->SetIntervalX(font_interval_x);
	}
	if (v != 0.0f)
	{
		font_interval_y += v * 0.05f;
		font->SetIntervalY(font_interval_y);
	}
}

void TextApp::OnRender()
{
	GLAppBase::OnRender();

	//DrawAtlas();

	//font->OnRender(modelMat, viewMat, projectionMat);

	m_Material->Bind();
	PTexture2D tex = Texture2D::Create("../Assets/fish.png");
	//m_Material->SetParam("AddTex", 1);
	m_Material->BindTexture("AddTex", tex->GetTextureId(), 1);
	m_Material->SetParam("M", Matrix4x4::identity);
	m_Material->SetParam("V", viewMat);
	m_Material->SetParam("P", projectionMat);
	m_RI->Render(RenderingObject{ m_Buffer.get(), m_Material.get() });
}

void TextApp::DrawAtlas()
{
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glDisable(GL_BLEND);
	glTranslatef(-2, 2, 0);
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_Texture->Bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(1, -1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();

	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}