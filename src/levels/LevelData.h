#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

// Static object descriptor in level data
struct StaticObjectData {
    std::string type;     // "tree", "rock", "bush", "water", "sand", "path", etc.
    glm::vec2 position;
    glm::vec2 scale;
    float rotation{0.0f};
    glm::vec4 color{1.0f};
};

// LevelData: plain data struct containing raw parsed level parameters
struct LevelData {
    int levelId{1};
    std::string name{"Level 1 — The Landing Shore"};
    glm::vec4 bounds{0.0f, 0.0f, 40.0f, 24.0f}; // minX, minY, maxX, maxY
    glm::vec2 startPosition{4.0f, 4.0f};
    int gemsRequired{3};
    int totalGems{4};

    std::vector<StaticObjectData> staticObjects;

    static LevelData CreateDefaultLevel1();
};
