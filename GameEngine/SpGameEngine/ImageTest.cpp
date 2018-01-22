#include"ImageTest.h"
#include"SpRendering\MeshFactory.h"
#include"SpAssetLoader\3DSModelLoader.h"

void CImageTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 2, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, -180, 0));
	InitLight();
	ImageLoader* image = _Resources->Load<ImageLoader>("textures/model.png");
	PTexture texture = Texture2D::Create(image);
	PMaterial mat_model = make_shared<Material>();
	mat_model->SetShader(Shader::Get("texture"))->SetMainTexture(texture);
	texture->SetEnvMode(ETexEnvMode::Modulate);
	ModelLoader* model = _Resources->Load<C3DSModelLoader>("models/model.3DS");
	go = _Maker->Instantiate(L"model");
	go->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f));
	go->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMeshRenderer* renderer = go->AddComponent<CMeshRenderer>();
	PMesh m(&model->m_model->m_meshes[0]);
	renderer->SetModel(PMeshBuffer(new MeshBuffer(m)))->SetMaterial(mat_model);

	ImageLoader* ground_image = _Resources->Load<ImageLoader>("textures/dlg01.bmp");
	PTexture ground_texture = Texture2D::Create(ground_image);
	PMaterial mat_ground = make_shared<Material>();
	mat_ground->SetShader(Shader::Get("texture"))->SetMainTexture(ground_texture);
	CGameObject* quadGo = _Maker->Instantiate(L"ground");
	quadGo->SetLocalPosition(Vector3(0, -3.5f, 0));
	quadGo->SetLocalScale(Vector3(7, 7, 7));
	quadGo->SetLocalEulerAngles(Vector3(-90, 0, 0));
	CMeshRenderer* ground_renderer = quadGo->AddComponent<CMeshRenderer>();
	ground_renderer->SetModel(_MeshFactory->SharedBuffer(EMeshType::Quad))->SetMaterial(mat_ground);

	CEditorTool::PrintTree();
}

void CImageTest::OnUpdate()
{
	Vector3 euler = go->GetLocalEulerAngles();
	euler.y += CTime::deltaTime * 50;
	go->SetLocalEulerAngles(euler);
}

void CImageTest::OnRender()
{

}

void CImageTest::OnClose()
{

}

void CImageTest::GetApplicationInfo(SApplicationInfo* info)
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

void CImageTest::InitLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
