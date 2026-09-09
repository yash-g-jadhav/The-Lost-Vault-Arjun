#include "graphics/Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera2D::Camera2D(float width, float height)
    : position(0.0f, 0.0f), viewWidth(width), viewHeight(height) {}

void Camera2D::SetViewSize(float width, float height) {
    viewWidth = width;
    viewHeight = height;
}

glm::vec2 Camera2D::ClampToBounds(const glm::vec2& rawPos, const glm::vec4& bounds) const {
    // bounds format: (minX, minY, maxX, maxY)
    float minX = bounds.x;
    float minY = bounds.y;
    float maxX = bounds.z;
    float maxY = bounds.w;

    float halfW = viewWidth * 0.5f;
    float halfH = viewHeight * 0.5f;

    float clampedX = rawPos.x;
    float clampedY = rawPos.y;

    // Clamp X
    if (maxX - minX <= viewWidth) {
        // Level is narrower than viewport -> center camera horizontally
        clampedX = (minX + maxX) * 0.5f;
    } else {
        clampedX = glm::clamp(rawPos.x, minX + halfW, maxX - halfW);
    }

    // Clamp Y
    if (maxY - minY <= viewHeight) {
        // Level is shorter than viewport -> center camera vertically
        clampedY = (minY + maxY) * 0.5f;
    } else {
        clampedY = glm::clamp(rawPos.y, minY + halfH, maxY - halfH);
    }

    return glm::vec2(clampedX, clampedY);
}

void Camera2D::Follow(const glm::vec2& targetPos, const glm::vec4& levelBounds, float dt) {
    if (dt <= 0.0f) {
        SetPosition(targetPos, levelBounds);
        return;
    }

    // Smooth lerp (k = 8.0 per Design.md §19)
    float lerpFactor = 1.0f - std::exp(-8.0f * dt);
    glm::vec2 newPos = position + (targetPos - position) * lerpFactor;

    position = ClampToBounds(newPos, levelBounds);
}

void Camera2D::SetPosition(const glm::vec2& pos, const glm::vec4& levelBounds) {
    position = ClampToBounds(pos, levelBounds);
}

glm::mat4 Camera2D::GetProjectionMatrix() const {
    float halfW = viewWidth * 0.5f;
    float halfH = viewHeight * 0.5f;
    return glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);
}

glm::mat4 Camera2D::GetViewMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, 0.0f));
}

glm::mat4 Camera2D::GetViewProjectionMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}
