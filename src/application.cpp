#include "application.hpp"

#include <iostream>
#include <chrono>
#include <gdiplus.h>

using Clock = std::chrono::steady_clock;

bool Application::init()
{
    if (!window.init(inputMap))
    {
        window.showError(L"Failed to init window");
        return false;
    }

    if (!renderer.init(window.getWindow()))
    {
        window.showError(L"Failed to init renderer");
        return false;
    }

    game.receiveWindowSize(window.getWidth(), window.getHeight());

    // std::cout << "Application '" << appName << "' started successfully" << std::endl;

    return true;
}

void Application::run()
{
    running = true;

    if (!game.start())
    {
        std::cout << "Game failed at start" << std::endl;
        return;
    }

    auto lastTime = Clock::now();

    while(running)
    {
        if (!window.processMessages())
        {
            running = false;
        }

        auto currentTime = Clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // prevent huge dt
        deltaTime = std::min(deltaTime, 1.f / 6.f);

        fpsCounter.update(deltaTime);
        renderer.drawText(
            DrawTextInfo(
                std::format(L"fps: {}", fpsCounter.getFps()).c_str(),
                {50.f, 100.f}
            )
        );

        game.obtainDeltaTime(deltaTime);
        if (!game.handleInput(inputMap))
        {
            // exit key pressed
            window.fireClose();
            break;
        }
        game.update();
        // game.render(window.getWindow());
        renderer.render(game.provideGameData());
    }

    game.end();

}

void Application::shutdown()
{
    // std::cout << "Application '" << appName << "' ended" << std::endl;
}