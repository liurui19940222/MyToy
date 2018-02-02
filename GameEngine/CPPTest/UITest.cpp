#include "UITest.h"
#include "SpCommon\Input.h"

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
	Color color = Color::Hex(0x314D79FF);
	SetBackgroundColor(color.r, color.g, color.b, color.a);
	CInput::Init(GetModuleHandle(NULL), m_Hwnd);

	PTexture2D tex = Texture2D::Create("D:\\GitHub\\MyToy\\GameEngine\\Assets\\gift_7.png");
	PTexture2D tex2 = Texture2D::Create("D:\\GitHub\\MyToy\\GameEngine\\Assets\\addtext_19.png");

	m_UISystem = new UISystem();
	m_UISystem->StartUp(m_RI, m_WindowWidth, m_WindowHeight);
	//PUIView view1 = m_UISystem->Create<UIView>();
	PUIImage view2 = m_UISystem->Create<UIImage>();
	PUIImage view3 = m_UISystem->Create<UIImage>();
	//PUIView view4 = make_shared<UIView>();
	//PUIView view5 = make_shared<UIView>();
	//view1->SetWidth(100);
	view2->SetRect(SRect2D(0.0f, 0.0f, 120.0f, 55.0f));
	view3->SetRect(SRect2D(0.0f, 0.0f, 50.0f, 30.0f));
	view3->SetSprite(Sprite::CreateSprite(tex2, TexcoordRange::full), true);
	view2->SetSprite(Sprite::CreateSprite(tex, TexcoordRange::full), true);
	//view4->SetMaterial(mat);
	//m_UISystem->AddChild(view1);
	m_UISystem->AddChild(view2);
	//m_UISystem->AddChild(view4);
	//m_UISystem->AddChild(view5);
	view2->AddChild(view3);
	m_MovedWidget = view2;
	m_MovedWidget->SetInteract(true);

	/*m_MovedWidget->AddMouseDownListener([](const Vector2& pos) {
		CDebug::Log(L"parent mouse down");
	});

	m_MovedWidget->AddMouseUpListener([](const Vector2& pos) {
		CDebug::Log(L"parent mouse up");
	});

	m_MovedWidget->AddMouseEnterListener([](const Vector2& pos) {
		CDebug::Log(L"parent mouse enter");
	});

	m_MovedWidget->AddMouseExitListener([](const Vector2& pos) {
		CDebug::Log(L"parent mouse exit");
	});

	m_MovedWidget->AddMouseOverListener([](const Vector2& pos) {
		CDebug::Log(L"parent mouse over");
	});*/

	m_SubWidget = view3;
	m_SubWidget->SetInteract(true);

	/*m_SubWidget->AddMouseDownListener([](const Vector2& pos) {
		CDebug::Log(L"sub mouse down");
	});

	m_SubWidget->AddMouseUpListener([](const Vector2& pos) {
		CDebug::Log(L"sub mouse up");
	});

	m_SubWidget->AddMouseEnterListener([](const Vector2& pos) {
		CDebug::Log(L"sub mouse enter");
	});

	m_SubWidget->AddMouseExitListener([](const Vector2& pos) {
		CDebug::Log(L"sub mouse exit");
	});

	m_SubWidget->AddMouseOverListener([](const Vector2& pos) {
		CDebug::Log(L"sub mouse over");
	});*/
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
