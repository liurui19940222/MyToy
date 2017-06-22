#include "GUITest.h"
#include "GUISystem.h"

using namespace guisystem;

void GUITest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	Engine->SetDrawDebug(true);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));

	go = Engine->CreateGameObject("cube");
	CGUIWidget* widget = go->AddComponent<CGUIWidget>();
	go->SetPosition(Vector3(400, 300, 0));
	widget->SetWidth(200)->SetHeight(40);
	widget->SetFillColor(Color(0, 1, 1, 0.5f))->SetFill(true);
}

void GUITest::OnUpdate()
{

}

void GUITest::OnRender()
{

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