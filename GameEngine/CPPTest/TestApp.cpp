#include"TestApp.h"
#include"..\SpCommon\Input.h"
#include"..\SpCommon\EngineDefine.h"
#include"..\imgui\imgui.h"
#include"..\SpImGui\SpImGui.h"
#include"..\SpRendering\Texture2D.h"
#include"..\SpRendering\Shader.h"
#include"..\SpCommon\AssetUtility.h"
#include<iostream>

USING_NAMESPACE_ENGINE;

TestApp::TestApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void TestApp::InitGL(HWND hwnd)
{
	GLAppBase::InitGL(hwnd);

	spgameengine::ImGui_Init();
}

void TestApp::OnInitialize()
{
	GLAppBase::OnInitialize();
	Input::Init(GetModuleHandle(NULL), m_Hwnd);

	Texture2DPtr texture = Texture2D::Create("../Assets/textures/wooden_case.jpg", true);
	AssetUtility::Save(texture.get(), "D:/tex.json");

	Texture2DPtr tex = make_shared<Texture2D>();
	AssetUtility::Load(tex.get(), "D:/tex.json");

	ShaderPtr shader1 = Shader::Get("texture");
	AssetUtility::Save(shader1.get(), "D:/shader.json");

	ShaderPtr shader2 = make_shared<Shader>();
	AssetUtility::Load(shader2.get(), "D:/shader.json");

	cout << "" << endl;
}

void TestApp::OnUpdate(float deltaTime)
{
	Input::GetState(EngineToolkit::GetGlobalRect(m_Hwnd, m_WindowWidth, m_WindowHeight));
	m_Angle += deltaTime * 200;

	spgameengine::ImGui_UpdateInput();
}

void TestApp::OnRender()
{
	GLAppBase::OnRender();

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotatef(m_Angle, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
	glPopMatrix();

	static bool showDemoWindow = true, show_another_window = true;

	spgameengine::ImGui_BeginFrame();

	ImGui::ShowDemoWindow(&showDemoWindow);

	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	ImGui::Text("mouse pos x:%d y:%d", (int)Input::InputMousePosition(true).x, (int)Input::InputMousePosition(true).y);
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();

	spgameengine::ImGui_EndFrame();
}

void TestApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
}