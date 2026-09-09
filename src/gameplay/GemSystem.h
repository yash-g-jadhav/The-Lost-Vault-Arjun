#pragma once

#include <vector>
#include "utils/AABB.h"

class Player;

class GemSystem {
public:
    GemSystem() = default;
    void AddGem(const AABB& bounds, size_t id) {
        gems.push_back({bounds, id, false});
    }
    void Update(Player& player, const std::vector<AABB>& triggerBounds);
    int GetCollectedCount() const { return collectedCount; }
private:
    struct Gem {
        AABB bounds;
        size_t id;
        bool collected{false};
    };
    std::vector<Gem> gems;
    int collectedCount{0};
};
