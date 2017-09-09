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
	 *���ô��ڵĴ�С
	 *parent_width:������Ŀͻ������
	 *parent_height:������Ŀͻ����߶�
	 */
	void SetLocalRect(RECT& rect, float parent_width, float parent_height);

	//�õ����ڵĴ�С
	RECT GetLocalRect();

	//�õ��ͻ�����ȫ�ִ�С
	static RECT GetGlobalClientRect(HWND hwnd);

	//��master��weightȨ�صĴ�С���ָ�guest��������guest�Ĵ�С
	static void Divide(CWindow& master, CWindow& guest, EDockDirection dir, float weight);

	//���ı�parent�Ĵ�С����weightȨ�صĴ�С������ָ�child��������child�Ĵ�С��ͬʱ�ἷѹwindows�����д�����������֮��
	static void Hold(CWindow& parent, CWindow& child, vector<CWindow*>& windows, EDockDirection dir, float weight);

	//��windows���д����ų���mask֮��
	static void Shrink(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height);

	//��windows���д��ھ�����ռ�����ÿռ�
	static void Expand(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height);

	//�õ�windows���д�����ռ������������
	static RECT MaxRect(vector<CWindow*>& windows);

	//������õ���Ч���������
	static RECT GetAvailableRect(RECT& all, RECT& mask, EDockDirection dir);

	//�õ�һ��RECT�ṹ�����ĵ�
	static Point2D GetCenterOfGravity(RECT& rect);

	property_r<HWND> WindowHandle = _prop_r(HWND, { return m_hwnd; });
	property_r<EChannelState> State = _prop_r(EChannelState, { return m_state; });
};

#endif