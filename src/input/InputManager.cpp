#include <glad/glad.h>
#include "input/InputManager.h"
#include <GLFW/glfw3.h>

InputManager::InputManager() {
    currentState.fill(false);
    previousState.fill(false);
}

// Primary key binding per Action
int InputManager::PrimaryKey(Action action) {
    switch (action) {
        case Action::MoveUp:    return GLFW_KEY_W;
        case Action::MoveDown:  return GLFW_KEY_S;
        case Action::MoveLeft:  return GLFW_KEY_A;
        case Action::MoveRight: return GLFW_KEY_D;
        case Action::Interact:  return GLFW_KEY_SPACE;
        case Action::Pause:     return GLFW_KEY_ESCAPE;
        case Action::Confirm:   return GLFW_KEY_ENTER;
        case Action::MenuUp:    return GLFW_KEY_W;
        case Action::MenuDown:  return GLFW_KEY_S;
        default:                return -1;
    }
}

// Secondary (arrow-key) bindings for movement; -1 = no secondary
int InputManager::SecondaryKey(Action action) {
    switch (action) {
        case Action::MoveUp:    return GLFW_KEY_UP;
        case Action::MoveDown:  return GLFW_KEY_DOWN;
        case Action::MoveLeft:  return GLFW_KEY_LEFT;
        case Action::MoveRight: return GLFW_KEY_RIGHT;
        case Action::MenuUp:    return GLFW_KEY_UP;
        case Action::MenuDown:  return GLFW_KEY_DOWN;
        default:                return -1;
    }
}

void InputManager::PollState(GLFWwindow* window) {
    previousState = currentState;

    for (int i = 0; i < ACTION_COUNT; ++i) {
        Action a = static_cast<Action>(i);
        int primary   = PrimaryKey(a);
        int secondary = SecondaryKey(a);

        bool held = false;
        if (primary   != -1) held = held || (glfwGetKey(window, primary)   == GLFW_PRESS);
        if (secondary != -1) held = held || (glfwGetKey(window, secondary) == GLFW_PRESS);
        currentState[i] = held;
    }
}

bool InputManager::IsActionHeld(Action action) const {
    return currentState[static_cast<int>(action)];
}

bool InputManager::WasActionPressed(Action action) const {
    int i = static_cast<int>(action);
    return currentState[i] && !previousState[i];
}
