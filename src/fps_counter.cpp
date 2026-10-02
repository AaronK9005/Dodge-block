#include "fps_counter.hpp"

FpsCounter::FpsCounter() {}

FpsCounter::FpsCounter(unsigned measuresPerSecond)
    : measureFrequencyPerSecond(measuresPerSecond)
    , measureTime(1.0f / static_cast<float>(measuresPerSecond)) {}

FpsCounter::~FpsCounter() {}

void FpsCounter::setMeasureFrequency(unsigned measuresPerSecond)
{
    measureFrequencyPerSecond = measuresPerSecond;

    measureTime = 1.0f / static_cast<float>(measuresPerSecond);
}

void FpsCounter::update(float dt)
{
    timeAccumulator += dt;

    if (timeAccumulator >= measureTime)
    {
        timeAccumulator -= measureTime;

        lastFps = currentFrames;
        currentFrames = 0;
    }

    currentFrames++;
}

unsigned FpsCounter::getFps()
{
    return lastFps * measureFrequencyPerSecond;
}

