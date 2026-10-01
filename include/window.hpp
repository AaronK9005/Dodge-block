#pragma once

#include <windows.h>
#include <string>

struct InputMap;

static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

struct Window
{
private:
    HINSTANCE hInstance = nullptr;
    HWND hWnd = nullptr; // window in win32
    const wchar_t* CLASS_NAME = L"FirstWindowClass";
    const wchar_t* WINDOW_NAME = L"Dodge the Falling Blocks";
    bool classRegistered = false;

    int winX = 0;
    int winY = 0;
    int width = 800; // actuall draw width
    int height = 600; // actuall draw height

public:
    HINSTANCE getHInstance() { return hInstance; }
    HWND getWindow() { return hWnd; }
    int getWidth() { return width; }
    int getHeight() { return height; }

    bool init(InputMap&);
    void fireClose();
    Window();
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool processMessages();
    void showError(const std::wstring& msg);
    void showError(const std::wstring& errorFunc, const std::wstring& msg);
    void showErrorInFunc(const std::wstring& where);
};