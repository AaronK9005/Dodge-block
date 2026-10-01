#include "game.hpp"

#include <windows.h>
#include <gdiplus.h>

#include <iostream> // debug

Game::Game()
    : gen(rd())
{

}

Game::~Game()
{
    
}

bool Game::start()
{
    data.player.position = {
        winSize.x / 2,
        (winSize.y - data.player.size.y) * 9 / 10
    };

    for (int i = 0; i < Game::startingBlocks; i++)
    {
        spawnBlock();
    }

    return true;
}

bool Game::handleInput(const InputMap& input)
{
    if (input.quit)
    {
        return false;
    }

    if (data.state == State::Gameover)
    {
        if (input.enter)
        {
            data = {};
            secondsAccumulator = 0.0f;
            start();
            return true;
        }
    }

    if (input.moveLeft)
    {
        Player& p = data.player;

        p.position.x -= static_cast<int>(p.speed * dt);

        if (p.position.x < 0)
        {
            p.position.x = 0;
        }
    }

    if (input.moveRight)
    {
        Player& p = data.player;

        p.position.x += static_cast<int>(p.speed * dt);

        if (p.position.x > winSize.x - p.size.x)
        {
            p.position.x = winSize.x - p.size.x;
        }
    }

    return true;
}

void Game::update()
{
    if (data.state == State::Gameover)
    {
        return;
    }

    // add score
    secondsAccumulator += dt;
    if (secondsAccumulator >= 1.f)
    {
        secondsAccumulator -= 1.f;
        data.player.score += scorePerSecond;
    }

    // update spawnCooldown and spawn blocks
    data.timeAccumulator += dt;
    if (data.timeAccumulator >= data.spawnCooldown)
    {
        data.timeAccumulator -= data.spawnCooldown;
        spawnBlock();

        data.spawnCooldown -= 0.05f;
        if (data.spawnCooldown < GameData::minSpawnCooldown)
        {
            data.spawnCooldown = GameData::minSpawnCooldown;
        }
    }

    for (size_t i = 0; i < data.blocks.size(); )
    {
        FallingBlock& block = data.blocks[i];

        block.position.y += static_cast<int>(data.gravity * block.fallSpeedMultiplier * dt);

        #pragma region AABB collision func
        auto intersectsAABB = []
        (glm::ivec2 const& posA, glm::ivec2 const sizeA, glm::ivec2 const& posB, glm::ivec2 const sizeB)
        {
            return  posA.x < posB.x + sizeB.x &&
                    posA.x + sizeA.x > posB.x &&
                    posA.y < posB.y + sizeB.y &&
                    posA.y + sizeA.y > posB.y;
        };
        #pragma endregion

        if (intersectsAABB(
            data.player.position,
            data.player.size,
            block.position,
            glm::ivec2(block.size, block.size)
        ))
        {
            // collision with player
            data.player.health--;
            if (data.player.health <= 0.0f)
            {
                data.state = State::Gameover;
                return;
            }
        }

        if (block.position.y >= winSize.y)
        {
            // block fell out -> update score
            // std::cout << "Block fell out. Current count: " << data.blocks.size() << std::endl;
            data.player.score += 10;

            data.blocks[i] = std::move(data.blocks.back());
            data.blocks.pop_back();
        }
        else{
            i++;
        }
    }
}

/*
void Game::render(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);

    Gdiplus::Graphics graphics(hdc);

    graphics.Clear(Gdiplus::Color(255, 30, 30, 30));

    // player
    const Gdiplus::Color playerColor(255, 50, 200, 50);
    Gdiplus::Pen playerPen(playerColor, 2.0f);
    Gdiplus::SolidBrush playerBrush(playerColor);
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
    const Gdiplus::Color blockColor(255, 200, 50, 50);
    Gdiplus::Pen blockPen(blockColor, 2.0f);
    Gdiplus::SolidBrush blockBrush(blockColor);

    for (const FallingBlock& block : data.blocks)
    {
        graphics.FillRectangle(
            &blockBrush,
            block.position.x,
            block.position.y,
            block.size,
            block.size
        );

        graphics.DrawRectangle(
            &blockPen,
            block.position.x,
            block.position.y,
            block.size,
            block.size
        );
    }

    // render score
    Gdiplus::Font font(L"Arial", 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF scorePosF = {50.f, 50.f};
    Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));

    graphics.DrawString(
        std::format(L"score: {}", data.player.score).c_str(),
        -1,
        &font,
        scorePosF,
        &whiteBrush
    );

    ReleaseDC(hwnd, hdc);
}
*/

void Game::end()
{
    
}

int Game::randomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

float Game::randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void Game::spawnBlock()
{
    int size = randomInt(10, 50);

    data.blocks.emplace_back(
        glm::ivec2(
            randomInt(0, winSize.x - size),
            randomInt(-200, 0) - size
        ),
        size,
        randomFloat(0.9f, 1.5f)
    );

    // std::cout << "block spawned. Current count: " << data.blocks.size() << std::endl;
}
