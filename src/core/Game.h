#pragma once

#include "graphics/Renderer2D.h"
#include "gameplay/GemSystem.h"
#include "graphics/Camera2D.h"
#include "input/InputManager.h"
#include "entities/Player.h"
#include "levels/LevelData.h"

class GLFWwindow;

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void Update(double fixedDt);
    void Render();
    void Shutdown();

    // Called by Application once per frame (before Update) to snapshot input.
    void PollInput(GLFWwindow* window);

    bool IsRunning() const { return isRunning; }
    Renderer2D&    GetRenderer()      { return renderer; }
    InputManager&  GetInputManager()  { return inputManager; }
    Camera2D&      GetCamera()        { return camera; }

private:
    bool         isRunning;
    Renderer2D   renderer;
    InputManager inputManager;
    Camera2D     camera;
    Player       player;
    GemSystem    gemSystem;
    LevelData    currentLevel;
};
