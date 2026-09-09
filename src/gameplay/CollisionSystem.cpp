#include "gameplay/CollisionSystem.h"
#include <algorithm>
#include <cmath>

bool CollisionSystem::ResolveX(AABB& playerBox, glm::vec2& playerPos, const AABB& obstacle) {
    if (!playerBox.Intersects(obstacle)) return false;

    glm::vec2 delta = playerBox.center - obstacle.center;
    glm::vec2 totalHalf = playerBox.halfExtents + obstacle.halfExtents;
    float overlapX = totalHalf.x - std::abs(delta.x);
    float overlapY = totalHalf.y - std::abs(delta.y);

    // Only resolve on X if overlap is non-zero
    if (overlapX > 0.0001f && overlapY > 0.0001f) {
        // Cap correction per frame (R2.4: no unbounded teleport)
        float correctionX = std::min(overlapX, 1.0f);
        if (delta.x < 0.0f) {
            playerPos.x -= correctionX;
        } else {
            playerPos.x += correctionX;
        }
        playerBox.center.x = playerPos.x;
        return true;
    }
    return false;
}

bool CollisionSystem::ResolveY(AABB& playerBox, glm::vec2& playerPos, const AABB& obstacle) {
    if (!playerBox.Intersects(obstacle)) return false;

    glm::vec2 delta = playerBox.center - obstacle.center;
    glm::vec2 totalHalf = playerBox.halfExtents + obstacle.halfExtents;
    float overlapX = totalHalf.x - std::abs(delta.x);
    float overlapY = totalHalf.y - std::abs(delta.y);

    if (overlapX > 0.0001f && overlapY > 0.0001f) {
        // Cap correction per frame (R2.4)
        float correctionY = std::min(overlapY, 1.0f);
        if (delta.y < 0.0f) {
            playerPos.y -= correctionY;
        } else {
            playerPos.y += correctionY;
        }
        playerBox.center.y = playerPos.y;
        return true;
    }
    return false;
}

void CollisionSystem::ResolvePlayerCollision(Player& player, const std::vector<AABB>& staticObstacles) {
    if (!player.collider) return;

    glm::vec2 playerPos = player.GetPosition();
    AABB playerBox = player.collider->GetWorldBounds(playerPos);

    // Pass 1: X-axis resolution against all solid obstacles
    for (const auto& obstacle : staticObstacles) {
        ResolveX(playerBox, playerPos, obstacle);
    }

    // Re-sync box after X pass
    playerBox = player.collider->GetWorldBounds(playerPos);

    // Pass 2: Y-axis resolution against all solid obstacles
    for (const auto& obstacle : staticObstacles) {
        ResolveY(playerBox, playerPos, obstacle);
    }

    player.SetPosition(playerPos);
}

void CollisionSystem::ResolvePlayerCollision(Player& player, const std::vector<AABB>& staticObstacles, const glm::vec4& levelBounds) {
    // Resolve static obstacle collisions first
    ResolvePlayerCollision(player, staticObstacles);

    // Enforce Level Bounds (R1.5: bounds act as implicit solid wall on all 4 sides)
    if (!player.collider) return;

    glm::vec2 playerPos = player.GetPosition();
    AABB playerBox = player.collider->GetWorldBounds(playerPos);

    float minX = levelBounds.x;
    float minY = levelBounds.y;
    float maxX = levelBounds.z;
    float maxY = levelBounds.w;

    float halfW = playerBox.halfExtents.x;
    float halfH = playerBox.halfExtents.y;

    playerPos.x = glm::clamp(playerPos.x, minX + halfW, maxX - halfW);
    playerPos.y = glm::clamp(playerPos.y, minY + halfH, maxY - halfH);

    player.SetPosition(playerPos);
}

// Detect trigger overlaps (e.g., gems) without position correction.
std::vector<size_t> CollisionSystem::DetectTriggerOverlaps(Player& player, const std::vector<AABB>& triggerBounds) {
    std::vector<size_t> overlappingIndices;
    if (!player.collider) return overlappingIndices;
    glm::vec2 playerPos = player.GetPosition();
    AABB playerBox = player.collider->GetWorldBounds(playerPos);
    for (size_t i = 0; i < triggerBounds.size(); ++i) {
        if (playerBox.Intersects(triggerBounds[i])) {
            overlappingIndices.push_back(i);
        }
    }
    return overlappingIndices;
}
