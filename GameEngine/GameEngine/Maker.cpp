#include"Maker.h"
#include"Texture2D.h"
#include"Material.h"
#include"MeshRenderer.h"
#include"MeshFactory.h"
#include"MessageCenter.h"
#include"MessageDef.h"
#include"Light.h"

void CMaker::OnInitialize()
{

}

CGameObject* CMaker::Instantiate()
{
	return Instantiate(L"NewGameObject");
}

CGameObject* CMaker::Instantiate(wstring name)
{
	CGameObject* go = Instantiate<CGameObject>();
	go->SetName(name);
	return go;
}

void CMaker::AddGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			return;
		}
		it++;
	}
	m_gameObjects.push_back(go);
}

void CMaker::RemoveGameObject(CGameObject* go)
{
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it) == go)
		{
			m_gameObjects.erase(it);
			return;
		}
		it++;
	}
}

void CMaker::DestroyGameObject(CGameObject* go)
{
	if (go->parent)
		go->parent->RemoveChild(go);
	_Maker->RemoveGameObject(go);

	vector<CGameObject*> list;
	ForeachGameObject(go, [&list](CGameObject* go, int depth) {
		go->OnDestroy();
		go->OnRelease();
		list.push_back(go);
	});
	for (int i = list.size() - 1; i >= 0; --i)
		delete(list[i]);
}

void CMaker::ForeachAllGameObject(ForeachGoCallback callback)
{
	ForeachGameObject(callback);
}

void CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObjectR(go, callback, 0);
}

void CMaker::ForeachGameObjectR(ForeachGoCallbackR callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (!ForeachGameObjectR(*it, callback, 0))
			break;
		it++;
	}
}

bool CMaker::ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth)
{
	if (!callback(go, depth)) return false;
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		if (!ForeachGameObjectR(*it, callback, depth + 1))
			return false;
	}
	return true;
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth)
{
	callback(go, depth);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1);
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallback callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0);
}

void CMaker::ForeachGameObject(ForeachGoCallback callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it, callback, 0);
		it++;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback)
{
	if (go == NULL || callback == NULL) return;
	ForeachGameObject(go, callback, 0, go->GetModelToWorldMat());
}

void CMaker::ForeachGameObject(ForeachGoCallbackMatrix callback)
{
	if (m_gameObjects.size() == 0 || callback == NULL) return;
	auto it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		ForeachGameObject(*it, callback, 0, (*it)->GetModelToWorldMat());
		it++;
	}
}

void CMaker::ForeachGameObject(CGameObject* go, ForeachGoCallbackMatrix callback, int depth, Matrix4x4& modelMatrix)
{
	callback(go, depth, modelMatrix);
	for (auto it = go->childs.begin(); it != go->childs.end(); ++it)
	{
		ForeachGameObject(*it, callback, depth + 1, modelMatrix * (*it)->GetModelToWorldMat());
	}
}

CGameObject* CMaker::FindGameObject(int instanceId)
{
	CGameObject* result = NULL;
	ForeachGameObjectR([instanceId, &result](CGameObject* go, int depth) {
		if (go->GetInstanceId() == instanceId)
		{
			result = go;
			return false;
		}
		return true;
	});
	return result;
}

CGameObject* CMaker::FindGameObjectWithTag(const string& tag)
{
	CGameObject* result = NULL;
	ForeachGameObjectR([&tag, &result](CGameObject* go, int depth) {
		if (go->GetTag() == tag)
		{
			result = go;
			return false;
		}
		return true;
	});
	return result;
}

CGameObject* CMaker::FindGameObjectWithName(const wstring& name)
{
	CGameObject* result = NULL;
	ForeachGameObjectR([&name, &result](CGameObject* go, int depth) {
		if (go->GetName() == name)
		{
			result = go;
			return false;
		}
		return true;
	});
	return result;
}

CGameObject* CMaker::CreateCube()
{
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetShader(CShader::Get("light"));
	mat->SetColor(Color::white);
	CGameObject* go = _Maker->Instantiate(L"Cube");
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))->SetMaterial(mat);
	return go;
}

CGameObject* CMaker::CreateQuad()
{
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetShader(CShader::Get("light"));
	mat->SetColor(Color::white);
	CGameObject* go = _Maker->Instantiate(L"Quad");
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Quad))->SetMaterial(mat);
	return go;
}

CGameObject* CMaker::CreateDirectionalLight()
{
	CGameObject* directionalGo = _Maker->Instantiate(L"DirectionLight");
	directionalGo->SetLocalEulerAngles(Vector3(50, 0, 0));
	CLight* light0 = directionalGo->AddComponent<CLight>();
	light0->SetColor(Color::white);
	light0->SetIntensity(1.0f);
	light0->SetCastShadow(false);
	return directionalGo;
}

CGameObject* CMaker::CreatePointLight()
{
	CGameObject* pointGo = _Maker->Instantiate(L"light1");
	CLight* pointLight = pointGo->AddComponent<CLight>();
	pointLight->SetType(ELightType::Point);
	pointLight->SetColor(Color::white);
	pointLight->SetIntensity(1.0f);
	return pointGo;
}