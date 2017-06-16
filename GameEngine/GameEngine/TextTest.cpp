#include "TextTest.h"

void CTextTest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));

	CTrueTypeFont* font = CFontManager::GetInstance()->LoadFont(1, "fonts/msyh.ttf");
	go = Engine->CreateGameObject();
	go->SetPosition(Vector3(1, 2, 0));
	CTextRenderer* text = go->AddComponent<CTextRenderer>();
	text->Init(font, NULL, 10, 0, 0.5f, Color::red(), EAlignment::LEFT_CENTER, SRect2D{ 0, 0, 5, 1 });
	text->SetText(L"Sams Publishing & Pearson Education Inc., CD-ROM and software copyright (C) 2003 Sams Publishing & Pearson Education Inc., ");
	//text->SetText(L"使用FreeImage写通用图像加载器 - cbbbc");

	CAtlas* atlas = (*font->GetAtlases(10))[0];
	CTexture* tex = CTexture::Create((UCHAR*)(atlas->GetPixels()), atlas->GetWidth(), atlas->GetHeight());
	textureGo = Engine->CreateGameObject();
	textureGo->AddComponent<CSpriteRenderer>()->SetTexture(tex);
	textureGo->SetLocalScale(Vector3(0.25, 0.25, 0.25));
	textureGo->SetPosition(Vector3(-1, 0, 0));
}

void CTextTest::OnUpdate()
{

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