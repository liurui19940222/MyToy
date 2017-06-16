#include "TextTest.h"

void CTextTest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));

	CTrueTypeFont* font = FontManager->LoadFont(1, "fonts/msyh.ttf");
	go = Engine->CreateGameObject();
	go->SetPosition(Vector3(0, 0, 0));
	text = go->AddComponent<CTextRenderer>();
	text->Init(font, NULL, 10, 0, 0.5f, Color::white(), EAlignment::CENTER_MIDDLE, SRect2D{ 0, 0, 5, 1 });
	text->SetText(L"Sams Publishing & Pearson Education Inc., \nCD-ROM and software copyright (C) 2003 Sams Publishing & Pearson");
	//text->SetText(L"дasdfsdf\nͨ564654564");

	//CAtlas* atlas = (*font->GetAtlases(10))[0];
	//CTexture* tex = CTexture::Create((UCHAR*)(atlas->GetPixels()), atlas->GetWidth(), atlas->GetHeight());
	//textureGo = Engine->CreateGameObject();
	//textureGo->AddComponent<CSpriteRenderer>()->SetTexture(tex);
	//textureGo->SetLocalScale(Vector3(0.25, 0.25, 0.25));
	//textureGo->SetPosition(Vector3(-1, 0, 0));
}

void CTextTest::OnUpdate()
{
	if (CInput::GetKeyDown(DIK_NUMPAD1))
	{
		text->SetAlignment(EAlignment::LEFT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD2))
	{
		text->SetAlignment(EAlignment::CENTER_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD3))
	{
		text->SetAlignment(EAlignment::RIGHT_BOTTOM);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD4))
	{
		text->SetAlignment(EAlignment::LEFT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD5))
	{
		text->SetAlignment(EAlignment::CENTER_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD6))
	{
		text->SetAlignment(EAlignment::RIGHT_MIDDLE);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD7))
	{
		text->SetAlignment(EAlignment::LEFT_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD8))
	{
		text->SetAlignment(EAlignment::CENTER_TOP);
	}
	if (CInput::GetKeyDown(DIK_NUMPAD9))
	{
		text->SetAlignment(EAlignment::RIGHT_TOP);
	}
	
	if (CInput::GetKeyDown(DIK_1))
	{
		text->SetColor(Color::orange());
	}
	if (CInput::GetKeyDown(DIK_2))
	{
		text->SetColor(Color::cyan());
	}
	if (CInput::GetKeyDown(DIK_3))
	{
		text->SetColor(Color::green());
	}
	if (CInput::GetKeyDown(DIK_4))
	{
		text->SetColor(Color::red());
	}

	float h = CInput::GetAxis("Horizontal");
	float v = CInput::GetAxis("Vertical");
	Vector3 euler = go->GetEulerAngles();
	euler.y += h * CTime::deltaTime * 60;
	euler.x += v * CTime::deltaTime * 60;
	go->SetEulerAngles(euler);
}

void CTextTest::OnRender()
{

}

void CTextTest::OnClose()
{

}

void CTextTest::GetApplicationInfo(SApplicationInfo* info)
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