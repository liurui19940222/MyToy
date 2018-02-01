#include "UIWidget.h"
#include "UISystem.h"

USING_NAMESPACE_GUI;

UIWidget::UIWidget() : m_Rect(Vector2::zero, Vector2(50.0f, 50.0f)), m_Parent(NULL), m_EulerAngles(Vector3::zero),
m_Pivot(Vector2(0.5f, 0.5f)), m_LocalScale(Vector2::one), m_Alignment(EAlignment::CENTER_MIDDLE) {}

UIWidget::~UIWidget()
{
}

bool UIWidget::Overlay(const Vector2& point)
{
	SRect2D rect = m_Rect;
	rect.center = ExtractWorldPosition();
	return rect.Overlay(point);
}

UIWidget* UIWidget::SetWidth(float width)
{
	this->m_Width = width;
	this->m_Rect.halfSize.x = width * 0.5f;
	return this;
}

UIWidget* UIWidget::SetHeight(float height)
{
	this->m_Height = height;
	this->m_Rect.halfSize.y = height * 0.5f;
	return this;
}

UIWidget* UIWidget::SetRect(const SRect2D& rect)
{
	this->m_Rect = rect;
	this->m_Width = rect.halfSize.x * 2;
	this->m_Height = rect.halfSize.y * 2;
	return this;
}

UIWidget* UIWidget::SetLocalPosition(const Vector2& pos)
{
	this->m_Rect.center = pos;
	return this;
}

UIWidget* UIWidget::AddChild(PUIWidget widget)
{
	m_Childreen.push_back(widget);
	widget->m_Parent = this;
	return this;
}

UIWidget* UIWidget::SetAlignment(EAlignment alignment)
{
	m_Alignment = alignment;
	return this;
}

UIWidget* UIWidget::RemoveChild(PUIWidget widget)
{
	if (widget.get())
	{
		for (auto it = m_Childreen.begin(); it != m_Childreen.end(); ++it)
		{
			if (it->get() == widget.get())
			{
				m_Childreen.erase(it);
				break;
			}
		}
	}
	m_System->AddChild(widget);
	return this;
}

void UIWidget::CalcModelMatrix(Matrix4x4& baseMatrix)
{
	static Matrix4x4 translate, scale, rotate;
	translate.MakeTranslate(m_Rect.center + GetOffset());
	scale.MakeScale(m_LocalScale);
	rotate.MakeRotate(m_EulerAngles);
	m_ModelMatrix = baseMatrix * translate * rotate * scale;
}

Vector2 UIWidget::GetOffset() const
{
	Vector2 selfOffset = -m_Rect.halfSize * 2 * (m_Pivot - Vector2(0.5f, 0.5f));
	Vector2 baseOffset(Vector2::zero);
	EAlignmentHorizontal horizontal = _GetHorizontal(m_Alignment);
	EAlignmentVertical vertical = _GetVertical(m_Alignment);
	if (horizontal == EAlignmentHorizontal::LEFT)
	{
		baseOffset.x -= m_Parent->m_Rect.halfSize.x;
	}
	else if (horizontal == EAlignmentHorizontal::RIGHT)
	{
		baseOffset.x += m_Parent->m_Rect.halfSize.x;
	}
	if (vertical == EAlignmentVertical::TOP)
	{
		baseOffset.y += m_Parent->m_Rect.halfSize.y;
	}
	else if (vertical == EAlignmentVertical::BOTTOM)
	{
		baseOffset.y -= m_Parent->m_Rect.halfSize.y;
	}
	return selfOffset + baseOffset;
}

 

