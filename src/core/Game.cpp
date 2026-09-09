#include <glad/glad.h>
#include "core/Game.h"
#include "core/Log.h"
#include "gameplay/CollisionSystem.h"
#include <GLFW/glfw3.h>
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

    // Load Level 1 data
    currentLevel = LevelData::CreateDefaultLevel1();

    // Place player at level start position
    player.SetPosition(currentLevel.startPosition);

    // Initialize camera position to player start position clamped by level bounds
    camera.SetPosition(player.GetPosition(), currentLevel.bounds);

    isRunning = true;
    return true;
}

void Game::PollInput(GLFWwindow* window) {
    inputManager.PollState(window);
}

void Game::Update(double fixedDt) {
    // Movement input read every fixed update tick (60Hz)
    player.HandleInput(inputManager, fixedDt);

    // Resolve solid obstacle collision & level bounds clamping
    CollisionSystem::ResolvePlayerCollision(player, currentLevel.GetSolidObstacles(), currentLevel.bounds);
    // Process trigger overlaps (gems, etc.)
    gemSystem.Update(player, currentLevel.GetTriggerBounds());

    // Camera follows player, clamped to level bounds
    camera.Follow(player.GetPosition(), currentLevel.bounds, static_cast<float>(fixedDt));
}

void Game::Render() {
    // Sky blue clear
    const glm::vec4 skyColor(143.0f / 255.0f, 211.0f / 255.0f, 232.0f / 255.0f, 1.0f);
    renderer.SetClearColor(skyColor);
    renderer.Clear();

    // ── World-space pass with Camera2D ViewProjection matrix ──────────────
    renderer.BeginScene(camera.GetViewProjectionMatrix());

    // Draw static level objects
    for (const auto& obj : currentLevel.staticObjects) {
        if (obj.type == "sun" || obj.type == "foliage" || obj.type == "circle") {
            renderer.DrawCircle(obj.position, obj.scale.x * 0.5f, obj.color);
        } else if (obj.type == "tree") {
            // Draw tree trunk and foliage overlay
            renderer.DrawQuad(obj.position, obj.scale, obj.color);
            glm::vec4 foliageColor(61.0f / 255.0f, 122.0f / 255.0f, 61.0f / 255.0f, 1.0f);
            renderer.DrawCircle(obj.position + glm::vec2(0.0f, obj.scale.y * 0.75f), obj.scale.y * 0.6f, foliageColor);
        } else {
            renderer.DrawQuad(obj.position, obj.scale, obj.color);
        }
    }

    // Player (hierarchical body parts)
    player.Render(renderer);

    renderer.EndScene();
}

void Game::Shutdown() {
    if (!isRunning) return;
    LOG_INFO("Game", "Shutting down game systems...");
    renderer.Shutdown();
    isRunning = false;
}
