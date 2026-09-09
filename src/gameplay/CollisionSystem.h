#pragma once

#include "utils/AABB.h"
#include "entities/Player.h"
#include <vector>
#include <glm/glm.hpp>

// CollisionSystem: handles axis-separated AABB collision resolution (Rules.md §2).
// First resolves X-axis movements against solid obstacles, then Y-axis movements.
class CollisionSystem {
public:
    CollisionSystem() = default;

    // Resolves player position against a list of static solid AABB obstacles and level bounds.
    static void ResolvePlayerCollision(Player& player, const std::vector<AABB>& staticObstacles, const glm::vec4& levelBounds);

    // Resolves player position against static solid AABB obstacles.
    static void ResolvePlayerCollision(Player& player, const std::vector<AABB>& staticObstacles);

    // Axis-separated resolution for a single player box against a single obstacle box.
    // Updates playerPos and playerBox in-place.
    static bool ResolveX(AABB& playerBox, glm::vec2& playerPos, const AABB& obstacle);
    static bool ResolveY(AABB& playerBox, glm::vec2& playerPos, const AABB& obstacle);
};
