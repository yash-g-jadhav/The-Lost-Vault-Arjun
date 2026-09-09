#include "utils/AABB.h"
#include "entities/Player.h"
#include "entities/ColliderComponent.h"
#include "gameplay/CollisionSystem.h"
#include <iostream>
#include <cassert>
#include <vector>

void TestAABBBasics() {
    AABB box1 = AABB::FromCenterAndSize({0.0f, 0.0f}, {2.0f, 2.0f}); // center (0,0), halfExtents (1,1)
    AABB box2 = AABB::FromCenterAndSize({1.5f, 0.0f}, {2.0f, 2.0f}); // center (1.5,0), halfExtents (1,1)
    AABB box3 = AABB::FromCenterAndSize({3.0f, 0.0f}, {2.0f, 2.0f}); // center (3.0,0), halfExtents (1,1)

    assert(box1.Intersects(box2) == true);
    assert(box1.Intersects(box3) == false);

    glm::vec2 overlap = box1.GetOverlap(box2);
    assert(std::abs(overlap.x - 0.5f) < 0.001f);
    assert(std::abs(overlap.y - 2.0f) < 0.001f);

    std::cout << "[PASS] TestAABBBasics" << std::endl;
}

void TestAxisSeparatedCollisionResolution() {
    Player player;
    player.SetPosition({0.0f, 0.0f});
    // Player collider halfExtents {0.3, 0.6}

    // Place an obstacle at (0.5, 0.0) with halfExtents {0.5, 0.5}
    std::vector<AABB> obstacles = { AABB({0.5f, 0.0f}, {0.5f, 0.5f}) };

    // Before collision resolution, player and obstacle overlap
    AABB playerBox = player.collider->GetWorldBounds(player.GetPosition());
    assert(playerBox.Intersects(obstacles[0]) == true);

    // Resolve collision
    CollisionSystem::ResolvePlayerCollision(player, obstacles);

    // Player position should be pushed left to x <= -0.3f
    glm::vec2 newPos = player.GetPosition();
    assert(newPos.x <= -0.3f);

    // After resolution, player box no longer intersects obstacle
    AABB resolvedBox = player.collider->GetWorldBounds(newPos);
    assert(resolvedBox.StrictlyIntersects(obstacles[0]) == false);

    std::cout << "[PASS] TestAxisSeparatedCollisionResolution" << std::endl;
}

void TestLevelBoundsClamping() {
    Player player;
    player.SetPosition({-5.0f, 30.0f}); // Far outside bounds
    glm::vec4 bounds{0.0f, 0.0f, 40.0f, 24.0f};

    std::vector<AABB> obstacles;
    CollisionSystem::ResolvePlayerCollision(player, obstacles, bounds);

    glm::vec2 clampedPos = player.GetPosition();
    AABB box = player.collider->GetWorldBounds(clampedPos);

    assert(box.GetMin().x >= bounds.x - 0.001f);
    assert(box.GetMin().y >= bounds.y - 0.001f);
    assert(box.GetMax().x <= bounds.z + 0.001f);
    assert(box.GetMax().y <= bounds.w + 0.001f);

    std::cout << "[PASS] TestLevelBoundsClamping" << std::endl;
}

int main() {
    std::cout << "Running Phase 5 Collision Tests..." << std::endl;
    TestAABBBasics();
    TestAxisSeparatedCollisionResolution();
    TestLevelBoundsClamping();
    std::cout << "All Phase 5 Collision Tests PASSED!" << std::endl;
    return 0;
}
