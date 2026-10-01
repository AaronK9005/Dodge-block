#pragma once

#include <windows.h>
#include <gdiplus.h>

#include <string>

#include "window.hpp"
#include "renderer.hpp"
#include "input_map.hpp"
#include "game.hpp"

class Application
{
    std::string appName = "Dodge blocks";
    Window window{};
    Renderer renderer{};
    Game game{};
    InputMap inputMap{};
    bool running = false;

public:
    bool init();
    void run();
    void shutdown();
};