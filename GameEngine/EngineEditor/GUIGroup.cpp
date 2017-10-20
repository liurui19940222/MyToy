#include"GUIGroup.h"
#include"GUIManager.h"
#include<GameEngine\DynamicFactory.h>

CGUIGroup::CGUIGroup() : CGUIElement(), m_hasIcon(false), m_opened(false), m_selected(false) {}

void CGUIGroup::OnStart()
{
	m_guiText = m_manager->Create<CGUIText>();
	m_guiTexture = m_manager->Create<CGUITexture>();
	m_guiText->SetAlignment(EAlignment::LEFT_MIDDLE)->SetFontSize(13)->SetWidth(300)->SetHeight(50);
	m_guiText->SetLayer(1);
	m_guiTexture->SetLayer(1);
	SetFillColor(Color(0.2f, 0.2f, 0.6f, 1.0f));
	SetCollide(true);
	static Vector3 t1[3] = { Vector3(0.5f, 0.5f, 0.0f), Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, -0.5f, 0.0f) };
	m_guiMesh = m_manager->Create<CGUIMesh>();
	m_guiMesh->SetVertices(t1, 3)->SetMeshColor(Color::white)->SetScale(Vector3::one * 10)->SetWidth(10)->SetHeight(10)->SetCollide(true)->AddOnMouseClickListener([this](Vector2 m) {
		this->SetOpened(!this->IsOpened());
		this->GetManager()->UpdateLayout();
		this->SetSelected(true);
	});
	m_guiMesh->SetLayer(1);
	m_guiMesh->SetEnable(false);
	SetOpened(false);
}

void CGUIGroup::OnDestroy()
{
	m_manager->Destroy(m_guiText);
	m_manager->Destroy(m_guiTexture);
	m_manager->Destroy(m_guiMesh);
	for (CGUIElement* element : m_members)
	{
		m_manager->Destroy(element);
	}
	m_manager->DeleteRow(m_cell->m_rowIndex, false);
	CGUIElement::OnDestroy();
}

void CGUIGroup::OnUpdate()
{
	CGUIElement::OnUpdate();
}

void CGUIGroup::OnRender()
{
	CRawRenderer* renderer = m_manager->GetRenderer();
	CGUIElement::OnRender();
	if (m_fill)
	{
		renderer->SetRenderMode(ERenderMode::Wireframe);
		renderer->DrawRect(m_rect, Color::orange);
		renderer->SetRenderMode(ERenderMode::Fill);
	}
}

void CGUIGroup::OnStateChanged()
{
	switch (m_state)
	{
	case EElementState::Pressed:
		SetSelected(true);
		break;
	}
}

void CGUIGroup::OnLayoutChanged(const SRect2D& rect)
{
	CGUIElement::OnLayoutChanged(rect);
	float x = m_position.x + m_depth * DEPTH_STEP;
	m_arrowPos = Vector2(x - m_rect.half_size_x + ARROW_SIZE * 0.5f, m_position.y);
	m_arrowRect = SRect2D(m_arrowPos.x, m_arrowPos.y, ARROW_SIZE * 0.5f, ARROW_SIZE * 0.5f);

	m_guiMesh->SetEnable(m_cell->m_visible && m_members.size() > 0);
	m_guiText->SetEnable(m_cell->m_visible);
	m_guiTexture->SetEnable(m_cell->m_visible);

	if (m_hasIcon)
	{
		m_guiMesh->SetRect(m_arrowRect);
		m_guiText->SetWidth(m_rect.half_size_x * 2)->SetHeight(m_rect.half_size_y * 2);
		m_guiText->SetPosition(Vector2(ARROW_SIZE * 0.5f + 30 + x, m_position.y));
		m_guiTexture->SetPosition(Vector2(ARROW_SIZE * 0.5f + 10 + m_arrowPos.x, m_arrowPos.y));
	}
	else
	{
		m_guiMesh->SetRect(m_arrowRect);
		m_guiText->SetWidth(m_rect.half_size_x * 2)->SetHeight(m_rect.half_size_y * 2);
		m_guiText->SetPosition(Vector2(ARROW_SIZE * 0.5f + 10 + x, m_position.y));
	}
}

void CGUIGroup::OnVisibleChanged(bool visible)
{
	CGUIElement::OnVisibleChanged(visible);
	if (visible)
	{
		if (m_opened)
		{
			for (CGUIElement* element : m_members)
			{
				m_manager->SetRowsVisible(element->m_cell->m_rowIndex, 1, true);
			}
		}
	}
	else
	{
		for (CGUIElement* element : m_members)
		{
			m_manager->SetRowsVisible(element->m_cell->m_rowIndex, 1, false);
		}
	}
}

bool CGUIGroup::Overlay(Vector2 pos)
{
	return CGUIElement::Overlay(pos);
}

void CGUIGroup::OnLostFocus()
{
	SetSelected(false);
}

CGUIGroup* CGUIGroup::SetOpened(bool opened)
{
	m_opened = opened;
	if (m_opened)
	{
		m_guiMesh->SetEulerAngles(Vector3(0, 0, 0));
	}
	else
	{
		m_guiMesh->SetEulerAngles(Vector3(0, 0, 90));
	}
	UpdateVisible();
	return this;
}

void CGUIGroup::UpdateVisible()
{
	if (m_cell)
	{
		for (int i = 0; i < m_members.size(); ++i)
		{
			m_manager->SetRowsVisible(m_members[i]->m_cell->m_rowIndex, 1, m_opened);
		}
	}
}

CGUIGroup* CGUIGroup::SetSelected(bool selected)
{
	if (!m_canSelect) return this;
	m_selected = selected;
	m_fill = m_selected;
	return this;
}

CGUIGroup* CGUIGroup::SetIcon(CTexture* texture)
{
	if (texture != NULL) m_hasIcon = true;
	m_guiTexture->SetTexture(texture);
	m_guiTexture->SetRawSize();
	return this;
}

CGUIGroup* CGUIGroup::SetText(const wstring& text)
{
	m_guiText->SetText(text);
	return this;
}

CGUIGroup* CGUIGroup::SetCanSelect(bool can)
{
	m_canSelect = can;
	return this;
}

bool CGUIGroup::IsOpened()
{
	return m_opened;
}

bool CGUIGroup::IsSelected()
{
	return m_selected;
}

void CGUIGroup::AddMember(CGUIElement* element)
{
	if (element == NULL) return;
	element->SetParent(this);
	m_manager->PutIntoGrid(GetBottomMostRowIndex(), 0, element, true);
	m_manager->SetRowsVisible(element->m_cell->m_rowIndex, 1, m_opened);
	element->SetDepth(this->m_depth + 1);
	m_manager->UpdateLayout();
	m_guiMesh->SetEnable(true);
	m_members.push_back(element);
}

void CGUIGroup::RemoveMember(CGUIElement* element)
{
	for (int i = 0; i < m_members.size(); ++i)
	{
		if (m_members[i] == element)
		{
			m_members.erase(m_members.begin() + i);
			return;
		}
	}
}

void CGUIGroup::RemoveInParent()
{
	if (m_parent && IS_TYPE(CGUIGroup, this))
	{
		((CGUIGroup*)m_parent)->RemoveMember(this);
	}
}

int CGUIGroup::GetBottomMostRowIndex()
{
	int result = GetBottomMostRowIndex(this);
	if (result == -1)
		result = m_cell->m_rowIndex + 1;
	return result;
}

int CGUIGroup::GetBottomMostRowIndex(CGUIGroup* group)
{
	if (group->m_members.size() > 0)
	{
		CGUIElement* element = group->m_members[group->m_members.size() - 1];
		if (IS_TYPE(CGUIGroup, element))
		{
			return GetBottomMostRowIndex((CGUIGroup*)element);
		}
		else
		{
			return element->m_cell->m_rowIndex;
		}
	}
	return group->m_cell ? group->m_cell->m_rowIndex + 1 : -1;
}

CGUIElement* CGUIGroup::operator[](int index)
{
	return m_members[index];
}

CGUIElement* CGUIGroup::GetMember(int index)
{
	return m_members[index];
}