#include"TextTest.h"
#include"SpCommon\Converter.h"

void CTextTest::OnStart()
{
	_MainCameraGo->SetLocalPosition(Vector3(0, 0, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));

	PTrueTypeFont font = _FontManager->LoadFont(1, "C:/Windows/Fonts/simkai.ttf");
	go = _Maker->Instantiate();
	go->SetLocalPosition(Vector3(0, 0, 0));
	go->SetLayer(Layer::Default);
	text = go->AddComponent<CTextRenderer>();
	text->SetFont(font);
	text->SetFontSize(30);
	text->SetTextRect(SRect2D{ 0, 0, 3, 1 });
	text->SetColor(Color::green);
	text->SetSingleLine(false);
	text->SetText(L"那么照此格式输出的字母都能以基线对齐。但是所有的字都比设置的位置要低一些，想问一下我的计算方式是不是不对，网上都是这么计算的  ……");
}

void CTextTest::OnUpdate()
{
	if (Input::GetKeyDown(DIK_NUMPAD1))
	{
		text->SetTextAlignment(EAlignment::LEFT_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD2))
	{
		text->SetTextAlignment(EAlignment::CENTER_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD3))
	{
		text->SetTextAlignment(EAlignment::RIGHT_BOTTOM);
	}
	if (Input::GetKeyDown(DIK_NUMPAD4))
	{
		text->SetTextAlignment(EAlignment::LEFT_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD5))
	{
		text->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD6))
	{
		text->SetTextAlignment(EAlignment::RIGHT_MIDDLE);
	}
	if (Input::GetKeyDown(DIK_NUMPAD7))
	{
		text->SetTextAlignment(EAlignment::LEFT_TOP);
	}
	if (Input::GetKeyDown(DIK_NUMPAD8))
	{
		text->SetTextAlignment(EAlignment::CENTER_TOP);
	}
	if (Input::GetKeyDown(DIK_NUMPAD9))
	{
		text->SetTextAlignment(EAlignment::RIGHT_TOP);
	}
	
	if (Input::GetKeyDown(DIK_1))
	{
		text->SetColor(Color::orange);
	}
	if (Input::GetKeyDown(DIK_2))
	{
		text->SetColor(Color::cyan);
	}
	if (Input::GetKeyDown(DIK_3))
	{
		text->SetColor(Color::green);
	}
	if (Input::GetKeyDown(DIK_4))
	{
		text->SetColor(Color::red);
	}

	if (Input::GetKey(DIK_N))
	{
		SRect2D rect = text->GetTextRect();
		rect.halfSize.x -= CTime::deltaTime * 10;
		text->SetTextRect(rect);
	}

	if (Input::GetKey(DIK_M))
	{
		SRect2D rect = text->GetTextRect();
		rect.halfSize.x += CTime::deltaTime * 10;
		text->SetTextRect(rect);
	}
	
	float h = Input::GetAxis("Horizontal");
	float v = Input::GetAxis("Vertical");
	Vector3 euler = go->GetLocalEulerAngles();
	euler.y += h * CTime::deltaTime * 60;
	euler.x += v * CTime::deltaTime * 60;
	go->SetLocalEulerAngles(euler);

	//text->SetText(CConverter::FormatWString(L"FPS:%g", CTime::fps));
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