#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include<functional>
#include"EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include"PriorityQueue.h"

using namespace std;
using namespace std::tr1;
using namespace container;

namespace guisystem {

	//typedef void(*OnMouseDown) (Vector2 mousePos);
	//typedef void(*OnMouseUp) (Vector2 mousePos);
	//typedef void(*OnMouseEnter) (Vector2 mousePos);
	//typedef void(*OnMouseExit) (Vector2 mousePos);
	//typedef void(*OnMouseOver) (Vector2 mousePos);

	typedef function<void(Vector2)> OnMouseDownEvent;
	typedef function<void(Vector2)> OnMouseUpEvent;
	typedef function<void(Vector2)> OnMouseEnterEvent;
	typedef function<void(Vector2)> OnMouseExitEvent;
	typedef function<void(Vector2)> OnMouseOverEvent;

	enum EWidgetState
	{
		Disabled = -1,
		Normal = 0,
		Hover = 1,
		Pressed = 2,
	};

	enum EWidgetEvent
	{
		MouseDown,
		MouseUp,
		MouseEnter,
		MouseExit,
		MouseOver,
	};

	class CGUIWidget : public CComponent, public DynamicCreate<CGUIWidget>
	{
		friend class CGUISystem;

	public:

		bool Overlay(Vector2 pos);
		bool isCollide();
		SRect2D GetRect();
		int GetLayer();
		float GetWidth();
		float GetHeight();
		EWidgetState GetState();
		bool IsState(EWidgetState state);
		CGUIWidget();
		CGUIWidget* SetCollide(bool isCollide);
		CGUIWidget* SetFill(bool isFill);
		CGUIWidget* SetFillColor(Color fillColor);
		CGUIWidget* SetRect(SRect2D rect);
		CGUIWidget* SetLayer(int layer);
		CGUIWidget* SetWidth(float width);
		CGUIWidget* SetHeight(float height);
		CGUIWidget* SetEnable(bool enable);

		CGUIWidget* AddOnMouseDownListener(OnMouseDownEvent down);
		CGUIWidget* AddOnMouseUpListener(OnMouseUpEvent up);
		CGUIWidget* AddOnMouseEnterListener(OnMouseEnterEvent enter);
		CGUIWidget* AddOnMouseExitListener(OnMouseExitEvent exit);
		CGUIWidget* AddOnMouseOverListener(OnMouseOverEvent over);
		CGUIWidget* RemoveOnMouseDownListener(OnMouseDownEvent down);
		CGUIWidget* RemoveOnMouseUpListener(OnMouseUpEvent up);
		CGUIWidget* RemoveOnMouseEnterListener(OnMouseEnterEvent enter);
		CGUIWidget* RemoveOnMouseExitListener(OnMouseExitEvent exit);
		CGUIWidget* RemoveOnMouseOverListener(OnMouseOverEvent over);
		CGUIWidget* RemoveAllOnMouseDownListener(OnMouseDownEvent down);
		CGUIWidget* RemoveAllOnMouseUpListener(OnMouseUpEvent up);
		CGUIWidget* RemoveAllOnMouseEnterListener(OnMouseEnterEvent enter);
		CGUIWidget* RemoveAllOnMouseExitListener(OnMouseExitEvent exit);
		CGUIWidget* RemoveAllOnMouseOverListener(OnMouseOverEvent over);

		bool operator>(CGUIWidget* widget);
		bool operator<(CGUIWidget* widget);

	protected:
		EWidgetState m_state;
		float m_width;
		float m_height;
		bool m_collide;
		bool m_fill;
		bool m_enable;
		Color m_fillColor;
		SRect2D m_rect;
		int m_layer;

		OnMouseDownEvent onMouseDown;
		OnMouseUpEvent onMouseUp;
		OnMouseEnterEvent onMouseEnter;
		OnMouseExitEvent onMouseExit;
		OnMouseOverEvent onMouseOver;

		virtual void OnUIUpdate();
		virtual void OnUIRender();
		virtual void OnUIDrawDebug();

	private:
		void SetState(EWidgetState state);
		void OnMouseDown(Vector2 mousePos);
		void OnMouseUp(Vector2 mousePos);
		void OnMouseEnter(Vector2 mousePos);
		void OnMouseExit(Vector2 mousePos);
		void OnMouseOver(Vector2 mousePos);

		virtual void OnStart() override;
		virtual void OnDestroy() override;
	};

	class CGUISystem
	{
	private:
		float m_resolutionX, m_resolutionY;
		static CGUISystem* m_instance;
		CPriorityQueue<CGUIWidget*> widgets;
		CGUIWidget* m_curOverlay;
		CGUIWidget* m_lastDown;

	public:
		static CGUISystem* GetInstance();

		void InitGUI(float resolution_x, float resolution_y);

		void AddWidget(CGUIWidget* widget);

		void DestroyWidget(CGUIWidget* widget);

		void OnUpdate();

		void OnRender();

		void OnDrawDebug();

		void Quit();
	};

}

#endif