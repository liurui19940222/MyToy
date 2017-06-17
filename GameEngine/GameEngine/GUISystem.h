#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include"EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include<queue>

using namespace std;

namespace GUISystem{

	typedef void(*OnMouseDown) (Vector2 mousePos);
	typedef void(*OnMouseUp) (Vector2 mousePos);
	typedef void(*OnMouseEnter) (Vector2 mousePos);
	typedef void(*OnMouseExit) (Vector2 mousePos);
	typedef void(*OnMouseMove) (Vector2 mousePos, Vector2 oldMousePos);

	class CGUIWidget : public CComponent, public DynamicCreate<CGUIWidget>
	{
	public:

		inline bool Overlay(Vector2 pos);
		inline bool isCollide();
		inline SRect2D GetRect();
		inline int GetLayer();
		inline float GetWidth();
		inline float GetHeight();
		inline CGUIWidget* SetCollide(bool isCollide);
		inline CGUIWidget* SetFillColor(Color fillColor);
		inline CGUIWidget* SetRect(SRect2D rect);
		inline CGUIWidget* SetLayer(int layer);
		inline CGUIWidget* SetWidth(float width);
		inline CGUIWidget* SetHeight(float height);

		inline bool operator>(CGUIWidget* widget);
		inline bool operator<(CGUIWidget* widget);

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

		virtual void OnStart() override;
		virtual void OnDestroy() override;

		virtual void OnUIUpdate();
		virtual void OnUIRender();
		virtual void OnUIDrawDebug();
	};

	class CGUISystem
	{
	private:
		float m_resolutionX, m_resolutionY;
		static CGUISystem* m_instance;
		priority_queue<CGUIWidget*> widgets;

	public:
		static CGUISystem* GetInstance();

		void InitGUI(float resolution_x, float resolution_y);

		void OnUpdate();

		void OnRender();

		void Quit();
	};

}

#endif