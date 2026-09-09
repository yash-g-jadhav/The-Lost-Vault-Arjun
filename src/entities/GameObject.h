#pragma once

#include "entities/TransformComponent.h"
#include <string>

// GameObject: base entity with a name and a TransformComponent.
// Concrete component slots (sprite, collider, animator, interactable) will be
// added in later phases as unique_ptr members here.
class GameObject {
public:
    explicit GameObject(std::string name = "GameObject");
    virtual ~GameObject() = default;

    virtual void Update(double dt);

    const std::string& GetName() const { return name; }

    TransformComponent transform;

protected:
    std::string name;
};
