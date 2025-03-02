#pragma once
#include "CustomWindows.h"

class Window
{
private:
// WindowClass singleton
	class WindowClass
	{
	public:
		static const WCHAR* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;			  // copy prohibition
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const WCHAR* wndClassName = L"Window for render";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};

// Window
public:
	Window(int width, int height, const WCHAR* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;
};

