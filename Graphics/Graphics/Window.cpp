#include "Window.h"

namespace Graphics
{
	Window::Window(const wchar_t* name, bool postQuitWhenDestroyed) : PostQuitWhenDestroyed(postQuitWhenDestroyed)
	{
		Handle = Create(NULL, NULL, name);
	}

	Window::~Window()
	{
	}

	LRESULT Window::Paint(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*handled*/)
	{
		RECT rect{};
		GetClientRect(&rect);
		PAINTSTRUCT ps{};

		BeginPaint(&ps);

		EndPaint(&ps);
		return 0;
	}

	LRESULT Window::Destroy(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*handled*/)
	{
		if (PostQuitWhenDestroyed)
			PostQuitMessage(0);
		return 0;
	}

	LRESULT Window::Resize(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*handled*/)
	{
		RECT rect{};
		GetClientRect(&rect);
		Width = static_cast<unsigned int>(rect.right - rect.left);
		Height = static_cast<unsigned int>(rect.bottom - rect.top);
		InvalidateRect(&rect);
		return 0;
	}

	LRESULT Window::DisplayChange(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*handled*/)
	{
		//todo we should probably do something here...
		return 0;
	}

	LRESULT Window::ResizeExit(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*handled*/)
	{

		if (onResize)
			onResize();
		return 0;
	}
}

