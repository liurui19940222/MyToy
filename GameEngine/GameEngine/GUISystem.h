#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include<vector>
#include<functional>
#include"EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include"PriorityQueue.h"
#include"Singleton.h"
#include"Renderer.h"
#include"MeshBuffer.h"
#include"Material.h"

#define _GUISystem guisystem::CGUISystem::GetInstance()

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

	class CGUIWidget : public CComponent, public IRenderer
	{
		REFLECT_CLASS(CGUIWidget)
		friend class CGUISystem;

	public:
		virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
		virtual void RenderDebug(Matrix4x4& modelMatrix) override;

		bool Overlay(Vector2 pos);
		bool IsCollide();
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
		virtual CGUIWidget* SetCollide(bool isCollide);
		virtual CGUIWidget* SetFill(bool isFill);
		virtual CGUIWidget* SetFillColor(Color fillColor);
		virtual CGUIWidget* SetRect(SRect2D rect);
		virtual CGUIWidget* SetAlignment(EAlignment alignment);
		virtual CGUIWidget* SetLayer(int layer);
		virtual CGUIWidget* SetWidth(float width);
		virtual CGUIWidget* SetHeight(float height);
		virtual CGUIWidget* SetEnable(bool enable);
		virtual CGUIWidget* SetPivot(Vector2 pivot);
		virtual CGUIWidget* SetAnchorPosition(Vector3 anchorPos);
		virtual CGUIWidget* RefreshAnchor();

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
		Vector3 m_vertices[6];
		int m_layer;
		float m_width;
		float m_height;
		bool m_collide;
		bool m_fill;
		bool m_enable;
		CMeshBuffer m_buffer;
		CMaterial* m_material;
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
		void UpdateVertices();

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

	class CGUISystem : public CSingleton<CGUISystem>
	{
	private:
		float m_resolutionX, m_resolutionY;
		CPriorityQueue<CGUIWidget*> widgets;
		CGUIWidget* m_curOverlay;
		CGUIWidget* m_lastDown;
		Vector3 m_centerPos;

	public:
		void InitGUI(float resolution_x, float resolution_y);

		void AddWidget(CGUIWidget* widget);

		void UpdateWidgetLayer(CGUIWidget* widget);

		void DestroyWidget(CGUIWidget* widget);

		void SetResolution(float resolution_x, float resolution_y);

		float GetResolutionX();

		float GetResolutionY();

		Vector3 GetCenterPosition();

		void OnUpdate();

		void Quit();
	};

}

#endif