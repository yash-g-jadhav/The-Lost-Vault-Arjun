#include "entities/TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

TransformComponent::TransformComponent()
    : position(0.0f, 0.0f), rotation(0.0f), scale(1.0f, 1.0f), parent(nullptr) {}

glm::mat4 TransformComponent::GetLocalMatrix() const {
    // T * R * S (standard TRS composition)
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    if (rotation != 0.0f) {
        mat = glm::rotate(mat, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    mat = glm::scale(mat, glm::vec3(scale, 1.0f));
    return mat;
}

glm::mat4 TransformComponent::GetWorldMatrix() const {
    if (parent) {
        return parent->GetWorldMatrix() * GetLocalMatrix();
    }
    return GetLocalMatrix();
}
