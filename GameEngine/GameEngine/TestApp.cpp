#include"TestApp.h"
#include"Input.h"
#include"Time.h"
#include"MeshCube.h"
#include"RenderTexture.h"
#include"3DSFile.h"
#include"Resources.h"
#include"include\rapidxml\rapidxml.hpp"
#include<fstream>
#include<sstream>

void CTestApp::OnStart()
{
	//ifstream is("Config/engine.xml");
	//stringstream ss;
	//ss << is.rdbuf();
	//int size = ss.str().size();
	//rapidxml::xml_document<> doc;
	//char* buf = (char*)malloc(size + 1);
	//ss.read(buf, size);
	//buf[size] = '\0';
	//doc.parse<0>(buf);
	Engine->SetDrawDebug(true);
	Engine->SetDrawGrid(false);
	MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	MainCameraGo->SetLocalPosition(Vector3(0, 0, 5));
	MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));
	camera = Maker->Instantiate("Camera");
	CRenderTexture* renderTexture = CRenderTexture::Create(400, 400, true);
	CCamera* c = camera->AddComponent<CCamera>();
	c->Perspective(54.0f, (float)Application->GetWindowWidth() / (float)Application->GetWindowHeight(), 0.1f, 1000.0f);
	c->SetCameraClearFlag(ECameraClearFlag::SolidColor)->SetCameraClearColor(Color::black)->SetDepth(1)->SetRenderTexture(renderTexture);
	c->SetName("camera");
	camera->SetLocalPosition(Vector3(100, 3, -10));

	model = Maker->Instantiate("model");
	model->SetLocalPosition(Vector3(100, 0, 0));
	model->SetLocalScale(Vector3(0.12f, 0.12f, 0.12f));
	CMaterial* model_mat = Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("texture"));
	model_mat->SetMainTexture(CTexture2D::Create("textures/model.png"));
	model->AddComponent<CMeshRenderer>()->SetModel(Resources->Load<C3DSFile>("models/model.3DS"))->SetMaterial(model_mat);

	CMaterial* material = Maker->Instantiate<CMaterial>();
	material->SetMainTexture(renderTexture)->SetShader(CShader::Get("texture"));
	CMeshProvider* quad = Maker->Instantiate<CMeshCube>();
	go1 = Maker->Instantiate("parent");
	go1->AddComponent<CMeshRenderer>()->SetModel(quad)->SetMaterial(material);

	go2 = Maker->Instantiate("child");
	go2->AddComponent<CMeshRenderer>()->SetModel(quad);
	go2->SetParent(go1);
	go2->SetLocalPosition(Vector3::right * 1.4f);
	go2->SetLocalScale(Vector3::one * 0.5f);

	go3 = Maker->Instantiate("grandchild");
	go3->AddComponent<CMeshRenderer>()->SetModel(quad);
	go3->SetParent(go2);
	go3->SetLocalPosition(Vector3::right * 1.4f);
	go3->SetLocalScale(Vector3::one * 0.5f);
	CEditorTool::PrintTree();
}

void CTestApp::OnUpdate()
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime;

	Vector3 euler = go1->GetLocalEulerAngles();
	euler.x += v * 300;
	euler.y += h * 300;
	go1->SetLocalEulerAngles(euler);

	euler = go3->GetLocalEulerAngles();
	if (CInput::GetKey(DIK_NUMPAD8))
	{
		euler.x += CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD2))
	{
		euler.x -= CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD4))
	{
		euler.y -= CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD6))
	{
		euler.y += CTime::deltaTime * 300;
	}
	go3->SetLocalEulerAngles(euler);

	euler = model->GetLocalEulerAngles();
	euler.y += CTime::deltaTime * 300;
	model->SetLocalEulerAngles(euler);
}

void CTestApp::OnRender()
{
	
}

void CTestApp::OnClose()
{
	
}

void CTestApp::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = window_width;
		info->windowHeight = window_height;
		info->windowBits = 32;
		info->isFullScreen = false;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}