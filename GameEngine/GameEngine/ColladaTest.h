#include"Application.h"
#include"Engine.h"
#include"EngineDefine.h"
#include"Input.h"
#include"Time.h"
#include"MeshCube.h"
#include"RenderTexture.h"
#include"3DSFile.h"
#include"Resources.h"
#include"Debug.h"
#include"Config.h"
#include"SkinnedMeshRenderer.h"

class CColladaTest : public CGameWindow
{
	CGameObject* model;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//Ӧ�ó������ʵ�ֵĺ���
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

