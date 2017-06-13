#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include"EngineDefine.h"
#include"Math.h"
#include<queue>

using namespace std;

namespace GUISystem{

	typedef void(*OnMouseDown) (Vector2 mousePos);
	typedef void(*OnMouseUp) (Vector2 mousePos);
	typedef void(*OnMouseEnter) (Vector2 mousePos);
	typedef void(*OnMouseExit) (Vector2 mousePos);
	typedef void(*OnMouseMove) (Vector2 mousePos, Vector2 oldMousePos);

	class CWidget
	{
	public:
		bool Overlay(Vector2 pos);
		bool isCollide();
		SRect2D GetRect();
		int GetLayer();
		CWidget* SetCollide(bool isCollide);
		CWidget* SetFillColor(Color fillColor);
		CWidget* SetRect(SRect2D rect);
		CWidget* SetLayer(int layer);

		bool operator>(CWidget* widget);

		bool operator<(CWidget* widget);

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
		priority_queue<CWidget*> widgets;

	public:
		CGUISystem();
		~CGUISystem();
	};

}

#endif