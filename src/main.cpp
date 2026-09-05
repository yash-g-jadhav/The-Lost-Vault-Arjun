#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "[ERROR][core] Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const int initialWidth = 1280;
    const int initialHeight = 720;
    GLFWwindow* window = glfwCreateWindow(initialWidth, initialHeight, "The Lost Vault", nullptr, nullptr);
    if (!window) {
        std::cerr << "[ERROR][core] Failed to create GLFW window (OpenGL 3.3 Core profile required)" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR][core] Failed to initialize GLAD OpenGL loader" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "[INFO][core] OpenGL context initialized successfully: "
              << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, initialWidth, initialHeight);

    // #8FD3E8 sky blue background color from Design.md §3
    const float clearR = 143.0f / 255.0f;
    const float clearG = 211.0f / 255.0f;
    const float clearB = 232.0f / 255.0f;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(clearR, clearG, clearB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
