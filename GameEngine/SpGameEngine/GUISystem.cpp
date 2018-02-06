#include "GUISystem.h"
#include "Engine.h"
#include "Application.h"
#include"SpCommon\Input.h"
#include "SpCommon\Object.hpp"
#include "GameObject.h"
#include "SpRendering\MeshFactory.h"
using namespace guisystem;
using namespace container;

#pragma region CGUIWidget

IMPL_CLASS(CGUIWidget)

CGUIWidget::CGUIWidget() :
	m_collide(false), m_enable(true), m_fill(false), m_fillColor(Color::white),
	m_height(0), m_width(0), m_layer(0), m_rect(), m_state(EWidgetState::Normal),
	m_pivot(0.5f, 0.5f)
{

}

bool CGUIWidget::Overlay(Vector2 pos)
{
	//因为rect被定义在模型空间，所以要用pos减去世界空间位置
	return m_rect.Overlay(pos - gameObject->GetRealPosition());
}

bool CGUIWidget::IsCollide()
{
	return m_collide;
}

SRect2D CGUIWidget::GetRect()
{
	return m_rect;
}

int CGUIWidget::GetLayer()
{
	return m_layer;
}

float CGUIWidget::GetWidth()
{
	return m_width;
}

float CGUIWidget::GetHeight()
{
	return m_height;
}

EWidgetState CGUIWidget::GetState()
{
	return m_state;
}

bool CGUIWidget::IsState(EWidgetState state)
{
	return m_state == state;
}

Vector2 CGUIWidget::GetPivot()
{
	return m_pivot;
}

Vector3 CGUIWidget::GetAnchorPosition()
{
	return m_anchorPos;
}

CGUIWidget* CGUIWidget::GetParentWidget()
{
	CGUIWidget* widget = NULL;
	if (gameObject->GetParent() && (widget = gameObject->GetParent()->GetComponent<CGUIWidget>()))
		return widget;
	return NULL;
}

CGUIWidget* CGUIWidget::SetCollide(bool isCollide)
{
	m_collide = isCollide;
	return this;
}

CGUIWidget* CGUIWidget::SetFill(bool isFill)
{
	m_fill = isFill;
	return this;
}

CGUIWidget* CGUIWidget::SetFillColor(Color fillColor)
{
	this->m_fillColor = fillColor;
	return this;
}

CGUIWidget* CGUIWidget::SetRect(SRect2D rect)
{
	this->m_rect = rect;
	this->m_width = rect.halfSize.x * 2;
	this->m_height = rect.halfSize.y * 2;
	UpdateVertices();
	return this;
}

CGUIWidget* CGUIWidget::SetAlignment(EAlignment alignment)
{
	this->m_alignment = alignment;
	this->m_alignment_h = _GetHorizontal(alignment);
	this->m_alignment_v = _GetVertical(alignment);
	RefreshAnchor();
	return this;
}

CGUIWidget* CGUIWidget::SetLayer(int layer)
{
	this->m_layer = layer;
	_GUISystem->UpdateWidgetLayer(this);
	return this;
}

CGUIWidget* CGUIWidget::SetWidth(float width)
{
	this->m_width = width;
	this->m_rect.halfSize.x = width * 0.5f;
	UpdateVertices();
	return this;
}

CGUIWidget* CGUIWidget::SetHeight(float height)
{
	this->m_height = height;
	this->m_rect.halfSize.y = height * 0.5f;
	UpdateVertices();
	return this;
}

CGUIWidget* CGUIWidget::SetEnable(bool enable)
{
	m_enable = enable;
	return this;
}

CGUIWidget* CGUIWidget::SetPivot(Vector2 pivot)
{
	m_pivot = pivot;
	RefreshAnchor();
	return this;
}

CGUIWidget* CGUIWidget::SetAnchorPosition(Vector3 anchorPos)
{
	m_anchorPos = anchorPos;
	Vector3 offset;
	offset.x = m_width * (0.5f - m_pivot.x);
	offset.y = m_height * (0.5f - m_pivot.y);
	gameObject->SetLocalPosition(GetCenterPositionInParent());
	gameObject->SetLocalPosition(anchorPos + offset);
	return this;
}

CGUIWidget* CGUIWidget::RefreshAnchor()
{
	SetAnchorPosition(m_anchorPos);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseDownListener(OnMouseDownEvent down)
{
	onMouseDown.push_back(down);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseUpListener(OnMouseUpEvent up)
{
	onMouseUp.push_back(up);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseClickListener(OnMouseClickEvent click)
{
	onMouseClick.push_back(click);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseEnterListener(OnMouseEnterEvent enter)
{
	onMouseEnter.push_back(enter);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseExitListener(OnMouseExitEvent exit)
{
	onMouseExit.push_back(exit);
	return this;
}

CGUIWidget* CGUIWidget::AddOnMouseOverListener(OnMouseOverEvent over)
{
	onMouseOver.push_back(over);
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseDownListener()
{
	onMouseDown.clear();
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseUpListener()
{
	onMouseUp.clear();
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseClickListener()
{
	onMouseClick.clear();
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseEnterListener()
{
	onMouseEnter.clear();
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseExitListener()
{
	onMouseExit.clear();
	return this;
}

CGUIWidget* CGUIWidget::RemoveAllOnMouseOverListener()
{
	onMouseOver.clear();
	return this;
}

bool CGUIWidget::operator>(CGUIWidget* widget) const
{
	return m_layer > widget->m_layer;
}

bool CGUIWidget::operator<(CGUIWidget* widget) const
{
	return m_layer < widget->m_layer;
}

Vector3 CGUIWidget::GetCenterPositionInParent()
{
	Vector2 parentSize = GetParentSize() * 0.5f;
	Vector3 pos;

	if (GetParentWidget())
	{
		if (m_alignment_h == EAlignmentHorizontal::LEFT)
			pos.x = -parentSize.x;
		else if (m_alignment_h == EAlignmentHorizontal::CENTER)
			pos.x = 0;
		else if (m_alignment_h == EAlignmentHorizontal::RIGHT)
			pos.x = parentSize.x;

		if (m_alignment_v == EAlignmentVertical::TOP)
			pos.y = parentSize.y;
		else if (m_alignment_v == EAlignmentVertical::MIDDLE)
			pos.y = 0;
		else if (m_alignment_v == EAlignmentVertical::BOTTOM)
			pos.y = -parentSize.y;

		pos += gameObject->GetParent()->GetLocalPosition();
	}
	else
	{
		if (m_alignment_h == EAlignmentHorizontal::LEFT)
			pos.x = 0;
		else if (m_alignment_h == EAlignmentHorizontal::CENTER)
			pos.x = parentSize.x;
		else if (m_alignment_h == EAlignmentHorizontal::RIGHT)
			pos.x = parentSize.x * 2;

		if (m_alignment_v == EAlignmentVertical::TOP)
			pos.y = parentSize.y * 2;
		else if (m_alignment_v == EAlignmentVertical::MIDDLE)
			pos.y = parentSize.y;
		else if (m_alignment_v == EAlignmentVertical::BOTTOM)
			pos.y = 0;
	}

	return pos;
}

Vector2 CGUIWidget::GetParentSize()
{
	CGUIWidget* parent = NULL;
	if ((parent = GetParentWidget()))
	{
		return Vector2(parent->m_width, parent->m_height);
	}
	return Vector2(_GUISystem->GetResolutionX(), _GUISystem->GetResolutionY());
}

void CGUIWidget::SetState(EWidgetState state)
{
	m_state = state;
}

void CGUIWidget::OnMouseDown(Vector2 mousePos)
{
	if (onMouseDown.size() > 0)
		for (auto it = onMouseDown.begin(); it != onMouseDown.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnMouseUp(Vector2 mousePos)
{
	if (onMouseUp.size() > 0)
		for (auto it = onMouseUp.begin(); it != onMouseUp.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnMouseClick(Vector2 mousePos)
{
	if (onMouseClick.size() > 0)
		for (auto it = onMouseClick.begin(); it != onMouseClick.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnMouseEnter(Vector2 mousePos)
{
	if (onMouseEnter.size() > 0)
		for (auto it = onMouseEnter.begin(); it != onMouseEnter.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnMouseExit(Vector2 mousePos)
{
	if (onMouseExit.size() > 0)
		for (auto it = onMouseExit.begin(); it != onMouseExit.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnMouseOver(Vector2 mousePos)
{
	if (onMouseOver.size() > 0)
		for (auto it = onMouseOver.begin(); it != onMouseOver.end(); ++it)
			(*it)(mousePos);
}

void CGUIWidget::OnStart()
{
	_GUISystem->AddWidget(this);
	gameObject->SetLayer(Layer::Overlay2D);
	SetAlignment(EAlignment::CENTER_MIDDLE);
	SetAnchorPosition(Vector3::zero);
	m_buffer.MakeBuffer(_MeshFactory->SharedMesh(EMeshType::Quad));
	m_material = _Maker->Instantiate<Material>();
	m_material->SetShader(Shader::Get("gui"))->SetState(statetype::DepthTest, false);
}

void CGUIWidget::OnDestroy()
{
	_GUISystem->DestroyWidget(this);
	m_buffer.ReleaseBuffer();
	_Maker->Destroy(m_material);
}

void CGUIWidget::OnUIUpdate()
{

}

void CGUIWidget::UpdateVertices()
{
	m_vertices[0].x = -m_rect.halfSize.x; m_vertices[0].y = m_rect.halfSize.y; m_vertices[0].z = 0;
	m_vertices[1].x = -m_rect.halfSize.x; m_vertices[1].y = -m_rect.halfSize.y; m_vertices[1].z = 0;
	m_vertices[2].x = m_rect.halfSize.x; m_vertices[2].y = -m_rect.halfSize.y; m_vertices[2].z = 0;
	m_vertices[3].x = m_rect.halfSize.x; m_vertices[3].y = -m_rect.halfSize.y; m_vertices[3].z = 0;
	m_vertices[4].x = m_rect.halfSize.x; m_vertices[4].y = m_rect.halfSize.y; m_vertices[4].z = 0;
	m_vertices[5].x = -m_rect.halfSize.x; m_vertices[5].y = m_rect.halfSize.y; m_vertices[5].z = 0;
}

void CGUIWidget::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (m_fill)
	{
		m_buffer.UpdateVertices(m_vertices, 0, 6);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_state == EWidgetState::Normal)
		{
			m_fillColor = Color::white;
			m_addColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (m_state == EWidgetState::Hover)
		{
			m_addColor = Color(0.1f, 0.1f, 0.1f, 0.1f);
		}
		else if (m_state == EWidgetState::Pressed)
		{
			m_addColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
			m_fillColor = Color(0.8f, 0.8f, 0.8f, 1.0f);
		}
		else if (m_state == EWidgetState::Disabled)
		{
			m_fillColor = Color::grey;
			m_addColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		}

		m_material->Bind();
		m_material->SetParam("AddColor", m_addColor);
		m_material->SetParam("M", modelMatrix);
		m_material->SetParam("V", viewMatrix);
		m_material->SetParam("P", projectionMatrix);
		m_buffer.BindBuffer();
		glDrawArrays(GL_TRIANGLES, 0, m_buffer.GetVertexNum());
		m_material->Unbind();

		glDisable(GL_BLEND);
	}
}

void CGUIWidget::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawRect(m_rect, modelMatrix);
}

#pragma endregion

#pragma region CGUISystem

void CGUISystem::InitGUI(float resolution_x, float resolution_y)
{
	SetResolution(resolution_x, resolution_y);
}

void CGUISystem::AddWidget(CGUIWidget* widget)
{
	widget->gameObject->SetParent(m_uiRoot);
}

void CGUISystem::UpdateWidgetLayer(CGUIWidget* widget)
{

}

void CGUISystem::DestroyWidget(CGUIWidget* dwidget)
{

}

void CGUISystem::ForeachWidght(WidghtForeachCallback callback)
{
	if (!m_uiRoot) return;
	_Maker->ForeachGameObject(m_uiRoot, [&callback](CGameObject* go, int depth) {
		CGUIWidget* widght = NULL;
		if (widght = go->GetComponent<CGUIWidget>())
		{
			callback(widght);
		}
	});
}

void CGUISystem::ForeachWidghtR(WidghtForeachCallbackR callback)
{
	if (!m_uiRoot) return;
	_Maker->ForeachGameObject(m_uiRoot, [&callback](CGameObject* go, int depth) {
		CGUIWidget* widght = NULL;
		if (widght = go->GetComponent<CGUIWidget>())
		{
			if (callback(widght))
				return;
		}
	});
}

void CGUISystem::InverseForeachWidght(WidghtForeachCallback callback)
{
	vector<CGUIWidget*> list;
	ForeachWidght([&list](CGUIWidget* widght) {
		list.push_back(widght);
	});
	for (int i = list.size() - 1; i >= 0; --i)
	{
		callback(list[i]);
	}
}

void CGUISystem::InverseForeachWidghtR(WidghtForeachCallbackR callback)
{
	vector<CGUIWidget*> list;
	ForeachWidght([&list](CGUIWidget* widght) {
		list.push_back(widght);
	});
	for (int i = list.size() - 1; i >= 0; --i)
	{
		if (callback(list[i]))
			break;
	}
}

void CGUISystem::SetResolution(float resolution_x, float resolution_y)
{
	m_resolutionX = resolution_x;
	m_resolutionY = resolution_y;
	m_centerPos.x = resolution_x * 0.5f;
	m_centerPos.y = resolution_y * 0.5f;
	ForeachWidght([](CGUIWidget* widget) {
		widget->RefreshAnchor();
	});
}

float CGUISystem::GetResolutionX()
{
	return m_resolutionX;
}

float CGUISystem::GetResolutionY()
{
	return m_resolutionY;
}

void CGUISystem::OnUpdate()
{
	if (!_MainCamera) return;
	Vector2 mousePos = CInput::InputMousePosition();
	mousePos.y = _SCH - mousePos.y;
	mousePos = _MainCamera->ScreenPosToViewPort(mousePos, _SCW, _SCH);
	ForeachWidghtR([this, &mousePos](CGUIWidget* widget) {
		if (!widget->IsState(EWidgetState::Disabled) && widget->IsCollide())
		{
			if (widget->Overlay(mousePos))
			{
				if (widget != m_curOverlay)
				{
					if (m_curOverlay)
					{
						m_curOverlay->SetState(EWidgetState::Normal);
						m_curOverlay->OnMouseExit(mousePos);
					}
					widget->OnMouseEnter(mousePos);
				}

				if (CInput::GetMouseDown(EMouseKey::Left))
				{
					widget->SetState(EWidgetState::Pressed);
					widget->OnMouseDown(mousePos);
					m_lastDown = widget;
				}
				else if (CInput::GetMouseUp(EMouseKey::Left))
				{
					if (widget->IsState(EWidgetState::Pressed))
					{
						widget->SetState(EWidgetState::Hover);
						widget->OnMouseUp(mousePos);
						widget->OnMouseClick(mousePos);
					}
				}
				else if (!widget->IsState(EWidgetState::Pressed))
				{
					widget->SetState(EWidgetState::Hover);
					if (widget->m_lastOverPos != mousePos)
					{
						widget->OnMouseOver(mousePos);
						widget->m_lastOverPos = mousePos;
					}
				}

				m_curOverlay = widget;
				return false;
			}
			else if (widget->IsState(EWidgetState::Hover) || widget->IsState(EWidgetState::Pressed))
			{
				widget->SetState(EWidgetState::Normal);
				widget->OnMouseExit(mousePos);
				if (widget == m_curOverlay)
					m_curOverlay = NULL;
			}
		}
		return true;
	});
	ForeachWidght([](CGUIWidget* widget) {
		widget->OnUIUpdate();
	});
}

void CGUISystem::Quit()
{

}

Vector3 CGUISystem::GetCenterPosition()
{
	return m_centerPos;
}

#pragma endregion