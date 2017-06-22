#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include"EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include"PriorityQueue.h"

using namespace std;
using namespace container;

namespace guisystem {

	typedef void(*OnMouseDown) (Vector2 mousePos);
	typedef void(*OnMouseUp) (Vector2 mousePos);
	typedef void(*OnMouseEnter) (Vector2 mousePos);
	typedef void(*OnMouseExit) (Vector2 mousePos);
	typedef void(*OnMouseMove) (Vector2 mousePos, Vector2 oldMousePos);

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
		CGUIWidget* SetCollide(bool isCollide);
		CGUIWidget* SetFill(bool isFill);
		CGUIWidget* SetFillColor(Color fillColor);
		CGUIWidget* SetRect(SRect2D rect);
		CGUIWidget* SetLayer(int layer);
		CGUIWidget* SetWidth(float width);
		CGUIWidget* SetHeight(float height);

		bool operator>(CGUIWidget* widget);
		bool operator<(CGUIWidget* widget);

	protected:

		float m_width;
		float m_height;
		bool m_collide;
		bool m_fill;
		Color m_fillColor;
		SRect2D m_rect;
		int m_layer;

		OnMouseDown* onMouseDown;
		OnMouseUp* onMouseUp;
		OnMouseEnter* onMouseEnter;
		OnMouseExit* onMouseExit;
		OnMouseMove* onMouseMove;

		virtual void OnUIUpdate();
		virtual void OnUIRender();
		virtual void OnUIDrawDebug();

	private:
		virtual void OnStart() override;
		virtual void OnDestroy() override;
	};

	class CGUISystem
	{
	private:
		float m_resolutionX, m_resolutionY;
		static CGUISystem* m_instance;
		CPriorityQueue<CGUIWidget*> widgets;

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