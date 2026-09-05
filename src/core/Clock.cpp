#include "core/Clock.h"
#include <GLFW/glfw3.h>

Clock::Clock()
    : lastTime(0.0), totalTime(0.0), deltaTime(0.0),
      fpsTimer(0.0), frameCounter(0), currentFPS(0.0f) {}

void Clock::Init() {
    lastTime = glfwGetTime();
    totalTime = 0.0;
    deltaTime = 0.0;
    fpsTimer = lastTime;
    frameCounter = 0;
    currentFPS = 0.0f;
}

void Clock::Tick() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    totalTime += deltaTime;

    frameCounter++;
    if (currentTime - fpsTimer >= 1.0) {
        currentFPS = static_cast<float>(frameCounter) / static_cast<float>(currentTime - fpsTimer);
        frameCounter = 0;
        fpsTimer = currentTime;
    }
}
