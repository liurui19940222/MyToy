#pragma once

#include<Windows.h>
#include<windowsx.h>
#include<memory>
#include<functional>
#include"defs.h"

namespace dxgame
{
	class IWindowEventHandler 
	{
	public:
		virtual void OnCreated();
		virtual void OnSizeChanged(int width, int height);
		virtual void OnActivate(bool active);
		virtual void OnDestroy();
	};

	class Window : public std::enable_shared_from_this<Window>
	{
	public:
		explicit Window(IWindowEventHandler* handler) except;
		virtual ~Window() noexcept;

		void init() except;
		void close() noexcept;
		void setTitle(const std::wstring& str) noexcept;
		int getWidth() const noexcept;
		int getHeight() const noexcept;
		void getRect(RECT& rect) const noexcept;

		inline HWND getHwnd() const noexcept { return m_Hwnd; }

		LRESULT CALLBACK procMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

		static std::shared_ptr<Window> getWindowPtr() noexcept;

	private:

		void onResize() noexcept;

		HWND				m_Hwnd;
		bool				m_Active;
		bool				m_IsMinimized;
		bool				m_IsMaximized;
		bool				m_IsResizing;
		IWindowEventHandler*			m_EventHandler;
		static std::shared_ptr<Window>	windowPtr;
	};
}