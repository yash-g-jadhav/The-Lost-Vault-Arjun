#include "core/Game.h"
#include "core/Log.h"
#include <glad/glad.h>

Game::Game() : isRunning(false) {}

Game::~Game() {
    if (isRunning) {
        Shutdown();
    }
}

bool Game::Initialize() {
    LOG_INFO("Game", "Initializing game systems...");
    isRunning = true;
    return true;
}

void Game::Update(double fixedDt) {
    (void)fixedDt;
    // Gameplay systems update logic will be wired here in future phases
}

void Game::Render() {
    // #8FD3E8 sky blue background color from Design.md §3
    const float clearR = 143.0f / 255.0f;
    const float clearG = 211.0f / 255.0f;
    const float clearB = 232.0f / 255.0f;

    glClearColor(clearR, clearG, clearB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::Shutdown() {
    if (!isRunning) return;
    LOG_INFO("Game", "Shutting down game systems...");
    isRunning = false;
}
