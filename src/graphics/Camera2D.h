#pragma once

#include <glm/glm.hpp>

// Camera2D: orthographic 2D camera with target follow and level bounds clamping.
// Maintains world-space view dimensions and produces view-projection matrices
// for Renderer2D.
class Camera2D {
public:
    Camera2D(float viewWidth = 16.0f, float viewHeight = 9.0f);

    // Update camera position to follow a target (e.g. Player) with smooth lerp
    // and clamp the camera's visible rectangle within levelBounds (minX, minY, maxX, maxY).
    void Follow(const glm::vec2& targetPos, const glm::vec4& levelBounds, float dt = 0.0f);

    // Immediately snap camera center to position (clamped by level bounds).
    void SetPosition(const glm::vec2& pos, const glm::vec4& levelBounds);

    // Update view size based on aspect ratio or window size
    void SetViewSize(float width, float height);

    // Matrix getters
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

    glm::vec2 GetPosition() const { return position; }
    float GetViewWidth() const { return viewWidth; }
    float GetViewHeight() const { return viewHeight; }

private:
    glm::vec2 position{0.0f, 0.0f};
    float viewWidth{16.0f};
    float viewHeight{9.0f};

    // Helper: clamp position so view rect stays within bounds
    glm::vec2 ClampToBounds(const glm::vec2& rawPos, const glm::vec4& bounds) const;
};
