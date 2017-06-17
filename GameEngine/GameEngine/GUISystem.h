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

	class CGUIWidget
	{
	public:
		bool Overlay(Vector2 pos);
		bool isCollide();
		SRect2D GetRect();
		int GetLayer();
		CGUIWidget* SetCollide(bool isCollide);
		CGUIWidget* SetFillColor(Color fillColor);
		CGUIWidget* SetRect(SRect2D rect);
		CGUIWidget* SetLayer(int layer);

		bool operator>(CGUIWidget* widget);

		bool operator<(CGUIWidget* widget);

	protected:
		bool collide;
		bool fill;
		Color fillColor;
		SRect2D rect;
		int layer;

		OnMouseDown* onMouseDown;
		OnMouseUp* onMouseUp;
		OnMouseEnter* onMouseEnter;
		OnMouseExit* onMouseExit;
		OnMouseMove* onMouseMove;
	};

	class CGUISystem
	{
	private:
		static CGUISystem* instance;
		priority_queue<CGUIWidget*> widgets;

	public:

	};

}

#endif