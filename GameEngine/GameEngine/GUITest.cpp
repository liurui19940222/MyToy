#include "GUITest.h"
#include "GUISystem.h"

void GUITest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	Engine->SetDrawDebug(true);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));

	go = Engine->CreateGameObject("cube");
	widget = go->AddComponent<CGUIWidget>();
	go->SetPosition(Vector3(400, 300, 0));
	widget->SetWidth(200)->SetHeight(40);
	widget->SetFillColor(Color(0, 1, 1, 0.5f))->SetFill(true);
	widget->AddOnMouseDownListener([](Vector2 pos) {
		CDebug::Log(L"down");
	});
	widget->AddOnMouseUpListener([](Vector2 pos) {
		CDebug::Log(L"up");
	});
	widget->AddOnMouseEnterListener([](Vector2 pos) {
		CDebug::Log(L"enter");
	});
	widget->AddOnMouseExitListener([](Vector2 pos) {
		CDebug::Log(L"exit");
	});
	widget->AddOnMouseOverListener([](Vector2 pos) {
		CDebug::Log(L"over");
	});

	FontManager->LoadFont(1, "fonts/msyh.ttf");
	CGameObject* texGo = Engine->CreateGameObject("Text");
	texGo->SetPosition(Vector3(-4.75f, 4.05f, 0));
	text = texGo->AddComponent<CTextRenderer>();
	text->Init(FontManager->GetFont(1), NULL, 8, 0, 0.5f, Color::white(), EAlignment::LEFT_TOP, SRect2D(0, 0, 2, 1));
}

void GUITest::OnUpdate()
{
	Vector2 pos = CInput::InputMousePosition();
	wchar_t buffer[128];
	swprintf_s(buffer, L"MousePosition X:%g Y:%g\nWidgetState:%d", pos.x, pos.y, widget->GetState());
	text->SetText(buffer);

	//if (CInput::GetMouseDown(EMouseKey::Left))
	//{
	//	CDebug::Log(L"MouseDown");
	//}

	//if (CInput::GetMouseUp(EMouseKey::Left))
	//{
	//	CDebug::Log(L"MouseUp");
	//}
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