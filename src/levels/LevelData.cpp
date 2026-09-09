#include "levels/LevelData.h"
#include <fstream>
#include <sstream>

LevelData LevelData::CreateDefaultLevel1() {
    LevelData data;
    data.levelId = 1;
    data.name = "The Landing Shore";
    data.bounds = glm::vec4(0.0f, 0.0f, 40.0f, 24.0f);
    data.startPosition = glm::vec2(4.0f, 4.0f);
    data.gemsRequired = 3;
    data.totalGems = 4;

    // Sand background
    data.staticObjects.push_back({"sand", {20.0f, 12.0f}, {40.0f, 24.0f}, 0.0f, {0.94f, 0.86f, 0.63f, 1.0f}});
    // Water border at bottom
    data.staticObjects.push_back({"water", {20.0f, 1.0f}, {40.0f, 2.0f}, 0.0f, {0.17f, 0.54f, 0.76f, 1.0f}});
    // Grass area
    data.staticObjects.push_back({"grass", {20.0f, 13.0f}, {38.0f, 20.0f}, 0.0f, {0.36f, 0.65f, 0.36f, 1.0f}});

    // Trees
    data.staticObjects.push_back({"tree", {8.0f, 6.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}});
    data.staticObjects.push_back({"tree", {14.0f, 10.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}});
    data.staticObjects.push_back({"tree", {18.0f, 16.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}});
    data.staticObjects.push_back({"tree", {25.0f, 18.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}});
    data.staticObjects.push_back({"tree", {32.0f, 20.0f}, {0.6f, 2.0f}, 0.0f, {0.48f, 0.32f, 0.19f, 1.0f}});

    // Rocks
    data.staticObjects.push_back({"rock", {10.0f, 8.0f}, {1.2f, 1.0f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});
    data.staticObjects.push_back({"rock", {12.0f, 5.0f}, {1.0f, 1.0f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});
    data.staticObjects.push_back({"rock", {20.0f, 10.0f}, {1.5f, 1.2f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});
    data.staticObjects.push_back({"rock", {28.0f, 14.0f}, {1.1f, 0.9f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});
    data.staticObjects.push_back({"rock", {30.0f, 16.0f}, {1.3f, 1.1f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});
    data.staticObjects.push_back({"rock", {36.0f, 18.0f}, {1.4f, 1.2f}, 0.0f, {0.55f, 0.55f, 0.52f, 1.0f}});

    // Bushes
    data.staticObjects.push_back({"bush", {6.0f, 10.0f}, {0.8f, 0.8f}, 0.0f, {0.31f, 0.54f, 0.31f, 1.0f}});
    data.staticObjects.push_back({"bush", {22.0f, 6.0f}, {0.9f, 0.9f}, 0.0f, {0.31f, 0.54f, 0.31f, 1.0f}});

    return data;
}
