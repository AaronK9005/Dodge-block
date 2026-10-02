#pragma once

class FpsCounter
{
    unsigned measureFrequencyPerSecond = 2;
    
    float timeAccumulator = 0.0f;
    float measureTime = 0.5f;

    unsigned currentFrames = 0;
    unsigned lastFps = 0;
public:
    FpsCounter();
    FpsCounter(unsigned measuresPerSecond);
    ~FpsCounter();
    void setMeasureFrequency(unsigned measuresPerSecond);
    void update(float dt);
    unsigned getFps();
};