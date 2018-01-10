#include"Application.h"
#include"Engine.h"
#include"..\SpCommon\EngineDefine.h"
#include"Input.h"
#include"Time.h"
#include"RenderTexture.h"
#include"3DSFile.h"
#include"Resources.h"
#include "..\SpCommon\Debug.h"
#include"Config.h"
#include"SkinnedMeshRenderer.h"
#include"ColladaFile.h"
#include"Light.h"
#include<vector>

using namespace std;

class CLightTest : public CGameWindow
{
	CColladaFile* collada;
	Model* m_model;
	CGameObject* model;
	CGameObject* directionalGo;
	CGameObject* pointGo;
	CLight* pointLight;
	vector<AnimationClip*> m_clips;
	CTexture* m_texture;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//应用程序必须实现的函数
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

