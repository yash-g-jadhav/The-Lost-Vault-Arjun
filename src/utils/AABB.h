#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

// AABB: Axis-Aligned Bounding Box in 2D world space.
// Defined by a center point and half-extents (width/2, height/2).
struct AABB {
    glm::vec2 center{0.0f, 0.0f};
    glm::vec2 halfExtents{0.5f, 0.5f};

    AABB() = default;
    AABB(const glm::vec2& center, const glm::vec2& halfExtents)
        : center(center), halfExtents(halfExtents) {}

    // Constructs AABB from center position and full width/height size.
    static AABB FromCenterAndSize(const glm::vec2& center, const glm::vec2& size) {
        return AABB(center, size * 0.5f);
    }

    // Constructs AABB from min (bottom-left) and max (top-right) corners.
    static AABB FromMinMax(const glm::vec2& min, const glm::vec2& max) {
        glm::vec2 center = (min + max) * 0.5f;
        glm::vec2 halfExtents = (max - min) * 0.5f;
        return AABB(center, halfExtents);
    }

    glm::vec2 GetMin() const { return center - halfExtents; }
    glm::vec2 GetMax() const { return center + halfExtents; }
    glm::vec2 GetSize() const { return halfExtents * 2.0f; }

    // Returns true if this AABB overlaps another AABB (inclusive edge overlap counts as intersection).
    bool Intersects(const AABB& other) const {
        glm::vec2 delta = glm::abs(center - other.center);
        glm::vec2 totalHalf = halfExtents + other.halfExtents;
        return (delta.x <= totalHalf.x) && (delta.y <= totalHalf.y);
    }

    // Returns true if this AABB strictly overlaps another AABB (exclusive of exact boundary edge touch).
    bool StrictlyIntersects(const AABB& other) const {
        glm::vec2 delta = glm::abs(center - other.center);
        glm::vec2 totalHalf = halfExtents + other.halfExtents;
        return (delta.x < totalHalf.x) && (delta.y < totalHalf.y);
    }

    // Calculates the overlap depth on both axes.
    // If intersecting, overlap.x > 0 and overlap.y > 0 represent penetrating distance.
    glm::vec2 GetOverlap(const AABB& other) const {
        glm::vec2 delta = glm::abs(center - other.center);
        glm::vec2 totalHalf = halfExtents + other.halfExtents;
        float overlapX = totalHalf.x - delta.x;
        float overlapY = totalHalf.y - delta.y;
        return glm::vec2(std::max(0.0f, overlapX), std::max(0.0f, overlapY));
    }
};
