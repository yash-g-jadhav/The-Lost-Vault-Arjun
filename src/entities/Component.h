#pragma once

// Abstract base for all component types attached to GameObjects.
// Concrete components implement Update and/or override as needed.
class Component {
public:
    virtual ~Component() = default;
    virtual void Update(double dt) { (void)dt; }
};
