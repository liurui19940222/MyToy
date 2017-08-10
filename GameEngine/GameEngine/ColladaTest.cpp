#include"ColladaTest.h"
#include"ColladaFile.h"

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 7, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(-30, 180, 0));

	model = _Maker->Instantiate("model");
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f));
	model->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("texture"));
	model_mat->SetMainTexture(CTexture2D::Create("textures/model.png"));
	model->AddComponent<CMeshRenderer>()->SetModel(_Resources->Load<CColladaFile>("models/walk.dae"))->SetMaterial(model_mat);
}

void CColladaTest::OnUpdate()
{

}

void CColladaTest::OnRender()
{

}

void CColladaTest::OnClose()
{
	
}

void CColladaTest::GetApplicationInfo(SApplicationInfo* info)
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