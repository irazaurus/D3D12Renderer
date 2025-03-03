#include "Window.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    // error handling
    try
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
    // catching errors
    catch (const Exception& e)
    {
        MessageBox(NULL, Window::charToWCHAR(e.what()), Window::charToWCHAR(e.GetType()), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(NULL, Window::charToWCHAR(e.what()), L"Standart Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch ( ... )
    {
        MessageBox(NULL, L"Unknown", L"Unknown", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}