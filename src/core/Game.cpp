#include "core/Game.h"
#include "core/Log.h"
#include <glm/gtc/matrix_transform.hpp>

Game::Game() : isRunning(false) {}

Game::~Game() {
    if (isRunning) {
        Shutdown();
    }
}

bool Game::Initialize() {
    LOG_INFO("Game", "Initializing game systems...");

    if (!renderer.Init("assets/shaders")) {
        LOG_ERROR("Game", "Failed to initialize Renderer2D");
        return false;
    }

    isRunning = true;
    return true;
}

void Game::Update(double fixedDt) {
    (void)fixedDt;
    // Game state / logic updates
}

void Game::Render() {
    // Sky blue clear
    const glm::vec4 skyColor(143.0f / 255.0f, 211.0f / 255.0f, 232.0f / 255.0f, 1.0f);
    renderer.SetClearColor(skyColor);
    renderer.Clear();

    // 16:9 world view coordinates for demonstration
    glm::mat4 proj = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    renderer.BeginScene(proj);

    // Demonstration primitives for Phase 2:
    // Sand ground quad (#F0DCA0)
    const glm::vec4 sandColor(240.0f / 255.0f, 220.0f / 255.0f, 160.0f / 255.0f, 1.0f);
    renderer.DrawQuad(glm::vec2(0.0f, -3.0f), glm::vec2(16.0f, 3.0f), sandColor);

    // Grass island quad (#5BA65B)
    const glm::vec4 grassColor(91.0f / 255.0f, 166.0f / 255.0f, 91.0f / 255.0f, 1.0f);
    renderer.DrawQuad(glm::vec2(0.0f, -1.5f), glm::vec2(10.0f, 1.0f), grassColor);

    // Sun circle (#FFF3B0)
    const glm::vec4 sunColor(255.0f / 255.0f, 243.0f / 255.0f, 176.0f / 255.0f, 1.0f);
    renderer.DrawCircle(glm::vec2(5.5f, 3.0f), 1.0f, sunColor);

    // Red gem circle (#E63946)
    const glm::vec4 gemColor(230.0f / 255.0f, 57.0f / 255.0f, 70.0f / 255.0f, 1.0f);
    renderer.DrawCircle(glm::vec2(-2.0f, 0.0f), 0.5f, gemColor);

    // Tree trunk quad (#7A5230)
    const glm::vec4 trunkColor(122.0f / 255.0f, 82.0f / 255.0f, 48.0f / 255.0f, 1.0f);
    renderer.DrawQuad(glm::vec2(2.0f, 0.0f), glm::vec2(0.6f, 2.0f), trunkColor);

    // Tree foliage circle (#3D7A3D)
    const glm::vec4 foliageColor(61.0f / 255.0f, 122.0f / 255.0f, 61.0f / 255.0f, 1.0f);
    renderer.DrawCircle(glm::vec2(2.0f, 1.5f), 1.2f, foliageColor);

    renderer.EndScene();
}

void Game::Shutdown() {
    if (!isRunning) return;
    LOG_INFO("Game", "Shutting down game systems...");
    renderer.Shutdown();
    isRunning = false;
}
