#include "renderer.hpp"

#include <format>

Renderer::Renderer() {}
Renderer::~Renderer() {}

// bool Renderer::resize()
// {

// }
bool Renderer::init(HWND hwnd)
{
    if (!hwnd)
    {
        return false;
    }

    hwnd_ = hwnd;

    RECT rect;
    GetClientRect(hwnd, &rect);

    if (!resizeBuffer(rect.right - rect.left, rect.bottom - rect.top))
    {
        return false;
    }

    return true;
}

void Renderer::render(const GameData& data)
{
    if (!updateWindowSize())
    {
        // back buffer needs resizing but it fails
        MessageBoxW(
            hwnd_,
            L"Back buffer resize failed!",
            L"Back buffer error",
            MB_OK | MB_ICONERROR
        );

        return;
    }

    {
        Gdiplus::Graphics graphics(buffer.get());

        if (data.state == State::Playing)
        {
            renderGameplay(data, graphics);
        }
        else if (data.state == State::Gameover)
        {
            renderGameOver(data, graphics);
        }
    }

    HDC hdc = GetDC(hwnd_);

    {
        Gdiplus::Graphics graphics(hdc);

        graphics.DrawImage(
            buffer.get(),
            0,
            0,
            bufferWidth,
            bufferHeight
        );
    }

    // InvalidateRect(hwnd_, nullptr, FALSE);

    ReleaseDC(hwnd_, hdc);
}

bool Renderer::resizeBuffer(int widht, int height)
{
    if (widht <= 0 || height <= 0)
    {
        // minimizing will set widht & height to 0
        return true;
    }

    auto new_buffer = std::make_unique<Gdiplus::Bitmap>(
        widht,
        height,
        PixelFormat32bppARGB
    );

    if (new_buffer->GetLastStatus() != Gdiplus::Ok)
    {
        return false;
    }

    buffer = std::move(new_buffer);

    bufferWidth = widht;
    bufferHeight = height;

    return true;
}

bool Renderer::updateWindowSize()
{
    RECT rect;
    GetClientRect(hwnd_, &rect);

    const int width = rect.right - rect.left;
    const int height = rect.bottom - rect.top;

    if (width != bufferWidth || height != bufferHeight)
    {
        return resizeBuffer(width, height);
    }

    return true;
}

void Renderer::renderGameplay(const GameData& data, Gdiplus::Graphics& graphics)
{
    graphics.Clear(backgroundColor);

    // player
    graphics.FillRectangle(
        &playerBrush,
        data.player.position.x,
        data.player.position.y,
        data.player.size.x,
        data.player.size.y
    );
    graphics.DrawRectangle(
        &playerPen,
        data.player.position.x,
        data.player.position.y,
        data.player.size.x,
        data.player.size.y
    );

    // render blocks
    for (const FallingBlock& block : data.blocks)
    {
        graphics.FillRectangle(
            &blocksBrush,
            block.position.x,
            block.position.y,
            block.size,
            block.size
        );

        graphics.DrawRectangle(
            &blocksPen,
            block.position.x,
            block.position.y,
            block.size,
            block.size
        );
    }

    // render score
    Gdiplus::PointF scorePosF = {50.f, 50.f};
    
    graphics.DrawString(
        std::format(L"score: {}", data.player.score).c_str(),
        -1,
        &font,
        scorePosF,
        &whiteBrush
    );
}

void Renderer::renderGameOver(const GameData& data, Gdiplus::Graphics& graphics)
{
    graphics.Clear(backgroundColor);

    RECT rect;
    GetClientRect(hwnd_, &rect);

    const float width = static_cast<float>(rect.right);
    const float height = static_cast<float>(rect.bottom);

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::SolidBrush red(Gdiplus::Color::Red);
    
    const float lineHeight = 30.0f;
    const float titleHeight = 40.0f;
    const float gap = 5.0f;

    const int whiteLineCount = 1;

    const float bodyHeight = lineHeight * whiteLineCount;
    const float totalHeight = titleHeight + gap + bodyHeight;

    float y = (height - totalHeight) / 2.0f;

    Gdiplus::RectF titleRect(0, y, width, titleHeight);
    graphics.DrawString(L"Game over!", -1, &font, titleRect, &format, &red);
    y += titleHeight + gap;

    const wchar_t* lines[] = {
        std::format(L"score: {}", data.player.score).c_str(),
        L"[esc/Q] quit",
        L"[R/enter] play again"
    };

    for (const auto* line : lines)
    {
        Gdiplus::RectF lineRect(0, y, width, lineHeight);

        graphics.DrawString(line, -1, &smallFont, lineRect, &format, &whiteBrush);

        y += lineHeight;
    }
}

