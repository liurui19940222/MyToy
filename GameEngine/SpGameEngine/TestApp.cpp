#include"TestApp.h"
#include"SpCommon\Input.h"
#include"Time.h"
#include"SpRendering\RenderTexture.h"
#include"SpAssetLoader\3DSModelLoader.h"
#include"Resources.h"
#include "SpCommon\Debug.h"
#include"Config.h"
#include"SpRendering\MeshFactory.h"

void CTestApp::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 0, 5));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));
	camera = _Maker->Instantiate(L"Camera");
	PRenderTexture renderTexture = RenderTexture::Create(400, 400, true);
	CCamera* c = camera->AddComponent<CCamera>();
	c->Perspective(54.0f, _SCW / _SCH, 0.1f, 1000.0f);
	c->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	c->SetCameraClearColor(Color::black);
	c->SetDepth(1);
	c-> SetRenderTexture(renderTexture);
	c->SetName(L"camera");
	camera->SetLocalPosition(Vector3(100, 3, -10));
	model = _Maker->Instantiate(L"model");
	model->SetLocalPosition(Vector3(100, 0, 0));
	model->SetLocalScale(Vector3(0.12f, 0.12f, 0.12f));
	shared_ptr<Material> model_mat = make_shared<Material>();
	model_mat->SetShader(Shader::Get("texture"));
	model_mat->SetMainTexture(Texture2D::Create("textures/model.png"));

	C3DSModelLoader* loader = _Resources->Load<C3DSModelLoader>("models/model.3DS");
	PMeshBuffer buff(new MeshBuffer(loader->m_model->m_meshes[0]));
	model->AddComponent<CMeshRenderer>()->SetModel(buff)->SetMaterial(model_mat);

	PMaterial material(new Material());
	material->SetMainTexture(renderTexture)->SetShader(Shader::Get("texture"));
	PMeshBuffer quad = _MeshFactory->SharedBuffer(EMeshType::Cube);
	go1 = _Maker->Instantiate(L"parent");
	go1->AddComponent<CMeshRenderer>()->SetModel(quad)->SetMaterial(material);

	go2 = _Maker->Instantiate(L"child");
	go2->AddComponent<CMeshRenderer>()->SetModel(quad);
	go2->SetParent(go1);
	go2->SetLocalPosition(Vector3::right * 1.4f);
	go2->SetLocalScale(Vector3::one * 0.5f);

	go3 = _Maker->Instantiate(L"grandchild");
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
	go1->SetRotation(go1->GetRotation());
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
		info->windowWidth = _Config->GetValue<int>("window", "width");
		info->windowHeight = _Config->GetValue<int>("window", "height");
		info->isFullScreen = _Config->GetValue<bool>("window", "full_screen");
		info->windowBits = 32;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}