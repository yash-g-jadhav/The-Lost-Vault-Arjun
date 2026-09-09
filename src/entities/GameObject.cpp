#include "entities/GameObject.h"

GameObject::GameObject(std::string name)
    : name(std::move(name)) {}

void GameObject::Update(double dt) {
    (void)dt;
    // Base implementation is a no-op; subclasses override.
}
