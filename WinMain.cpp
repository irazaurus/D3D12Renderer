#include <Windows.h>

// Custom window processing for closing the window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // close application
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
        // left mouse button
    case WM_LBUTTONDOWN:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{

    // register window class
    const WCHAR* windowName = L"CustomRenderer";
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(wc);
    wc.style = 0;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL; wc.hCursor = NULL; 
    wc.hbrBackground = NULL; wc.lpszMenuName = NULL;
    wc.lpszClassName = windowName;

    RegisterClassEx(&wc);

    // create a window
    HWND handleWindow = CreateWindowEx(
        0, windowName, L"Renderer",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        300, 300,
        640, 480,
        NULL, NULL,
        hInstance,
        NULL
    );

    // show the damn window
    ShowWindow(handleWindow, SW_SHOW);

    // message handle
    MSG msg;
    BOOL quitCode;
    while ((quitCode = GetMessage(&msg, NULL, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // return code
    if (quitCode == -1) 
        return -1;
    
    return msg.wParam;
}