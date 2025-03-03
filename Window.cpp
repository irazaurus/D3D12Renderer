#include "Window.h"
#include <sstream>

// WindowClass
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance(GetModuleHandle(NULL))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = 0;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = NULL; wc.hCursor = NULL;
    wc.hbrBackground = NULL; wc.lpszMenuName = NULL;
    wc.lpszClassName = GetName();
    RegisterClassEx(&wc);
}


Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const WCHAR* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInstance;
}

WCHAR* Window::charToWCHAR(const char* c)
{
    size_t newSize = strlen(c) + 1;
    wchar_t* res = new wchar_t[newSize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, res, newSize, c, _TRUNCATE);
    return res;
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) return msg.wParam;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

// Window
Window::Window(int width, int height, const WCHAR* name)
{

    // calculate window size for the client region AKA except title and border
    RECT wr;
    wr.left = 200;
    wr.right = width + wr.left;
    wr.top = 200;
    wr.bottom = height + wr.top;
    if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
    {
        throw LAST_WNDEXCEPTION();
    }
    // create window and get hWnd
    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, WindowClass::GetInstance(), this
    );
    if (hWnd == nullptr)
    {
        throw LAST_WNDEXCEPTION();
    }
    // show window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

// setting up the pointer to Window instance
LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // this is where we connect our window instance to the Windows API
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); // link Window inside the UserData on the WinAPI side
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk)); // link to HandleMsgThunk
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// envokes HandleMsg function
LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

// message handle for closing the window, etc.
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    // close application
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0; // to not destroy the window twice

    // left mouse button
    case WM_LBUTTONDOWN:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WinException::WinException(int line, const char* file, HRESULT hRes) noexcept
    :
    Exception(line, file),
    hRes(hRes)
{
}

const char* Window::WinException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::WinException::GetType() const noexcept
{
    return "Window Error";
}

std::string Window::WinException::TranslateErrorCode(HRESULT hRes) noexcept
{
    LPWSTR pMsgBuf = nullptr;

    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, hRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&pMsgBuf, 0, NULL
    );

    // 0 string length returned indicates a failure
    if (nMsgLen == 0)
    {
        return "Unidentified";
    }

    // copy error string from windows-allocated buffer to char
    std::wstring wstr(pMsgBuf);
    size_t len = wstr.length() + 1;
    size_t i = 0;
    char* buffer = new char[len];

    // Converting wstring to string
    wcstombs_s(&i, buffer, len, pMsgBuf, len - 1);
    std::string str(wstr.begin(), wstr.end());
    //std::string str(buffer);

    // man fuck this shit

    // Cleaning up the buffers
    delete[] buffer;
    LocalFree(pMsgBuf);
    return str;
}

HRESULT Window::WinException::GetErrorCode() const noexcept
{
    return hRes;
}

std::string Window::WinException::GetErrorString() const noexcept
{
    return TranslateErrorCode(hRes);
}

