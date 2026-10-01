#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <memory>

#include "game_data.hpp"

class Renderer
{
    Gdiplus::Font font{
        L"Arial",
        24,
        Gdiplus::FontStyleRegular,
        Gdiplus::UnitPixel
    };
    Gdiplus::Font smallFont{
        L"Arial",
        18,
        Gdiplus::FontStyleRegular,
        Gdiplus::UnitPixel
    };

    Gdiplus::Color whiteColor{255, 255, 255, 255};
    Gdiplus::Color backgroundColor{255, 30, 30, 30};
    Gdiplus::Color playerColor{255, 50, 200, 50};
    Gdiplus::Color blocksColor{255, 200, 50, 50};

    Gdiplus::Pen playerPen{playerColor, 2.0f};
    Gdiplus::Pen blocksPen{blocksColor, 2.0f};

    Gdiplus::SolidBrush playerBrush{playerColor};
    Gdiplus::SolidBrush blocksBrush{blocksColor};
    Gdiplus::SolidBrush whiteBrush{whiteColor};

    HWND hwnd_ = nullptr;

    std::unique_ptr<Gdiplus::Bitmap> buffer = nullptr;

    int bufferWidth = 0;
    int bufferHeight = 0;

private:
    bool resizeBuffer(int widht, int height);
    bool updateWindowSize();

    void renderGameplay(const GameData& data, Gdiplus::Graphics& graphics);
    void renderGameOver(const GameData& data, Gdiplus::Graphics& graphics);

public:
    Renderer();
    ~Renderer();
    bool init(HWND hwnd);
    // bool resize();
    void render(const GameData& data);
};