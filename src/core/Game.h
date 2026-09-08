#pragma once

#include "graphics/Renderer2D.h"

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void Update(double fixedDt);
    void Render();
    void Shutdown();

    bool IsRunning() const { return isRunning; }
    Renderer2D& GetRenderer() { return renderer; }

private:
    bool isRunning;
    Renderer2D renderer;
};
