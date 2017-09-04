#include "GUITest.h"
#include "GUISystem.h"
#include"MeshFactory.h"

CGameObject* r;
void GUITest::OnStart()
{
	FontManager->LoadFont(2, "C:/Windows/Fonts/simsun.ttc");
	CTexture* tex = CTexture2D::Create("F://monthad_1.png");
	CTexture* tex2 = CTexture2D::Create("F://avatar_15.png");
	go = _Maker->Instantiate("widget");
	widget = go->AddComponent<CGUILabel>();
	widget->SetWidth(100)->SetHeight(30);
	//widget->SetFill(true)->SetFillColor(Color::blue());
	widget->SetAlignment(EAlignment::CENTER_MIDDLE);
	widget->SetPivot(Vector2(0.5f, 0.5f));
	widget->SetCollide(true)->SetFill(true);
	widget->SetFillColor(Color::grey);
	widget->SetFont(FontManager->GetFont(2));
	widget->SetText(L"Button");
	widget->AddOnMouseClickListener([](Vector2 mousepos) {
		CDebug::Box(L"x:%g y:%g", mousepos.x, mousepos.y);
	});
	//widget->SetText(L"Ïß");
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetShader(CShader::Get("color"));
	CMeshRenderer* render = _Maker->Instantiate("Cube")->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube));
	r = render->gameObject;
	render->SetMaterial(mat);
	render->gameObject->SetLocalPosition(Vector3(0, 0, 10));
	/*CGameObject* go3 = _Maker->Instantiate();
	go3->AddComponent<CGUIImage>()->SetTexture(tex2)->SetLayer(2)->SetCollide(true);

	go2 = _Maker->Instantiate("widget2");
	go2->SetParent(go);
	widget2 = go2->AddComponent<CGUILable>();
	widget2->SetWidth(300)->SetHeight(300);
	//widget2->SetFillColor(Color(1, 0, 0, 0.1f))->SetFill(true);
	widget2->SetLayer(3);
	widget2->SetAlignment(EAlignment::CENTER_MIDDLE);
	widget2->SetPivot(Vector2(0.5f, 0.5f));
	widget2->SetCollide(false);
	CTrueTypeFont* font = FontManager->LoadFont(1, "fonts/msyh.ttf");
	widget2->SetColor(Color::red)->SetFont(font)->SetFontSize(5)->SetIntervalY(25)->SetText(L"Sams Publishing & Pearson Education Inc., \nCD-ROM and software copyright (C) 2003 Sams Publishing & Pearson");
	*/



	//FontManager->LoadFont(2, "fonts/msyh.ttf");
	//CGameObject* texGo = _Engine->CreateGameObject("Text");
	//texGo->SetLocalPosition(Vector3(-4.75f, 4.05f, 0));
	//text = texGo->AddComponent<CTextRenderer>();
	//text->Init(FontManager->GetFont(2), NULL, 8, 0, 0.5f, Color::green(), EAlignment::LEFT_TOP, SRect2D(0, 0, 2, 1));
}

void GUITest::OnUpdate()
{
	CGUILabel* text = widget;
	if (CInput::GetKeyDown(DIK_NUMPAD1))
	{
		text->SetTextAlignment(EAlignment::LEFT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD2))
	{
		text->SetTextAlignment(EAlignment::CENTER_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD3))
	{
		text->SetTextAlignment(EAlignment::RIGHT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD4))
	{
		text->SetTextAlignment(EAlignment::LEFT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD5))
	{
		text->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD6))
	{
		text->SetTextAlignment(EAlignment::RIGHT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD7))
	{
		text->SetTextAlignment(EAlignment::LEFT_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD8))
	{
		text->SetTextAlignment(EAlignment::CENTER_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD9))
	{
		text->SetTextAlignment(EAlignment::RIGHT_TOP);
	}

	if (CInput::GetKeyDown(DIK_1))
	{
		text->SetColor(Color::orange);
	}
	if (CInput::GetKeyDown(DIK_2))
	{
		text->SetColor(Color::cyan);
	}
	if (CInput::GetKeyDown(DIK_3))
	{
		text->SetColor(Color::green);
	}
	if (CInput::GetKeyDown(DIK_4))
	{
		text->SetColor(Color::red);
	}

	if (CInput::GetKey(DIK_N))
	{
		float w = text->GetWidth();
		w -= CTime::deltaTime * 10;
		text->SetWidth(w);
	}

	if (CInput::GetKey(DIK_M))
	{
		float w = text->GetWidth();
		w += CTime::deltaTime * 10;
		text->SetWidth(w);
	}

float h = CInput::GetAxis("Horizontal");
float v = CInput::GetAxis("Vertical");
Vector3 anchor_pos = widget->GetAnchorPosition();
anchor_pos.x += h * CTime::deltaTime * 50;
anchor_pos.y += v * CTime::deltaTime * 50;
widget->SetAnchorPosition(anchor_pos);
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