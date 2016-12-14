#pragma once
#pragma warning(push, 0)
#include <atlbase.h>
#include <atlwin.h>
#pragma warning(pop)

namespace Graphics
{
	class Window : public CWindowImpl<Window, CWindow, CWinTraits<WS_OVERLAPPEDWINDOW | WS_VISIBLE>>
	{
	public:
		DECLARE_WND_CLASS_EX(L"PCGraphicsWindow", CS_VREDRAW | CS_HREDRAW, -1)
		int Width;
		int Height;
		HWND Handle;
		bool PostQuitWhenDestroyed;

		Window(const wchar_t* name, bool postQuitWhenDestroyed = true);
		~Window();

	private:
		BEGIN_MSG_MAP(Window)
			MESSAGE_HANDLER(WM_PAINT, Paint)
			MESSAGE_HANDLER(WM_DESTROY, Destroy)
			MESSAGE_HANDLER(WM_SIZE, Resize)
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, DisplayChange)
		END_MSG_MAP()

		LRESULT Paint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT Destroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT Resize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT DisplayChange(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
	};
}