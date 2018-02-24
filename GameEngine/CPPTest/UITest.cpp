#include "UITest.h"
#include "SpCommon\Input.h"
#include <string>
#include <sstream>

#define KeyState(keyword) 	if (CInput::GetMouseDown(keyword)) {\
	mouseState.m_MouseKeyStates[keyword] = EKeyState::Down; }\
else if (CInput::GetMouseUp(keyword)) {\
	mouseState.m_MouseKeyStates[keyword] = EKeyState::Up; }\
else { mouseState.m_MouseKeyStates[keyword] = EKeyState::Normal; }

UITest::UITest(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void UITest::InitGL(HWND hwnd)
{
	GLRenderingStartUpParams params;
	params.m_HWND = hwnd;
	m_RI = new GLRendering();
	m_RI->StartUp(&params);
}

void UITest::OnInitialize()
{
	GLAppBase::OnInitialize();
	SetBackgroundColor(0.3f, 0.5f, 0.5f, 1.0f);
	CInput::Init(GetModuleHandle(NULL), m_Hwnd);

	PSpriteSet set = make_shared<SpriteSet>("D:\\GitHub\\MyToy\\GameEngine\\Assets\\atlas.png", "D:\\GitHub\\MyToy\\GameEngine\\Assets\\atlas.xml");

	m_UISystem = new UISystem();
	m_UISystem->StartUp(m_RI, m_WindowWidth, m_WindowHeight);

	m_MovedWidget = m_UISystem->Create<UIImage>();
	m_SubWidget = m_UISystem->Create<UIImage>();
	m_Button = m_UISystem->Create<UIImage>();

	m_UISystem->AddChild(m_MovedWidget);

	m_Button->SetSprite(set->GetSprite("btn_3.png"), true);
	m_SubWidget->SetSprite(set->GetSprite("sure.png"), true);
	m_MovedWidget->SetSprite(set->GetSprite("bg_1.png"), false);

	m_MovedWidget->AddChild(m_Button);
	m_Button->AddChild(m_SubWidget);
	m_MovedWidget->SetInteract(true);

	m_SubWidget->SetInteract(true);

	m_Label = m_UISystem->Create<UILabel>();
	m_Label->SetLocalPosition(Vector2(0, 200));
	m_Label->SetColor(Color::cyan);
	PTrueTypeFont f = _FontManager->LoadFont(1, FONT_PATH);
	m_FMG = dynamic_cast<FontMeshGenerator*>(m_Label.get());
	m_FMG->SetFont(f);
	m_FMG->SetFontSize(FONT_SIZE);
	m_FMG->SetTextAlignment(EAlignment::CENTER_MIDDLE);
	m_FMG->SetTextRect(SRect2D(0, 0, 400, 100));
	m_FMG->SetText(L"UILabel");
	m_FMG->SetIntervalX(0);
	m_FMG->SetIntervalY(0);
	m_FMG->SetPixelScale(1.0f);
	m_MovedWidget->AddChild(m_Label);

	m_ExtraInfo.resize(2);
}

void UITest::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
	CInput::GetState(EngineToolkit::GetGlobalRect(m_Hwnd, m_WindowWidth, m_WindowHeight));
	static SMouseState mouseState;
	mouseState.m_MousePosition = CInput::InputMousePosition();
	KeyState(EMouseKey::Left);
	KeyState(EMouseKey::Center);
	KeyState(EMouseKey::Right);
	m_UISystem->UpdateAll(mouseState);

	if (CInput::GetKeyDown(DIK_R))
	{
		m_MovedWidget->RemoveChild(m_SubWidget);
	}

	static bool shift = false, ctrl = false, rb = false;
	if (CInput::GetKeyDown(DIK_LSHIFT))
	{
		shift = true;
	}
	if (CInput::GetKeyUp(DIK_LSHIFT))
	{
		shift = false;
	}
	if (CInput::GetKeyDown(DIK_LCONTROL))
	{
		ctrl = true;
	}
	if (CInput::GetKeyUp(DIK_LCONTROL))
	{
		ctrl = false;
	}
	if (CInput::GetMouseDown(EMouseKey::Right))
	{
		rb = true;
	}
	if (CInput::GetMouseUp(EMouseKey::Right))
	{
		rb = false;
	}
	PUIWidget widget = ctrl ? m_SubWidget : m_MovedWidget;

	float moveSpeed = 100.0f, scaleSpeed = 10.0f, rotateSpeed = 50.0f;
	Vector2 position = widget->GetLocalPosition();
	Vector2 scale = widget->GetScale();
	Vector3 eulerAngles = widget->GetEulerAngles();
	if (CInput::GetKey(DIK_LEFT))
	{
		position.x -= moveSpeed * deltaTime;
	}
	if (CInput::GetKey(DIK_RIGHT))
	{
		position.x += moveSpeed * deltaTime;
	}
	if (CInput::GetKey(DIK_UP))
	{
		position.y += moveSpeed * deltaTime;
	}
	if (CInput::GetKey(DIK_DOWN))
	{
		position.y -= moveSpeed * deltaTime;
	}
	if (CInput::GetKey(DIK_Z))
	{
		scale -= Vector2::one * scaleSpeed * deltaTime;
	}
	if (CInput::GetKey(DIK_X))
	{
		scale += Vector2::one * scaleSpeed * deltaTime;
	}

	if (!shift)
	{
		if (CInput::GetKeyDown(DIK_NUMPAD7))
		{
			widget->SetPivot(Vector2(1.0f, 0.0f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD8))
		{
			widget->SetPivot(Vector2(0.5f, 0.0f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD9))
		{
			widget->SetPivot(Vector2(0.0f, 0.0f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD4))
		{
			widget->SetPivot(Vector2(1.0f, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD5))
		{
			widget->SetPivot(Vector2(0.5f, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD6))
		{
			widget->SetPivot(Vector2(0.0f, 0.5f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD1))
		{
			widget->SetPivot(Vector2(1.0f, 1.0f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD2))
		{
			widget->SetPivot(Vector2(0.5f, 1.0f));
		}
		if (CInput::GetKeyDown(DIK_NUMPAD3))
		{
			widget->SetPivot(Vector2(0.0f, 1.0f));
		}
	}
	else
	{
		if (CInput::GetKeyDown(DIK_NUMPAD7))
		{
			widget->SetAlignment(EAlignment::LEFT_TOP);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD8))
		{
			widget->SetAlignment(EAlignment::CENTER_TOP);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD9))
		{
			widget->SetAlignment(EAlignment::RIGHT_TOP);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD4))
		{
			widget->SetAlignment(EAlignment::LEFT_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD5))
		{
			widget->SetAlignment(EAlignment::CENTER_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD6))
		{
			widget->SetAlignment(EAlignment::RIGHT_MIDDLE);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD1))
		{
			widget->SetAlignment(EAlignment::LEFT_BOTTOM);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD2))
		{
			widget->SetAlignment(EAlignment::CENTER_BOTTOM);
		}
		if (CInput::GetKeyDown(DIK_NUMPAD3))
		{
			widget->SetAlignment(EAlignment::RIGHT_BOTTOM);
		}
	}

	if (CInput::GetKey(DIK_N))
	{
		eulerAngles.z -= deltaTime * rotateSpeed;
	}
	if (CInput::GetKey(DIK_M))
	{
		eulerAngles.z += deltaTime * rotateSpeed;
	}

	widget->SetLocalPosition(position);
	widget->SetScale(scale);
	widget->SetEulerAngles(eulerAngles);

	static stringstream ss;
	ss.str("");
	ss << "ui instance:" << m_UISystem->instanceCount();
	m_ExtraInfo[0] = std::move(ss.str());
	ss.str("");
	ss << "ui drawcalls:" << m_UISystem->drawcalls();
	m_ExtraInfo[1] = std::move(ss.str());
}

void UITest::OnRender()
{
	GLAppBase::OnRender();

	m_UISystem->RenderAll();
}

void UITest::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	m_UISystem->SetSize(width, height);
}
