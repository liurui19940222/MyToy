#pragma once

#include"SpRendering\Material.h"
#include"SpRendering\Rendering.h"
#include"MeshBufferUIInstance.h"
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

	void StartUp(IRenderingInterface* ri, int width, int height);
	void SetSize(int width, int height);
	void ShutDown();
	void UpdateAll(SMouseState mouseState);
	void RenderAll();
	void SubmitBatch(const vector<UIView*> list, PMaterial mat, PTexture texture, int startingIndex, int count);
	void DrawCall(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors, vector<SRect2D>& rects, vector<Matrix4x4>& modelMatrices, PTexture texture);
	void AddChild(PUIWidget widget);
	void RemoveChild(PUIWidget widget);
	void Foreach(PUIWidget widget, ForeachCallback callback);
	void ForeachAll(ForeachCallback callback);
	void ForeachWithModelMatrix(PUIWidget widget, Matrix4x4& baseMatrix, ForeachCallback callback);
	void ForeachAllWithModelMatrix(ForeachCallback callback);

	inline PTexture GetSharedTexture() const { return m_SharedTexture; }
	inline PMaterial GetSharedMaterial() const { return m_SharedMaterial; }
	inline PUIWidget GetRoot() const { return m_Root; }
	inline Vector2 ScreenPointToView(const Vector2& point) { return point - m_Root->GetHalfSize(); }
	inline Vector2 ViewPointToScreen(const Vector2& point) { return point + m_Root->GetHalfSize(); }

	template<typename T> shared_ptr<T> Create()
	{
		shared_ptr<T> ptr = make_shared<T>();
		ptr->m_System = this;
		return ptr;
	}

private:
	IRenderingInterface*	m_RI;
	Matrix4x4				m_ViewMatrix;
	Matrix4x4				m_ProjMatrix;
	PMeshBufferUIInstance	m_SharedMesh;
	PTexture				m_SharedTexture;
	PMaterial				m_SharedMaterial;
	PUIWidget				m_Root;
	IInteractable*			m_LastIntracted;
	vector<UIView*>			m_ForRenderList;
	vector<IInteractable*>	m_ForInteractList;
	vector<TexcoordRange>	m_TexcoordRanges;
	vector<Color>			m_Colors;
	vector<SRect2D>			m_RectList;
	vector<Matrix4x4>		m_ModelMatrices;
};

END_NAMESPACE_GUI