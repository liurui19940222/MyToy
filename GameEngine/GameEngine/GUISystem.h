#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include<vector>
#include<functional>
#include"EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include"PriorityQueue.h"

using namespace std;
using namespace std::tr1;
using namespace container;

namespace guisystem {

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
		Vector2 GetPivot();
		Vector3 GetAnchorPosition();
		CGUIWidget();
		CGUIWidget* GetParentWidget();
		CGUIWidget* SetCollide(bool isCollide);
		CGUIWidget* SetFill(bool isFill);
		CGUIWidget* SetFillColor(Color fillColor);
		CGUIWidget* SetRect(SRect2D rect);
		CGUIWidget* SetAlignment(EAlignment alignment);
		CGUIWidget* SetLayer(int layer);
		CGUIWidget* SetWidth(float width);
		CGUIWidget* SetHeight(float height);
		CGUIWidget* SetEnable(bool enable);
		CGUIWidget* SetPivot(Vector2 pivot);
		CGUIWidget* SetAnchorPosition(Vector3 anchorPos);

		CGUIWidget* AddOnMouseDownListener(OnMouseDownEvent down);
		CGUIWidget* AddOnMouseUpListener(OnMouseUpEvent up);
		CGUIWidget* AddOnMouseEnterListener(OnMouseEnterEvent enter);
		CGUIWidget* AddOnMouseExitListener(OnMouseExitEvent exit);
		CGUIWidget* AddOnMouseOverListener(OnMouseOverEvent over);
		CGUIWidget* RemoveAllOnMouseDownListener();
		CGUIWidget* RemoveAllOnMouseUpListener();
		CGUIWidget* RemoveAllOnMouseEnterListener();
		CGUIWidget* RemoveAllOnMouseExitListener();
		CGUIWidget* RemoveAllOnMouseOverListener();

		bool operator>(CGUIWidget* widget) const;
		bool operator<(CGUIWidget* widget) const;

	protected:
		int m_layer;
		float m_width;
		float m_height;
		bool m_collide;
		bool m_fill;
		bool m_enable;
		Color m_fillColor;
		SRect2D m_rect;
		EWidgetState m_state;
		Vector2 m_lastOverPos;
		Vector3 m_anchorPos;
		Vector2 m_pivot;
		EAlignment m_alignment;
		EAlignmentHorizontal m_alignment_h;
		EAlignmentVertical m_alignment_v;

		vector<OnMouseDownEvent> onMouseDown;
		vector<OnMouseUpEvent> onMouseUp;
		vector<OnMouseEnterEvent> onMouseEnter;
		vector<OnMouseExitEvent> onMouseExit;
		vector<OnMouseOverEvent> onMouseOver;

		virtual void OnUIUpdate();
		virtual void OnUIRender();
		virtual void OnUIDrawDebug();

	private:
		Vector3 GetCenterPositionInParent();
		Vector2 GetParentSize();
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
		Vector3 m_centerPos;

	public:
		static CGUISystem* GetInstance();

		void InitGUI(float resolution_x, float resolution_y);

		void AddWidget(CGUIWidget* widget);

		void UpdateWidgetLayer(CGUIWidget* widget);

		void DestroyWidget(CGUIWidget* widget);

		void SetResolution(float resolution_x, float resolution_y);

		float GetResolutionX();

		float GetResolutionY();

		Vector3 GetCenterPosition();

		void OnUpdate();

		void OnRender();

		void OnDrawDebug();

		void Quit();

	};

}

#endif