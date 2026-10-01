#pragma once

#include <glm/glm.hpp>
#include <magic_enum/magic_enum.hpp>

#include <vector>

enum class State
{
    Playing, Gameover
};

struct FallingBlock {
    glm::ivec2 position = {0, 0};
    int size = 10;
    float fallSpeedMultiplier = 1.f;

    FallingBlock(glm::ivec2 pos, int size, float fallSpeedMult)
        : position(pos)
        , size(size)
        , fallSpeedMultiplier(fallSpeedMult)
    {}
};

struct Player {
    int score = 0;
    float speed = 500.f;

    float health = 1.0f;

    glm::ivec2 position = {100, 100};
    glm::ivec2 size = {30, 10};
};

struct GameData {
    State state = State::Playing;

    float gravity = 150.f;

    Player player = {};

    static constexpr float minSpawnCooldown = 1.0f / 5.0f;
    float spawnCooldown = 1.f;
    float timeAccumulator = 0.f;

    std::vector<FallingBlock> blocks = {};
};