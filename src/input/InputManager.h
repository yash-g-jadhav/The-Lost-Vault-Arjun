#pragma once

struct GLFWwindow;
#include <array>

// Action enum: all gameplay-relevant input actions.
// Gameplay code always queries Actions — never raw GLFW_KEY_* constants.
enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Interact,
    Pause,
    Confirm,
    MenuUp,
    MenuDown,
    COUNT  // sentinel — keep last
};

// InputManager: wraps GLFW key polling into an Action-based API.
// Call PollState(window) once per frame before any gameplay queries.
class InputManager {
public:
    InputManager();

    // Snapshot current GLFW key state. Call once per frame (before Update).
    void PollState(GLFWwindow* window);

    // True every frame the action key is physically held down.
    bool IsActionHeld(Action action) const;

    // True only on the frame the action key transitioned from up → down.
    bool WasActionPressed(Action action) const;

private:
    static constexpr int ACTION_COUNT = static_cast<int>(Action::COUNT);

    // Maps each Action to its primary and optional secondary GLFW key code.
    static int PrimaryKey(Action action);
    static int SecondaryKey(Action action);  // returns -1 if none

    std::array<bool, ACTION_COUNT> currentState{};
    std::array<bool, ACTION_COUNT> previousState{};
};
