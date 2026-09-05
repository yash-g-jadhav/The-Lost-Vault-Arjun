#pragma once

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void Update(double fixedDt);
    void Render();
    void Shutdown();

    bool IsRunning() const { return isRunning; }

private:
    bool isRunning;
};
