#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TransformComponent: holds local position, rotation (degrees, Z-axis), and
// scale.  Supports a parent pointer so child transforms compose with their
// parent's world matrix (hierarchical transforms for player body/limbs).
class TransformComponent {
public:
    TransformComponent();

    // Local-space properties
    glm::vec2 position{0.0f, 0.0f};
    float     rotation{0.0f};   // degrees, CCW
    glm::vec2 scale{1.0f, 1.0f};

    // Optional parent (non-owning, raw pointer — lifecycle managed by caller)
    const TransformComponent* parent{nullptr};

    // Returns the local TRS matrix (T * R * S)
    glm::mat4 GetLocalMatrix() const;

    // Returns the world matrix (parent->world * local)
    glm::mat4 GetWorldMatrix() const;
};
