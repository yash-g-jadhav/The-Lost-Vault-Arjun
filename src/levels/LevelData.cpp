#include "levels/LevelData.h"
#include <fstream>
#include <sstream>

std::vector<AABB> LevelData::GetSolidObstacles() const {
    std::vector<AABB> obstacles;
    for (const auto& obj : staticObjects) {
        if (!obj.isSolid) continue;

        if (obj.type == "tree") {
            // Design.md §7: Tree trunk AABB ~0.3 x 0.4 near the base
            obstacles.push_back(AABB(obj.position, {0.15f, 0.2f}));
        } else if (obj.type == "rock") {
            // Design.md §8: Rock AABB tightly fit to polygon bounding box (~80% scale)
            obstacles.push_back(AABB(obj.position, obj.scale * 0.4f));
        } else {
            // Default solid AABB covering full scale
            obstacles.push_back(AABB(obj.position, obj.scale * 0.5f));
        }
    }
    return obstacles;
}

LevelData LevelData::CreateDefaultLevel1() {
    LevelData data;
    data.levelId = 1;
    data.name = "The Landing Shore";
    data.bounds = glm::vec4(0.0f, 0.0f, 40.0f, 24.0f);
    data.startPosition = glm::vec2(4.0f, 4.0f);
    data.gemsRequired = 3;
    data.totalGems = 4;

    // Sand background (non-solid)
    data.staticObjects.push_back({"sand", {20.0f, 12.0f}, {40.0f, 24.0f}, 0.0f, {0.94f, 0.86f, 0.63f, 1.0f}, false});
    // Water border at bottom (solid obstacle)
    data.staticObjects.push_back({"water", {20.0f, 1.0f}, {40.0f, 2.0f}, 0.0f, {0.17f, 0.54f, 0.76f, 1.0f}, true});
    // Grass area (non-solid)
    data.staticObjects.push_back({"grass", {20.0f, 13.0f}, {38.0f, 20.0f}, 0.0f, {0.36f, 0.65f, 0.36f, 1.0f}, false});

    // Trees (solid trunk)
    data.staticObjects.push_back({"tree", {8.0f, 6.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}, true});
    data.staticObjects.push_back({"tree", {14.0f, 10.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}, true});
    data.staticObjects.push_back({"tree", {18.0f, 16.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}, true});
    data.staticObjects.push_back({"tree", {25.0f, 18.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}, true});
    data.staticObjects.push_back({"tree", {32.0f, 20.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}, true});

    // Rocks (solid)
    data.staticObjects.push_back({"rock", {10.0f, 8.0f}, {1.2f, 1.0f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});
    data.staticObjects.push_back({"rock", {12.0f, 5.0f}, {1.0f, 1.0f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});
    data.staticObjects.push_back({"rock", {20.0f, 10.0f}, {1.5f, 1.2f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});
    data.staticObjects.push_back({"rock", {28.0f, 14.0f}, {1.1f, 0.9f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});
    data.staticObjects.push_back({"rock", {30.0f, 16.0f}, {1.3f, 1.1f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});
    data.staticObjects.push_back({"rock", {36.0f, 18.0f}, {1.4f, 1.2f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}, true});

    // Bushes (non-solid visual detail)
    data.staticObjects.push_back({"bush", {6.0f, 10.0f}, {0.8f, 0.8f}, 0.0f, {0.31f, 0.54f, 0.31f, 1.0f}, false});
    data.staticObjects.push_back({"bush", {22.0f, 6.0f}, {0.9f, 0.9f}, 0.0f, {0.31f, 0.54f, 0.31f, 1.0f}, false});

    return data;
}
