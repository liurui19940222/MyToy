#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include<functional>
#include<GameEngine\Singleton.h>
#include<GameEngine\Math.h>
#include<GameEngine\PriorityQueue.h>

#define _GUIManager CGUIManager::GetInstance()

class CGUIElement;

using namespace std;
using namespace container;

class CGUIManager
{
public:
	typedef function<void(CGUIElement*)> ElementForeachCallback;
	typedef function<bool(CGUIElement*)> ElementForeachCallbackR;
private:
	float m_resolutionX, m_resolutionY;
	CGUIElement* m_curOverlay;
	CGUIElement* m_lastDown;
	Vector3 m_centerPos;
	CPriorityQueue<CGUIElement*> m_elements;

public:
	void InitGUI(float resolution_x, float resolution_y);
	void UpdateWidgetLayer(CGUIElement* widget);
	void Destroy(CGUIElement* widget);
	void ForeachElement(ElementForeachCallback callback);
	void ForeachElementR(ElementForeachCallbackR callback);
	void InverseForeachElement(ElementForeachCallback callback);
	void InverseForeachElementR(ElementForeachCallbackR callback);
	void SetResolution(float resolution_x, float resolution_y);
	float GetResolutionX();
	float GetResolutionY();
	Vector3 GetCenterPosition();
	void OnUpdate();
	void OnRender();
	void Quit();

	template<typename T> T* Create()
	{
		T* t = new T();
		t->m_manager = this;
		m_elements.Enqueue(t);
		return t;
	}
};

#endif