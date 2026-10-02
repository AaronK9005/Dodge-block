#pragma once

#include "input_map.hpp"
#include "game_data.hpp"

#include <windef.h>
#include <random>

class Game
{
    static constexpr int startingBlocks = 1;
    static constexpr int scorePerSecond = 1;
    static constexpr int scoreForDodged = 10;

    GameData data = {};
    
    float dt = 0.f;
    float secondsAccumulator = 0.f;

    glm::ivec2 winSize = {800, 600}; // treshold, over which blocks should be destroyed

    // random
    std::random_device rd{};
    std::mt19937 gen{};
private:
    int randomInt(int min, int max);
    float randomFloat(float min, float max);
    void spawnBlock();
    void perSecondUpdates();
public:
    Game();
    ~Game();
    bool start();
    void receiveWindowSize(int widht, int height) { winSize = { widht, height }; }
    void obtainDeltaTime(float dt) { this->dt = dt; }
    bool handleInput(const InputMap& input);
    void update();
    const GameData& provideGameData() { return data; }
    void end();
};