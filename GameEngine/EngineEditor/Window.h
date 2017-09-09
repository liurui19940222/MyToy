#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include<Windows.h>
#include<windowsx.h>
#include<GameEngine\Property.h>
#include"Editor.h"

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

class CWindow
{
private:
	FRect m_normalizedRect;
	float m_parentWidth;
	float m_parentHeight;

	void OnSizeChanged();

protected:
	HWND m_hwnd;
	EChannelState m_state;

	void Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);
	void SetRect(RECT& rect);
	RECT GetRect();
	RECT GetRectClient();

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

	/*
	 *设置窗口的大小
	 *parent_width:父窗体的客户区宽度
	 *parent_height:父窗体的客户区高度
	 */
	void SetLocalRect(RECT& rect, float parent_width, float parent_height);

	//得到窗口的大小
	RECT GetLocalRect();

	//得到客户区的全局大小
	static RECT GetGlobalClientRect(HWND hwnd);

	//将master的weight权重的大小，分给guest，并设置guest的大小
	static void Divide(CWindow& master, CWindow& guest, EDockDirection dir, float weight);

	//不改变parent的大小，将weight权重的大小的区域分给child，并设置child的大小，同时会挤压windows的所有窗口至该区域之外
	static void Hold(CWindow& parent, CWindow& child, vector<CWindow*>& windows, EDockDirection dir, float weight);

	//把windows所有窗口排除到mask之外
	static void Shrink(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height);

	//把windows所有窗口尽可能占满可用空间
	static void Expand(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height);

	//得到windows所有窗口所占的最大矩形区域
	static RECT MaxRect(vector<CWindow*>& windows);

	//按方向得到有效的最大区域
	static RECT GetAvailableRect(RECT& all, RECT& mask, EDockDirection dir);

	//得到一个RECT结构的中心点
	static Point2D GetCenterOfGravity(RECT& rect);

	property_r<HWND> WindowHandle = _prop_r(HWND, { return m_hwnd; });
	property_r<EChannelState> State = _prop_r(EChannelState, { return m_state; });
};

#endif