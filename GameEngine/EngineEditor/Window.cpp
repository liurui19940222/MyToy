#include"Window.h"
#include"Editor.h"
#include<GameEngine\Input.h>

CWindow::CWindow() {}

CWindow::CWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{
	Create(class_name, instance, parent, width, height, dword);
}

CWindow::~CWindow()
{

}

void CWindow::Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{
	int x = 0, y = 0;
	if (parent == NULL)
	{
		x = (GetSystemMetrics(0) - (int)width) >> 1;
		y = (GetSystemMetrics(1) - (int)height) >> 1;
	}
	else
	{
		RECT rect;
		GetWindowRect(parent, &rect);
		x = (((rect.right - rect.left) - (int)width) >> 1);
		y = (((rect.bottom - rect.top) - (int)height) >> 1);
	}
	m_state = EChannelState::Fixed;
	m_hwnd = CreateWindowEx(NULL, class_name, GetTitle(), dword | GetStyle(),
		x, y, width, height, parent, GetMenu(), instance, (void*)this);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

EWindowType CWindow::GetType()
{
	return EWindowType::Other;
}

DWORD CWindow::GetStyle()
{
	return 	0;
}

HMENU CWindow::GetMenu()
{
	return NULL;
}

wchar_t* CWindow::GetTitle()
{
	return L"";
}

LRESULT CALLBACK CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		if (CInput::GetMouse(EMouseKey::Left))
			OnSizeChanged();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CWindow::Divide(CWindow& master, CWindow& guest, EDockDirection dir, float weight)
{
	RECT clientRect = master.GetLocalRect();
	RECT guestRect = clientRect;
	RECT masterRect = clientRect;
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	if (dir == EDockDirection::Left)
	{
		guestRect.right = clientRect.left + (int)((float)width * weight);
		masterRect.left = clientRect.right - (int)((float)width * (1 - weight));
	}
	else if (dir == EDockDirection::Top)
	{
		guestRect.bottom = clientRect.top + (int)((float)height * weight);
		masterRect.top = clientRect.bottom - (int)((float)height * (1 - weight));
	}
	else if (dir == EDockDirection::Right)
	{
		guestRect.left = clientRect.right - (int)((float)width * weight);
		masterRect.right = clientRect.left + (int)((float)width * (1 - weight));
	}
	else if (dir == EDockDirection::Bottom)
	{
		guestRect.top = clientRect.bottom - (int)((float)height * weight);
		masterRect.bottom = clientRect.top + (int)((float)height * (1 - weight));
	}
	master.SetLocalRect(masterRect, width, height);
	guest.SetLocalRect(guestRect, width, height);
}

void CWindow::Hold(CWindow& parent, CWindow& child, vector<CWindow*>& windows, EDockDirection dir, float weight)
{
	RECT clientRect = parent.GetRectClient();
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	if (weight == -1.0f)
	{
		child.SetRect(GetAvailableRect(clientRect, MaxRect(windows), dir));
		return;
	}
	RECT targetRect{ 0, 0, width, height };
	if (dir == EDockDirection::Left)
	{
		targetRect.right = (int)((float)width * weight);
	}
	else if (dir == EDockDirection::Top)
	{
		targetRect.bottom = (int)((float)height * weight);
	}
	else if (dir == EDockDirection::Right)
	{
		targetRect.left = width - (int)((float)width * weight);
	}
	else if (dir == EDockDirection::Bottom)
	{
		targetRect.top = height - (int)((float)height * weight);
	}
	child.SetLocalRect(targetRect, width, height);
	Shrink(windows, targetRect, (EDockDirection)((int)dir * -1), width, height);
}

void CWindow::Shrink(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height)
{
	Point2D cg = GetCenterOfGravity(mask);
	int dirFlag = abs((int)dir);
	for (CWindow* window : windows)
	{
		RECT rect = window->GetLocalRect();
		Point2D wcg = GetCenterOfGravity(rect);
		if (dirFlag == 2)
		{
			if (cg.x > wcg.x)
			{
				if (rect.right > mask.left)
				{
					rect.right = mask.left;
				}
				if (rect.left > mask.left)
				{
					rect.left = mask.left;
				}
			}
			else
			{
				if (rect.left < mask.right)
				{
					rect.left = mask.right;
				}
				if (rect.right < mask.right)
				{
					rect.right = mask.right;
				}
			}
		}
		else if (dirFlag == 1)
		{
			if (cg.y > wcg.y)
			{
				if (rect.bottom > mask.top)
				{
					rect.bottom = mask.top;
				}
				if (rect.top > mask.top)
				{
					rect.top = mask.top;
				}
			}
			else
			{
				if (rect.top < mask.bottom)
				{
					rect.top = mask.bottom;
				}
				if (rect.bottom < mask.bottom)
				{
					rect.bottom = mask.bottom;
				}
			}
		}
		window->SetLocalRect(rect, parent_width, parent_height);
	}
}

void CWindow::Expand(vector<CWindow*>& windows, RECT& mask, EDockDirection dir, float parent_width, float parent_height)
{
	vector<CWindow*> nearby;
	for (int i = 0; i < windows.size() - 1; ++i)
	{
		for (int j = 0; j < windows.size() - i - 1; ++j)
		{
			bool change = false;
			CWindow* c1 = windows[j];
			CWindow* c2 = windows[j + 1];
			if (dir == EDockDirection::Left)
			{
				change = c1->GetRect().left > c2->GetRect().left;
			}
			else if (dir == EDockDirection::Top)
			{
				change = c1->GetRect().top < c2->GetRect().top;
			}
			else if (dir == EDockDirection::Right)
			{
				change = c1->GetRect().right < c2->GetRect().right;
			}
			else if (dir == EDockDirection::Bottom)
			{
				change = c1->GetRect().bottom < c2->GetRect().bottom;
			}
			if (change)
			{
				CWindow* temp = windows[j];
				windows[j] = windows[j + 1];
				windows[j + 1] = temp;
			}
		}
	}
}

RECT CWindow::MaxRect(vector<CWindow*>& windows)
{
	RECT rect{ 100000, -100000, -100000, 100000 };
	for (CWindow* window : windows)
	{
		RECT r = window->GetRect();
		if (r.left < rect.left)
			rect.left = r.left;
		if (r.right > rect.right)
			rect.right = r.right;
		if (r.top < rect.top)
			rect.top = r.top;
		if (r.bottom > rect.bottom)
			rect.bottom = r.bottom;
	}
	return rect;
}

RECT CWindow::GetAvailableRect(RECT& all, RECT& mask, EDockDirection dir)
{
	RECT rect;
	if (dir == EDockDirection::Left)
	{
		rect.left = all.left;
		rect.right = mask.left;
		rect.top = all.top;
		rect.bottom = all.bottom;
	}
	else if (dir == EDockDirection::Top)
	{
		rect.top = all.top;
		rect.bottom = mask.bottom;
		rect.left = all.left;
		rect.right = all.right;
	}
	else if (dir == EDockDirection::Right)
	{
		rect.left = mask.right;
		rect.right = all.right;
		rect.top = all.top;
		rect.bottom = all.bottom;
	}
	else if (dir == EDockDirection::Bottom)
	{
		rect.top = mask.bottom;
		rect.bottom = all.bottom;
		rect.left = all.left;
		rect.right = all.right;
	}
	return rect;
}

void CWindow::SetRect(RECT& rect)
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
	{
		SetLocalRect(rect, rect.right - rect.left, rect.bottom - rect.top);
		return;
	}
	RECT parentRect = GetGlobalClientRect(parent);
	SetLocalRect(RECT{ rect.left - parentRect.left, rect.top - parentRect.top,
		rect.right - parentRect.left, rect.bottom - parentRect.top },
		parentRect.right - parentRect.left, parentRect.bottom - parentRect.top);
}

RECT CWindow::GetRect()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	return rect;
}

RECT CWindow::GetRectClient()
{
	return GetGlobalClientRect(m_hwnd);
}

void CWindow::SetLocalRect(RECT& rect, float parent_width, float parent_height)
{
	m_normalizedRect.left = (float)rect.left / parent_width;
	m_normalizedRect.top = (float)rect.top / parent_height;
	m_normalizedRect.right = (float)rect.right / parent_width;
	m_normalizedRect.bottom = (float)rect.bottom / parent_height;
	m_parentWidth = parent_width;
	m_parentHeight = parent_height;
	MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
}

RECT CWindow::GetLocalRect()
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
		return GetRect();
	RECT parentRect = GetGlobalClientRect(parent);
	RECT childRect = GetRect();
	return RECT{ childRect.left - parentRect.left, childRect.top - parentRect.top,
		childRect.right - parentRect.left, childRect.bottom - parentRect.top };
}

RECT CWindow::GetGlobalClientRect(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	RECT rect;
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + clientRect.right - clientRect.left;
	rect.top = p.y;
	rect.bottom = p.y + clientRect.bottom - clientRect.top;
	return rect;
}

void CWindow::UpdateRect(float parent_width, float parent_height)
{
	RECT rect;
	rect.left = (int)(m_normalizedRect.left * parent_width);
	rect.right = (int)(m_normalizedRect.right * parent_width);
	rect.top = (int)(m_normalizedRect.top * parent_height);
	rect.bottom = (int)(m_normalizedRect.bottom * parent_height);
	SetLocalRect(rect, parent_width, parent_height);
}

Point2D CWindow::GetCenterOfGravity(RECT& rect)
{
	Point2D point;
	point.x = rect.left + ((rect.right - rect.left) >> 1);
	point.y = rect.top + ((rect.bottom - rect.top) >> 1);
	return point;
}

void CWindow::OnSizeChanged()
{
	RECT rect = GetLocalRect();
	float nor_left = rect.left / m_parentWidth;
	float nor_right = rect.right / m_parentWidth;
	float nor_top = rect.top / m_parentHeight;
	float nor_bottom = rect.bottom / m_parentHeight;
	if (nor_left < m_normalizedRect.left)
	{
		CWindow::Shrink(_Editor->GetWindowsExcept(GetType()), rect, EDockDirection::Left, m_parentWidth, m_parentHeight);
	}
	else if (nor_right > m_normalizedRect.right)
	{
		CWindow::Shrink(_Editor->GetWindowsExcept(GetType()), rect, EDockDirection::Right, m_parentWidth, m_parentHeight);
	}
	if (nor_top > m_normalizedRect.top)
	{
		CWindow::Shrink(_Editor->GetWindowsExcept(GetType()), rect, EDockDirection::Top, m_parentWidth, m_parentHeight);
	}
	else if (nor_bottom < m_normalizedRect.bottom)
	{
		CWindow::Shrink(_Editor->GetWindowsExcept(GetType()), rect, EDockDirection::Bottom, m_parentWidth, m_parentHeight);
	}

	m_normalizedRect.left = nor_left;
	m_normalizedRect.top = nor_top;
	m_normalizedRect.right = nor_right;
	m_normalizedRect.bottom = nor_bottom;
}