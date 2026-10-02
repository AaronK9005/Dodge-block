#include <iostream>

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <format>

#include "application.hpp"
#include "error_util.hpp"

int main()
{
    #pragma region gdiplus startup
    Gdiplus::GdiplusStartupInput gdiplusInput;
    ULONG_PTR gdiplusToken;

    auto result = Gdiplus::GdiplusStartup(
        &gdiplusToken,
        &gdiplusInput,
        nullptr
    );

    if (result != Gdiplus::Ok)
    {
        std::cout << std::format("Gdiplus startup failed: Gdiplus::GpStatus = {}", static_cast<int>(result));
        return 1;
    }
    #pragma endregion

    {
        Application app;

        if (app.init())
        {
            app.run();
        }
        app.shutdown();
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}