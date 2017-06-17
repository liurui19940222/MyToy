#include "GUITest.h"

void GUITest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 7, 10));
	MainCamera->SetEulerAngles(Vector3(30, 180, 0));

}

void GUITest::OnUpdate()
{

}

void GUITest::OnRender()
{
	Engine->BeginOrtho();
	
	CEditorTool::DrawQuad(Vector3(400, 300, 0), 400);

	Engine->EndOrtho();
}

void GUITest::OnClose()
{

}

void GUITest::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = 800;
		info->windowHeight = 600;
		info->windowBits = 32;
		info->isFullScreen = false;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}