#pragma once

#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"
#include"GUIDefines.h"
#include"UIView.h"
#include<functional>

BEGIN_NAMESPACE_GUI

struct UIBatch
{
	int		m_StartingIndex;
	int		m_Count;
};

class UISystem
{
	typedef function<void(PUIWidget)> ForeachCallback;
	const float ZNEAR = 0.001f;
	const float ZFAR = 1000.0f;
public:
	UISystem();
	~UISystem();

	void StartUp(int width, int height);
	void SetSize(int width, int height);
	void ShutDown();
	void RenderAll();
	void SubmitBatch(const vector<UIView*> list, PMaterial mat
		, int startingIndex, int count);
	void AddChild(PUIWidget widget);
	void RemoveChild(PUIWidget widget);
	void Foreach(PUIWidget widget, ForeachCallback callback);
	void ForeachAll(ForeachCallback callback);
	void ForeachWithModelMatrix(PUIWidget widget, Matrix4x4& baseMatrix, ForeachCallback callback);
	void ForeachAllWithModelMatrix(ForeachCallback callback);

private:
	Matrix4x4			m_ViewMatrix;
	Matrix4x4			m_ProjMatrix;
	PMeshBuffer			m_SharedMesh;
	PMaterial			m_SharedMaterial;
	PUIWidget			m_Root;
	vector<UIView*>		m_ForRenderList;
	vector<UIBatch>		m_Batchs;
};

END_NAMESPACE_GUI