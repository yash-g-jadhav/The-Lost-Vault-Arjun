#pragma once

#include "entities/TransformComponent.h"
#include "entities/ColliderComponent.h"
#include <string>
#include <memory>

// GameObject: base entity with a name, a TransformComponent, and optional component slots.
class GameObject {
public:
    explicit GameObject(std::string name = "GameObject");
    virtual ~GameObject() = default;

    virtual void Update(double dt);

    const std::string& GetName() const { return name; }

    TransformComponent transform;
    std::unique_ptr<ColliderComponent> collider;

protected:
    std::string name;
};
