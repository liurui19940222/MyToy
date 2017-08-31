#include"Application.h"
#include"Engine.h"
#include"EngineDefine.h"
#include"Input.h"
#include"Time.h"
#include"RenderTexture.h"
#include"3DSFile.h"
#include"Resources.h"
#include"Debug.h"
#include"Config.h"
#include"SkinnedMeshRenderer.h"
#include"ColladaFile.h"
#include<vector>

using namespace std;

class CColladaTest : public CGameWindow
{
	CColladaFile* collada;
	Model* m_model;
	CGameObject* model;
	vector<AnimationClip*> m_clips;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//应用程序必须实现的函数
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

