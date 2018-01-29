#include "UISystem.h"
#include "SpRendering\MeshFactory.h"
#include "SpCommon\EngineDefine.h"
#include "SpCommon\Debug.h"

USING_NAMESPACE_GUI;

UISystem::UISystem()
{
}


UISystem::~UISystem()
{
}

void UISystem::StartUp(int width, int height)
{
	m_SharedMesh = _MeshFactory->CreateBuffer(EMeshType::Quad);
	m_SharedMaterial = make_shared<Material>();
	m_Root = make_shared<UIWidget>();
	m_Root->m_System = this;
	m_ViewMatrix = Matrix4x4::LookAt(Vector3(0, 0, 100), Vector3::zero, Vector3::up);
	SetSize(width, height);
}

void UISystem::SetSize(int width, int height)
{
	float halfWidth = (float)width * 0.5f;
	float halfHeight = (float)height * 0.5f;
	m_Root->SetRect(SRect2D(0, 0, halfWidth, halfHeight));
	m_ProjMatrix = Matrix4x4::Ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, ZNEAR, ZFAR);
}

void UISystem::RenderAll()
{
	/*
	* 深度遍历所有View，计算模型矩阵，加入到一个线性表
	*/
	m_ForRenderList.clear();
	ForeachAllWithModelMatrix([this](PUIWidget widget) {
		if (IS_TYPE(UIView, widget.get()))
			m_ForRenderList.push_back((UIView*)widget.get());
	});

	int size = m_ForRenderList.size();

	if (size == 0)
		return;

	/*
	* 遍历所有的View，根据使用的Material来组合成不同的批次
	* 按提交给GPU渲染
	*/
	int beginMaterialId = m_ForRenderList[0]->GetMaterialId();
	int beginIndex = 0;
	int materialId = 0;
	for (int i = 0; i < size; ++i)
	{
		materialId = m_ForRenderList[i]->GetMaterialId();
		if (materialId != beginMaterialId)
		{
			SubmitBatch(m_ForRenderList, materialId ? m_ForRenderList[i - 1]->m_Material
				: m_SharedMaterial, beginIndex, i - beginIndex);
			beginIndex = i;
			beginMaterialId = materialId;
		}
	}
	if (beginIndex < size)
	{
		SubmitBatch(m_ForRenderList, materialId ? m_ForRenderList[beginIndex]->m_Material
			: m_SharedMaterial, beginIndex, size - beginIndex);
	}
}

void UISystem::SubmitBatch(const vector<UIView*> list, PMaterial mat, int startingIndex, int count)
{

}

void UISystem::AddChild(PUIWidget widget)
{
	m_Root->AddChild(widget);
}

void UISystem::RemoveChild(PUIWidget widget)
{
	m_Root->RemoveChild(widget);
}

void UISystem::Foreach(PUIWidget widget, ForeachCallback callback)
{
	callback(widget);
	for (PUIWidget w : widget->m_Childreen)
	{
		Foreach(w, callback);
	}
}

void UISystem::ForeachAll(ForeachCallback callback)
{
	Foreach(m_Root, callback);
}

void UISystem::ForeachWithModelMatrix(PUIWidget widget, Matrix4x4& baseMatrix, ForeachCallback callback)
{
	widget->CalcModelMatrix(baseMatrix);
	callback(widget);
	for (PUIWidget w : widget->m_Childreen)
	{
		ForeachWithModelMatrix(w, w->m_ModelMatrix, callback);
	}
}

void UISystem::ForeachAllWithModelMatrix(ForeachCallback callback)
{
	ForeachWithModelMatrix(m_Root, Matrix4x4::Identity(), callback);
}