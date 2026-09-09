#include "core/Application.h"
#include "core/Log.h"
#include <iostream>
#include <cmath>

Application::Application()
    : window(nullptr), width(1280), height(720), initialized(false) {}

Application::~Application() {
    if (initialized) {
        Shutdown();
    }
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->width = width;
        app->height = height;
    }
    glViewport(0, 0, width, height);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;
    // Escape handling is now delegated to InputManager / GameStateManager.
    // Only handle OS-level close (Alt+F4, window X button) via glfwWindowShouldClose.
    (void)window; (void)key; (void)action;
}

bool Application::Initialize(int w, int h, const char* title) {
    width = w;
    height = h;

    Log::Init();
    LOG_INFO("Application", "Initializing GLFW window...");

    if (!glfwInit()) {
        LOG_ERROR("Application", "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Application", "Failed to create GLFW window (OpenGL 3.3 Core profile required)");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Application", "Failed to initialize GLAD OpenGL loader");
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    const GLubyte* glVersion = glGetString(GL_VERSION);
    if (glVersion) {
        LOG_INFO("Application", "OpenGL Context created: " << glVersion);
    } else {
        LOG_INFO("Application", "OpenGL Context created (OpenGL 3.3 Core)");
    }

    glViewport(0, 0, width, height);

    if (!game.Initialize()) {
        LOG_ERROR("Application", "Failed to initialize Game systems");
        return false;
    }

    clock.Init();
    initialized = true;
    return true;
}

void Application::Run() {
    if (!initialized) {
        LOG_ERROR("Application", "Cannot run Application before successful initialization");
        return;
    }

    LOG_INFO("Application", "Starting main loop...");

    const double FIXED_DT = 1.0 / 60.0;
    const double MAX_FRAMETIME = 0.25; // Clamp to avoid runaway "spiral of death"

    double accumulator = 0.0;
    float lastLogFps = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        clock.Tick();

        double frameTime = clock.GetDeltaTime();
        if (frameTime > MAX_FRAMETIME) {
            frameTime = MAX_FRAMETIME;
        }

        accumulator += frameTime;
        glfwPollEvents();

        // Snapshot input state once per frame (before any fixed updates)
        game.PollInput(window);

        while (accumulator >= FIXED_DT) {
            game.Update(FIXED_DT);
            accumulator -= FIXED_DT;
        }

        game.Render();
        glfwSwapBuffers(window);

        // Log FPS periodically in debug/info mode when updated
        if (std::fabs(clock.GetFPS() - lastLogFps) > 5.0f && clock.GetFPS() > 0.0f) {
            LOG_DEBUG("Application", "FPS: " << clock.GetFPS());
            lastLogFps = clock.GetFPS();
        }
    }

    LOG_INFO("Application", "Main loop ended");
}

void Application::Shutdown() {
    if (!initialized) return;

    LOG_INFO("Application", "Shutting down Application...");
    game.Shutdown();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
    initialized = false;
}
