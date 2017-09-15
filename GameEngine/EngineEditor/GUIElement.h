#ifndef _GUI_ELEMENT_H_
#define _GUI_ELEMENT_H_

#include<vector>
#include<functional>
#include<GameEngine\EngineDefine.h>

using namespace std;
using namespace std::tr1;

typedef function<void(Vector2)> OnMouseDownCallback;
typedef function<void(Vector2)> OnMouseUpCallback;
typedef function<void(Vector2)> OnMouseClickCallback;
typedef function<void(Vector2)> OnMouseEnterCallback;
typedef function<void(Vector2)> OnMouseExitCallback;
typedef function<void(Vector2)> OnMouseOverCallback;

enum ENGINE_API EElementState
{
	Disabled = -1,
	Normal = 0,
	Hover = 1,
	Pressed = 2,
};

enum ENGINE_API EElementEvent
{
	MouseDown,
	MouseUp,
	MouseEnter,
	MouseExit,
	MouseOver,
};

class CGUIElement
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
	CGUIManager* m_manager;
	Vector2 m_position;
	int m_layer;
	float m_width;
	float m_height;
	bool m_collide;
	bool m_fill;
	bool m_enable;
	Color m_fillColor;
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

	virtual void OnStart();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();
public:
	bool Overlay(Vector2 pos);
	bool IsCollide();
	SRect2D GetRect();
	int GetLayer();
	float GetWidth();
	float GetHeight();
	EElementState GetState();
	bool IsState(EElementState state);

	virtual CGUIElement* SetCollide(bool isCollide);
	virtual CGUIElement* SetFill(bool isFill);
	virtual CGUIElement* SetFillColor(Color fillColor);
	virtual CGUIElement* SetRect(SRect2D rect);
	virtual CGUIElement* SetLayer(int layer);
	virtual CGUIElement* SetWidth(float width);
	virtual CGUIElement* SetHeight(float height);
	virtual CGUIElement* SetEnable(bool enable);
	virtual CGUIElement* SetPosition(const Vector2& position);

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

	bool operator>(const CGUIElement& widget) const;
	bool operator<(const CGUIElement& widget) const;
};

#endif