#pragma once

#include "entities/Component.h"
#include "utils/AABB.h"

// ColliderComponent: attached to GameObject to provide AABB bounding boxes
// for physical collision or trigger detection.
class ColliderComponent : public Component {
public:
    glm::vec2 offset{0.0f, 0.0f};
    glm::vec2 halfExtents{0.5f, 0.5f};
    bool isTrigger{false}; // if true, overlapping reports event without physical resolution
    bool isSolid{true};    // if true and not trigger, blocks movement

    ColliderComponent() = default;
    ColliderComponent(const glm::vec2& halfExtents, bool isTrigger = false, const glm::vec2& offset = {0.0f, 0.0f})
        : offset(offset), halfExtents(halfExtents), isTrigger(isTrigger), isSolid(!isTrigger) {}

    // Computes world-space AABB given the owner's position
    AABB GetWorldBounds(const glm::vec2& ownerPosition) const {
        return AABB(ownerPosition + offset, halfExtents);
    }
};
