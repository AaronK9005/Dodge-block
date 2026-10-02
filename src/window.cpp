#include "window.hpp"

#include <format>

#include "error_util.hpp"
#include "input_map.hpp"

LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    InputMap* inputMap = nullptr;

    if (msg == WM_NCCREATE)
    {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);

        inputMap = static_cast<InputMap*>(create->lpCreateParams);

        SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(inputMap)
        );
    }
    else
    {
        inputMap = reinterpret_cast<InputMap*>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA)
        );
    }

    if (!inputMap)
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case InputMap::MOVE_LEFT:
        case VK_LEFT:
            inputMap->moveLeft = true;
            break;
        case InputMap::MOVE_RIGHT:
        case VK_RIGHT:
            inputMap->moveRight = true;
            break;
        case InputMap::TOGGLE_PAUSE:
        case VK_TAB:
            // ignore auto repeat
            if (lParam & (1 << 30))
            {
                return 0;
            }
            inputMap->togglePause = true;
            break;
        case InputMap::QUIT:
        case VK_ESCAPE:
            // ignore auto repeat
            if (lParam & (1 << 30))
            {
                return 0;
            }
            inputMap->quit = true;
            break;
        case InputMap::REPLAY:
            inputMap->replay = true;
            break;
        case VK_RETURN: // enter
            if (lParam & (1 << 30))
            {
                return 0;
            }
            inputMap->enter = true;
            break;
        }
        return 0;
    case WM_KEYUP:
        switch (wParam)
        {
        case InputMap::MOVE_LEFT:
        case VK_LEFT:
            inputMap->moveLeft = false;
            break;
        case InputMap::MOVE_RIGHT:
        case VK_RIGHT:
            inputMap->moveRight = false;
            break;
        case InputMap::TOGGLE_PAUSE:
        case VK_TAB:
            inputMap->togglePause = false;
            break;
        case InputMap::QUIT:
        case VK_ESCAPE:
            inputMap->quit = false;
            break;
        case InputMap::REPLAY:
            inputMap->replay = false;
            break;
        case VK_RETURN: // enter
            inputMap->enter = false;
            break;
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window() {}

bool Window::init(InputMap& inputMap)
{
    hInstance = GetModuleHandleW(nullptr);
    if (!hInstance)
    {
        showErrorInFunc(L"GetModuleHandleW");
        return false;
    }

    HICON hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    if (!hIcon)
    {
        showErrorInFunc(L"LoadIcon");
        return false;
    }

    HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);
    if (!hCursor)
    {
        showErrorInFunc(L"LoadCursor");
        return false;
    }

    WNDCLASSW wc{};

    wc.lpfnWndProc      = WindowProc;
    wc.hInstance        = hInstance;
    wc.hIcon            = hIcon;
    wc.hCursor          = hCursor;
    wc.hbrBackground    = nullptr;
    wc.lpszClassName    = CLASS_NAME;

    if (!RegisterClassW(&wc))
    {
        showErrorInFunc(L"RegisterClassW");
        return false;
    }
    else
    {
        classRegistered = true;
    }

    DWORD style = 0;
    RECT rect;

    style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    rect.left = 200;
    rect.top = 200;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        WINDOW_NAME,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        &inputMap
    );

    if (!hWnd)
    {
        showErrorInFunc(L"CreateWindowExW");
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);

    return true;
}

void Window::fireClose()
{
    // fire and wait for dispach
    SendMessageW(hWnd, WM_CLOSE, 0, 0);
}

Window::~Window()
{
    if (classRegistered)
    {
        UnregisterClassW(CLASS_NAME, hInstance);
    }
}

bool Window::processMessages()
{
    MSG msg = {};

    while (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return true;
}

void Window::showError(const std::wstring& msg)
{
    MessageBoxW(
        hWnd,
        msg.c_str(),
        L"Error",
        MB_OK | MB_ICONERROR
    );
}

void Window::showError(const std::wstring& errorFunc, const std::wstring& msg)
{
    MessageBoxW(
        hWnd,
        msg.c_str(),
        std::format(L"Error in: {}", errorFunc).c_str(),
        MB_OK | MB_ICONERROR
    );
}

/**
 * @brief 
 * 
 * @param where errorFunc
 * @return 
 */
void Window::showErrorInFunc(const std::wstring& where)
{
    showError(where, getLastErrorMessage());
}
