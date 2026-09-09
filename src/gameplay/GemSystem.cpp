#include "gameplay/GemSystem.h"
#include "entities/Player.h"
#include "gameplay/CollisionSystem.h"
#include "core/Log.h"

void GemSystem::Update(Player& player, const std::vector<AABB>& triggerBounds) {
    // Initialize gems on first update if not done yet
    if (gems.empty() && !triggerBounds.empty()) {
        for (size_t i = 0; i < triggerBounds.size(); ++i) {
            AddGem(triggerBounds[i], i);
        }
    }

    // Find which trigger bounds overlap with the player
    std::vector<size_t> overlappingIndices = CollisionSystem::DetectTriggerOverlaps(player, triggerBounds);
    for (size_t idx : overlappingIndices) {
        if (idx >= gems.size()) continue; // safety guard
        Gem& gem = gems[idx];
        if (!gem.collected) {
            gem.collected = true;
            ++collectedCount;
            player.IncrementCollectedGems();
            LOG_INFO("GemSystem", "Collected gem id=" + std::to_string(gem.id));
        }
    }
}
