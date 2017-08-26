#include"ColladaTest.h"
#include"ColladaFile.h"
#include"MeshCube.h"
#include"Maker.h"
#include<glm\glm.hpp>

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 6.8, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(-25, 180, 0));

	model = _Maker->Instantiate("model");

	//model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 2);
	//model->SetLocalEulerAngles(Vector3(0, 90, 0));
	//CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"))
	//	->SetMainTexture(CTexture2D::Create("textures/longxia.png"));
	//collada = _Resources->Load<CColladaFile>("models/longxia.xml");

	//model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 10);
	//model->SetLocalEulerAngles(Vector3(0, 0, 0));
	//CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"));
	//collada = _Resources->Load<CColladaFile>("models/walk.xml");
	
	model->SetLocalPosition(Vector3(0, 1, 0));
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 1.8);
	model->SetLocalEulerAngles(Vector3(0, -70, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"))
		->SetMainTexture(CTexture2D::Create("textures/shake.png"));
	collada = _Resources->Load<CColladaFile>("models/shake.xml");

	model->AddComponent<CSkinnedMeshRenderer>()->SetModel(collada)->SetMaterial(model_mat);
}

void CColladaTest::OnUpdate()
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 100;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 100;

	Vector3 euler = model->GetLocalEulerAngles();
	euler.y += h;
	model->SetLocalEulerAngles(euler);
	static bool b = false;
	if (CInput::GetKeyDown(DIK_SPACE))
	{
		b = true;
	}
	if (CInput::GetKeyDown(DIK_N))
	{
		b = false;
	}

	if(!b)
		collada->Sample(CTime::time);
	else
		collada->SampleB(CTime::time);
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