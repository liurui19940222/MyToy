#ifndef _GUI_ELEMENT_H_
#define _GUI_ELEMENT_H_

#include<vector>
#include<functional>
#include<SpGameEngine\EngineDefine.h>
#include"GridLayout.h"

using namespace std;
using namespace std::tr1;

typedef function<void(Vector2)> OnMouseDownCallback;
typedef function<void(Vector2)> OnMouseUpCallback;
typedef function<void(Vector2)> OnMouseClickCallback;
typedef function<void(Vector2)> OnMouseEnterCallback;
typedef function<void(Vector2)> OnMouseExitCallback;
typedef function<void(Vector2)> OnMouseOverCallback;

enum EElementState
{
	Disabled = -1,
	Normal = 0,
	Hover = 1,
	Pressed = 2,
};

enum EElementEvent
{
	MouseDown,
	MouseUp,
	MouseEnter,
	MouseExit,
	MouseOver,
};

class CGUIElement : public CGridLayoutElement
{
	friend class CGUIManager;
private:
	void SetState(EElementState state);
	void OnMouseDown(Vector2 mousePos);
	void OnMouseUp(Vector2 mousePos);
	void OnMouseClick(Vector2 mousePos);
	void OnMouseEnter(Vector2 mousePos);
	void OnMouseExit(Vector2 mousePos);
	void OnMouseOver(Vector2 mousePos);
protected:
	CGUIElement();
	virtual ~CGUIElement();
	CGUIManager* m_manager;
	CGUIElement* m_parent;
	Vector2 m_position;
	int m_layer;
	int m_depth;
	float m_width;
	float m_height;
	bool m_collide;
	bool m_fill;
	bool m_enable;
	bool m_haveFocus;
	Color m_fillColor;
	Color m_addColor;
	SRect2D m_rect;
	EElementState m_state;
	Vector2 m_lastOverPos;
	Vector2 m_pivot;
	EAlignment m_alignment;
	EAlignmentHorizontal m_alignment_h;
	EAlignmentVertical m_alignment_v;

	vector<OnMouseDownCallback> onMouseDown;
	vector<OnMouseUpCallback> onMouseUp;
	vector<OnMouseClickCallback> onMouseClick;
	vector<OnMouseEnterCallback> onMouseEnter;
	vector<OnMouseExitCallback> onMouseExit;
	vector<OnMouseOverCallback> onMouseOver;

	virtual void OnGotFocus();
	virtual void OnLostFocus();
	virtual void OnStart();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();
public:
	virtual bool Overlay(Vector2 pos);
	bool IsCollide();
	bool IsEnalbe();
	SRect2D GetRect();
	int GetLayer();
	int GetDepth();
	float GetWidth();
	float GetHeight();
	EElementState GetState();
	bool IsState(EElementState state);
	bool Visible();
	void EnableClip(bool clip);

	virtual CGUIElement* SetCollide(bool isCollide);
	virtual CGUIElement* SetFill(bool isFill);
	virtual CGUIElement* SetFillColor(Color fillColor);
	virtual CGUIElement* SetRect(SRect2D rect);
	virtual CGUIElement* SetLayer(int layer);
	virtual CGUIElement* SetWidth(float width);
	virtual CGUIElement* SetHeight(float height);
	virtual CGUIElement* SetEnable(bool enable);
	virtual CGUIElement* SetPosition(const Vector2& position);
	virtual CGUIElement* SetDepth(int depth);
	virtual CGUIElement* SetParent(CGUIElement* parent);

	virtual void OnStateChanged();
	virtual void OnLayoutChanged(const SRect2D& rect) override;
	virtual void OnVisibleChanged(bool visible) override;

	CGUIElement* AddOnMouseDownListener(OnMouseDownCallback down);
	CGUIElement* AddOnMouseUpListener(OnMouseUpCallback up);
	CGUIElement* AddOnMouseClickListener(OnMouseClickCallback enter);
	CGUIElement* AddOnMouseEnterListener(OnMouseEnterCallback enter);
	CGUIElement* AddOnMouseExitListener(OnMouseExitCallback exit);
	CGUIElement* AddOnMouseOverListener(OnMouseOverCallback over);
	CGUIElement* RemoveAllOnMouseDownListener();
	CGUIElement* RemoveAllOnMouseUpListener();
	CGUIElement* RemoveAllOnMouseClickListener();
	CGUIElement* RemoveAllOnMouseEnterListener();
	CGUIElement* RemoveAllOnMouseExitListener();
	CGUIElement* RemoveAllOnMouseOverListener();
	inline CGUIManager* GetManager() { return m_manager; }

	bool operator>(const CGUIElement& widget) const;
	bool operator<(const CGUIElement& widget) const;
};

#endif