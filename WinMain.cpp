#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    // register window class
    Window wnd(800, 500, L"Renderer");

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