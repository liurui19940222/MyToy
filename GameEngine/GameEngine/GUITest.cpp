#include "GUITest.h"
#include "GUISystem.h"

void Down1(Vector2 pos) { CDebug::Log(L"down1"); }

void Down2(Vector2 pos) { CDebug::Log(L"down2"); }

void GUITest::OnStart()
{
	Engine->SetClearColor(Color::Hex(0x18253BFF));
	Engine->SetDrawGrid(false);
	Engine->SetDrawDebug(true);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetLocalEulerAngles(Vector3(0, 180, 0));

	CTexture* tex = CTexture::Create("F://monthad_1.png");
	CTexture* tex2 = CTexture::Create("F://avatar_15.png");
	go = Engine->CreateGameObject("widget");
	widget = go->AddComponent<CGUIImage>();
	//widget->SetWidth(400)->SetHeight(300);
	widget->SetTexture(tex);
	//widget->SetFill(true)->SetFillColor(Color::blue());
	widget->SetAlignment(EAlignment::CENTER_MIDDLE);
	widget->SetPivot(Vector2(0.5f, 0.5f));
	widget->SetCollide(true);

	CGameObject* go3 = Engine->CreateGameObject();
	go3->AddComponent<CGUIImage>()->SetTexture(tex2)->SetLayer(2)->SetCollide(true);

	go2 = Engine->CreateGameObject("widget2");
	go2->SetParent(go);
	widget2 = go2->AddComponent<CGUILable>();
	widget2->SetWidth(300)->SetHeight(300);
	//widget2->SetFillColor(Color(1, 0, 0, 0.1f))->SetFill(true);
	widget2->SetLayer(3);
	widget2->SetAlignment(EAlignment::CENTER_MIDDLE);
	widget2->SetPivot(Vector2(0.5f, 0.5f));
	widget2->SetCollide(false);
	CTrueTypeFont* font = FontManager->LoadFont(1, "fonts/msyh.ttf");
	widget2->SetColor(Color::red())->SetFont(font)->SetFontSize(5)->SetIntervalY(25)->SetText(L"Sams Publishing & Pearson Education Inc., \nCD-ROM and software copyright (C) 2003 Sams Publishing & Pearson");




	//FontManager->LoadFont(2, "fonts/msyh.ttf");
	//CGameObject* texGo = Engine->CreateGameObject("Text");
	//texGo->SetPosition(Vector3(-4.75f, 4.05f, 0));
	//text = texGo->AddComponent<CTextRenderer>();
	//text->Init(FontManager->GetFont(2), NULL, 8, 0, 0.5f, Color::green(), EAlignment::LEFT_TOP, SRect2D(0, 0, 2, 1));
}

void GUITest::OnUpdate()
{
	Vector2 pos = CInput::InputMousePosition();
	wchar_t buffer[128];
	swprintf_s(buffer, L"MousePosition X:%g Y:%g\nWidgetState:%d\nAnchor X:%g Y:%g\nPivot X:%g Y:%g", 
		pos.x, pos.y, widget2->GetState(), widget2->GetAnchorPosition().x, widget2->GetAnchorPosition().y, widget2->GetPivot().x, widget2->GetPivot().y);
	//text->SetText(buffer);

	float h = CInput::GetAxis("Horizontal");
	float v = CInput::GetAxis("Vertical");

	static bool controlDown = false;

	if (CInput::GetKeyDown(DIK_LCONTROL))
	{
		controlDown = true;
	}
	if (CInput::GetKeyUp(DIK_LCONTROL))
	{
		controlDown = false;
	}
	if (CInput::GetKeyDown(DIK_R))
	{
		//widget2->SetAnchorPosition(Vector3::Zero());
		widget2->RefreshAnchor();
	}
	if (controlDown)
	{
		if (CInput::GetKeyDown(DIK_NUMPAD1))
		{
			widget2->SetAlignment(EAlignment::LEFT_BOTTOM);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD2))
		{
			widget2->SetAlignment(EAlignment::CENTER_BOTTOM);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD3))
		{
			widget2->SetAlignment(EAlignment::RIGHT_BOTTOM);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD4))
		{
			widget2->SetAlignment(EAlignment::LEFT_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD5))
		{
			widget2->SetAlignment(EAlignment::CENTER_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD6))
		{
			widget2->SetAlignment(EAlignment::RIGHT_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD7))
		{
			widget2->SetAlignment(EAlignment::LEFT_TOP);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD8))
		{
			widget2->SetAlignment(EAlignment::CENTER_TOP);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD9))
		{
			widget2->SetAlignment(EAlignment::RIGHT_TOP);
		}
	}
	else
	{
		if (CInput::GetKeyDown(DIK_NUMPAD1))
		{
			widget2->SetPivot(Vector2(0, 0));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD2))
		{
			widget2->SetPivot(Vector2(0.5f, 0));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD3))
		{
			widget2->SetPivot(Vector2(1, 0));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD4))
		{
			widget2->SetPivot(Vector2(0, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD5))
		{
			widget2->SetPivot(Vector2(0.5f, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD6))
		{
			widget2->SetPivot(Vector2(1, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD7))
		{
			widget2->SetPivot(Vector2(0, 1));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD8))
		{
			widget2->SetPivot(Vector2(0.5f, 1));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD9))
		{
			widget2->SetPivot(Vector2(1, 1));
		}
	}

	//Vector3 anchor_pos = widget2->GetAnchorPosition();
	//anchor_pos.x += h * CTime::deltaTime * 50;
	//anchor_pos.y += v * CTime::deltaTime * 50;
	//widget2->SetAnchorPosition(anchor_pos);

	if (h != 0 || v != 0)
	{
		float width = widget2->GetWidth();
		float height = widget2->GetHeight();
		width += h * CTime::deltaTime * 50;
		height += v * CTime::deltaTime * 50;
		widget2->SetWidth(width);
		widget2->SetHeight(height);
	}

	//Vector3 euler = go2->GetLocalEulerAngles();
	//euler.z += CTime::deltaTime * 30;
	//go2->SetLocalEulerAngles(euler);

	//euler = go->GetLocalEulerAngles();
	//euler.z -= CTime::deltaTime * 30;
	//go->SetLocalEulerAngles(euler);
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