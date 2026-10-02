#include "game.hpp"

#include <windows.h>
#include <gdiplus.h>

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
        if (input.replay || input.enter)
        {
            end();
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

    perSecondUpdates();

    // spawn blocks
    data.timeAccumulator += dt;
    if (data.timeAccumulator >= data.spawnCooldown)
    {
        data.timeAccumulator -= data.spawnCooldown;
        spawnBlock();
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

        // block fell out
        if (block.position.y >= winSize.y)
        {
            data.player.score += scoreForDodged;

            data.blocks[i] = std::move(data.blocks.back());
            data.blocks.pop_back();
        }
        else{
            i++;
        }
    }
}

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
}

void Game::perSecondUpdates()
{
    // seconds timer
    secondsAccumulator += dt;
    if (secondsAccumulator >= 1.f)
    {
        secondsAccumulator -= 1.f;
        data.player.score += scorePerSecond;

        data.gravity += 1.0f;
        if (data.gravity > GameData::maxGravity)
        {
            data.gravity = GameData::maxGravity;
        }

        data.spawnCooldown -= 0.05f;
        if (data.spawnCooldown < GameData::minSpawnCooldown)
        {
            data.spawnCooldown = GameData::minSpawnCooldown;
        }
    }
}
