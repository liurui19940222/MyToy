#include "TextTest.h"

void CTextTest::OnStart()
{
	_MainCameraGo->SetLocalPosition(Vector3(0, 0, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));

	CTrueTypeFont* font = FontManager->LoadFont(1, "C:/Windows/Fonts/StencilStd.otf");
	go = _Maker->Instantiate();
	go->SetLocalPosition(Vector3(0, 0, 0));
	text = go->AddComponent<CTextRenderer>();
	text->SetFont(font)->SetFontSize(10)->SetTextRect(SRect2D{ 0, 0, 5, 1 });
	text->SetText(L"Sams Publishing & Pearson Education Inc., \nCD-ROM and software copyright (C) 2003 Sams Publishing & Pearson");
	//text->SetText(L"дasdfsdf\nͨ564654564");

	//CBitImage* image = _Engine->CreateImage("textures/model.png");
	//CTexture* texture = CTexture::Create(image);
	//CGameObject* texGo = _Engine->CreateGameObject();
	//texGo->AddComponent<CSpriteRenderer>()->SetTexture(texture);
	//texGo->SetLocalScale(Vector3(0.1, 0.1, 0));
	//texGo->SetLocalPosition(Vector3(0, -1, 0));

	//CAtlas* atlas = (*font->GetAtlases(10))[0];
	//CTexture* tex = CTexture::Create((UCHAR*)(atlas->GetPixels()), atlas->GetWidth(), atlas->GetHeight());
	//textureGo = _Engine->CreateGameObject();
	//textureGo->AddComponent<CSpriteRenderer>()->SetTexture(tex);
	//textureGo->SetLocalScale(Vector3(0.25, 0.25, 0.25));
	//textureGo->SetLocalPosition(Vector3(-1, 0, 0));
}

void CTextTest::OnUpdate()
{
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

	float h = CInput::GetAxis("Horizontal");
	float v = CInput::GetAxis("Vertical");
	Vector3 euler = go->GetLocalEulerAngles();
	euler.y += h * CTime::deltaTime * 60;
	euler.x += v * CTime::deltaTime * 60;
	go->SetLocalEulerAngles(euler);
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