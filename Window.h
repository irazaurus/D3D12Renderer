#pragma once
#include "CustomWindows.h"
#include "Exception.h"
#include <cstdlib>
#include <optional>

class Window
{
private:
// Exceptions
	class WinException : public Exception {
	public:
		WinException(int line, const char* file, HRESULT hRes) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
//		virtual const WCHAR* GetTypeWCHAR() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hRes) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hRes;
	};

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
	Window(int width, int height, const WCHAR* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static WCHAR* charToWCHAR(const char* c);
	// custom process for the window to stop freezing while waiting for messages
	static std::optional<int> ProcessMessages();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;
};

#define WNDEXCEPTION(hRes) Window::WinException( __LINE__, __FILE__, hRes)
#define LAST_WNDEXCEPTION() Window::WinException( __LINE__, __FILE__, GetLastError())