#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include<Windows.h>
#include<windowsx.h>
#include<GameEngine\Property.h>
#include<GameEngine\MessageCenter.h>
#include<GameEngine\MessageDef.h>
#include"Editor.h"
#include"ContextMenu.h"

enum EWindowType;

enum class EChannelState
{
	Float = 4,
	Fixed = 5,
};

enum class EDockDirection
{
	Top = -1,
	Bottom = 1,
	Left = - 2,
	Right = 2,
};

class CWindow : public CGridElement, public IReceiver
{
	friend class CEditor;
private:
	FRect m_normalizedRect;
	float m_parentWidth;
	float m_parentHeight;

protected:
	float m_width;
	float m_height;
	HWND m_hwnd;
	EChannelState m_state;
	CContextMenu* m_contextMenu;

	void Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);
	virtual void OnCreate();
	virtual void OnWindowInitialized();
	virtual void OnClose();
	virtual void OnReceiveMsg(SMessage& message) override;

public:
	CWindow();
	CWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);
	virtual ~CWindow();

	virtual EWindowType GetType();
	virtual DWORD GetStyle();
	virtual HMENU GetMenu();
	virtual wchar_t* GetTitle();
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void UpdateRect(float parent_width, float parent_height);

	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnPositionChanged();
	virtual void OnFieldChanged(const FRect& rect) override;

	/*
	 *设置窗口的大小
	 *parent_width:父窗体的客户区宽度
	 *parent_height:父窗体的客户区高度
	 */
	void SetLocalRect(RECT& rect, float parent_width, float parent_height);

	//得到窗口的大小
	RECT GetLocalRect();

	void SetRect(RECT& rect);
	RECT GetRect();
	RECT GetRectClient();

	RECT LocalToGlobalRect(const RECT& rect);
	RECT GlobalToLocalRect(const RECT& rect);

	//得到客户区的全局大小
	static RECT GetGlobalClientRect(HWND hwnd);

	//得到头部(包含菜单栏，工具栏)高度
	float GetHeaderHeight();

	property_r<HWND> WindowHandle = _prop_r(HWND, { return m_hwnd; });
	property_r<EChannelState> State = _prop_r(EChannelState, { return m_state; });
};

#endif