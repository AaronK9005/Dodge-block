#pragma once

struct InputMap
{
    bool moveLeft = false;
    bool moveRight = false;
    bool togglePause = false;
    bool quit = false;
    bool enter = false;

    // virtual code are always uppercase for win32
    static constexpr char MOVE_LEFT = 'A';
    static constexpr char MOVE_RIGHT = 'D';
    static constexpr char TOGGLE_PAUSE = 'P';
    static constexpr char QUIT = 'Q';
};