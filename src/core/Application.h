#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Game.h"
#include "core/Clock.h"

class Application {
public:
    Application();
    ~Application();

    bool Initialize(int width = 1280, int height = 720, const char* title = "The Lost Vault");
    void Run();
    void Shutdown();

    GLFWwindow* GetWindow() const { return window; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* window;
    int width;
    int height;
    Game game;
    Clock clock;
    bool initialized;
};
