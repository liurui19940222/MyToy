#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include<functional>
#include<SpGameEngine\Singleton.h>
#include<SpGameEngine\Math.h>
#include<SpGameEngine\PriorityQueue.h>
#include<SpGameEngine\RawRenderer.h>
#include"GridLayout.h"

class CGUIElement;

using namespace std;
using namespace container;

class CGUIManager
{
public:
	CGUIManager();
	typedef function<void(CGUIElement*)> ElementForeachCallback;
	typedef function<bool(CGUIElement*)> ElementForeachCallbackR;
private:
	Vector2 m_absoluteWindowLTPos = { 0, 0 };
	float m_resolutionX, m_resolutionY;
	CGUIElement* m_curOverlay;
	CGUIElement* m_lastDown;
	Vector3 m_centerPos;
	CPriorityQueue<CGUIElement*> m_elements;
	CRawRenderer* m_renderer;
	CGridLayout m_layout;
	int m_curRow = 0; //自动排列时的当前行

public:
	void InitGUI(float resolution_x, float resolution_y);
	void UpdateWidgetLayer(CGUIElement* widget);
	void Destroy(CGUIElement* widget);
	void DestroyAll();
	void ForeachElement(ElementForeachCallback callback);
	void ForeachElementR(ElementForeachCallbackR callback);
	void InverseForeachElement(ElementForeachCallback callback);
	void InverseForeachElementR(ElementForeachCallbackR callback);
	void SetResolution(float resolution_x, float resolution_y);
	void SetAbsoluteWindowLTPos(const Vector2& pos);
	float GetResolutionX();
	float GetResolutionY();
	Vector3 GetCenterPosition();
	Vector2 GetAbsoluteWindowLTPos() const;
	void SetRenderer(CRawRenderer* renderer);
	CRawRenderer* GetRenderer() const;
	void OnUpdate();
	void OnRender();
	void Quit();

	void DrawLayout();
	void PrintLayout();
	CGUIManager* SetRowsVisible(int startIndex, int count, bool visible);
	CGUIManager* UpdateLayout();
	CGUIManager* SetLayoutOffsetX(float offset);
	CGUIManager* SetLayoutOffsetY(float offset);
	CGUIManager* SetGridRowHeight(float rowHeight);
	CGUIManager* SetGridRowCount(int rowCount);
	CGUIManager* SetGridColumns(int colCount, vector<float>& weights);
	CGUIManager* PutIntoGrid(int rowIndex, int colIndex, CGUIElement* element, bool newline = false);
	CGUIManager* DeleteRow(int rowIndex, bool releaseElement);

	float GetLayoutOffsetX() const;
	float GetLayoutOffsetY() const;

	template<typename T> T* Create()
	{
		T* t = new T();
		t->m_manager = this;
		t->OnStart();
		m_elements.Enqueue(t);
		return t;
	}
};

#endif