#pragma once

class Clock {
public:
    Clock();

    void Init();
    void Tick();

    double GetDeltaTime() const { return deltaTime; }
    double GetTotalTime() const { return totalTime; }
    float GetFPS() const { return currentFPS; }

private:
    double lastTime;
    double totalTime;
    double deltaTime;

    double fpsTimer;
    int frameCounter;
    float currentFPS;
};
