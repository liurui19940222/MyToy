#include "UISystem.h"
#include "SpRendering\MeshFactory.h"
#include "SpCommon\EngineDefine.h"
#include "SpCommon\Debug.h"
#include "UILabel.h"

USING_NAMESPACE_GUI;

#define CHOOSE_TEX(textureId, index) textureId == 0 ? m_SharedTexture : m_ForRenderList[index]->GetTexture()
#define CHOOSE_MAT(materialId, index) materialId == 0 ? m_SharedMaterial : m_ForRenderList[index]->m_Material

UISystem::UISystem()
{
}

UISystem::~UISystem()
{
}

void UISystem::StartUp(IRenderingInterface* ri, int width, int height)
{
	m_RI = ri;
	m_SharedMesh = _MeshFactory->CreateBuffer<MeshBufferUIInstance>(EMeshType::Quad);
	m_SharedMaterial = make_shared<Material>();
	m_SharedMaterial->SetBlendFunc(EBlendFactor::SRC_ALPHA, EBlendFactor::ONE_MINUS_SRC_ALPHA);
	m_SharedMaterial->SetShader(Shader::Get("ui_instance"));
	m_SharedMaterial->SetState(statetype::DepthTest, false);
	m_SharedMaterial->SetState(statetype::Blend, true);
	m_SharedTexture = m_SharedMaterial->GetMainTexture();
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

void UISystem::ShutDown()
{

}

void UISystem::UpdateAll(SMouseState mouseState)
{
	mouseState.m_MousePosition = ScreenPointToView(mouseState.m_MousePosition);
	/*
	* 深度遍历所有View，计算模型矩阵，加入到一个线性表
	*/
	m_ForRenderList.clear();
	m_ForInteractList.clear();
	m_InstanceCount = -1;
	ForeachAllWithModelMatrix([this](PUIWidget widget) {
		if (IS_TYPE(UIView, widget.get()))
			m_ForRenderList.push_back((UIView*)widget.get());
		if (IS_TYPE(IInteractable, widget.get()))
			m_ForInteractList.push_back(dynamic_cast<IInteractable*>(widget.get()));
		m_InstanceCount++;
	});

	int size = (int)((IInteractable*)(UIInteractor*)NULL);

	/*
	* 检测鼠标与UI的交互
	*/
	IInteractable* result = NULL;
	for (auto it = m_ForInteractList.rbegin(); it != m_ForInteractList.rend(); ++it)
	{
		if (!((*it)->IsInteractive()))
			continue;

		if ((*it)->Interact(mouseState))
		{
			if (m_LastIntracted != (*it))
				SECURITY(m_LastIntracted)->LostFocus(mouseState);
			m_LastIntracted = (*it);
			result = (*it);
			break;
		}
	}
	if (!result) SECURITY(m_LastIntracted)->LostFocus(mouseState);
}

void UISystem::RenderAll()
{
	int size = m_ForRenderList.size();

	if (size == 0)
		return;
	m_DrawCalls = 0;
	/*
	* 遍历所有的View，根据使用的Material来组合成不同的批次
	* 按提交给GPU渲染
	*/
	int beginMaterialId = m_ForRenderList[0]->GetMaterialId();
	uint beginTextureId = m_ForRenderList[0]->GetTextureId();
	int beginIndex = 0;
	int materialId = 0;
	uint textureId = 0;
	for (int i = 0; i < size; ++i)
	{
		materialId = m_ForRenderList[i]->GetMaterialId();
		textureId = m_ForRenderList[i]->GetTextureId();
		if (materialId != beginMaterialId || textureId != beginTextureId)
		{
			//如果是字体渲染，先将之前的所有view提交，再提交字体的mesh，然后将begin跳到i+1的位置
			if (IS_TYPE(UILabel, m_ForRenderList[i]))
			{
				SubmitBatch(m_ForRenderList, CHOOSE_MAT(beginMaterialId, beginIndex), CHOOSE_TEX(beginTextureId, beginIndex), beginIndex, i - beginIndex);
				UILabel* label = dynamic_cast<UILabel*>(m_ForRenderList[i]);
				label->BuildInstanceData();
				DrawInstance(label->texcoordRanges(), label->colors(), label->rects(), label->modelMatrices(), label->m_ModelMatrix, label->GetMaterial(), label->GetTexture());
				beginIndex = ++i;
				if (i < size)
				{
					materialId = beginMaterialId = m_ForRenderList[i]->GetMaterialId();
					textureId = beginTextureId = m_ForRenderList[i]->GetTextureId();
				}
			}
			//如果是非字体渲染，将从begin开始到i-1的位置全部提交，重新记录开始点
			else
			{
				SubmitBatch(m_ForRenderList, CHOOSE_MAT(beginMaterialId, beginIndex), CHOOSE_TEX(beginTextureId, beginIndex), beginIndex, i - beginIndex);
				beginIndex = i;
				beginMaterialId = materialId;
				beginTextureId = textureId;
			}
		}
	}
	//如果还有未提交的view，全部提交
	if (beginIndex < size)
	{
		SubmitBatch(m_ForRenderList, CHOOSE_MAT(materialId, size - 1), CHOOSE_TEX(textureId, size - 1), beginIndex, size - beginIndex);
	}
}

void UISystem::SubmitBatch(const vector<UIView*> list, PMaterial mat, PTexture texture, int startingIndex, int count)
{
	m_TexcoordRanges.resize(count);
	m_Colors.resize(count);
	m_RectList.resize(count);
	m_ModelMatrices.resize(count);
	for (int i = 0, j = startingIndex; i < count; ++i, ++j)
	{
		m_TexcoordRanges[i] = list[j]->GetTexcoordRange();
		m_Colors[i] = list[j]->GetColor();
		m_RectList[i] = list[j]->m_Rect;
		m_ModelMatrices[i] = list[j]->m_ModelMatrix;
	}
	m_SharedMesh->MakeInstanceBuffer(m_TexcoordRanges, m_Colors, m_RectList, m_ModelMatrices, count);
	mat->SetMainTexture(texture);
	mat->Bind();
	mat->SetParam("u_M", Matrix4x4::identity);
	mat->SetParam("u_V", m_ViewMatrix);
	mat->SetParam("u_P", m_ProjMatrix);
	m_RI->RenderInstance(RenderingObject{ m_SharedMesh.get(), mat.get() }, count);
	m_DrawCalls++;
}

void UISystem::DrawInstance(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors, vector<SRect2D>& rects,
	vector<Matrix4x4>& modelMatrices, const Matrix4x4& modelMatrix, PMaterial mat, PTexture texture)
{
	size_t size = texcoordRanges.size();
	m_SharedMesh->MakeInstanceBuffer(texcoordRanges, colors, rects, modelMatrices, size);
	mat->SetMainTexture(texture);
	mat->Bind();
	mat->SetParam("u_M", modelMatrix);
	mat->SetParam("u_V", m_ViewMatrix);
	mat->SetParam("u_P", m_ProjMatrix);
	m_RI->RenderInstance(RenderingObject{ m_SharedMesh.get(), mat.get() }, size);
	m_DrawCalls++;
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
		ForeachWithModelMatrix(w, widget->m_ModelMatrix, callback);
	}
}

void UISystem::ForeachAllWithModelMatrix(ForeachCallback callback)
{
	ForeachWithModelMatrix(m_Root, Matrix4x4::Identity(), callback);
}