#include <glad/glad.h>
#include "entities/Player.h"
#include <GLFW/glfw3.h>
#include <cmath>

const glm::vec4 Player::BODY_COLOR{0.27f, 0.51f, 0.71f, 1.0f};
const glm::vec4 Player::HEAD_COLOR{0.96f, 0.76f, 0.57f, 1.0f};
const glm::vec4 Player::LEG_COLOR {0.18f, 0.24f, 0.42f, 1.0f};
const glm::vec4 Player::ARM_COLOR {0.96f, 0.76f, 0.57f, 1.0f};

// ─── Player constructor ────────────────────────────────────────────────────────
Player::Player() : GameObject("Player") {
    // Parent all body-part transforms to the body root (transform)
    headTransform.parent      = &transform;
    leftArmTransform.parent   = &transform;
    rightArmTransform.parent  = &transform;
    leftLegTransform.parent   = &transform;
    rightLegTransform.parent  = &transform;

    // ── Offsets relative to body-root origin (body centre) ──────────────────
    // Body root is the torso centre; origin at middle of the body.

    // Head: centred above torso
    headTransform.position = {0.0f,  0.55f};
    headTransform.scale    = {0.35f, 0.35f};

    // Arms: slightly below head, one either side
    leftArmTransform.position  = {-0.35f, 0.15f};
    leftArmTransform.scale     = {0.15f,  0.45f};
    rightArmTransform.position = { 0.35f, 0.15f};
    rightArmTransform.scale    = {0.15f,  0.45f};

    // Legs: below torso centre
    leftLegTransform.position  = {-0.15f, -0.45f};
    leftLegTransform.scale     = {0.15f,   0.45f};
    rightLegTransform.position = { 0.15f, -0.45f};
    rightLegTransform.scale    = {0.15f,   0.45f};

    // Body root scale (the torso rect)
    transform.scale = {0.45f, 0.55f};
}

// ─── Update forwarding ────────────────────────────────────────────────────────
void Player::Update(double dt) {
    // HandleInput is driven externally by Game; Update is a no-op unless
    // future phases add timers (invulnerability windows, etc.)
    (void)dt;
}

// ─── Input + movement ─────────────────────────────────────────────────────────
void Player::HandleInput(const InputManager& input, double dt) {
    float dx = 0.0f, dy = 0.0f;

    if (input.IsActionHeld(Action::MoveRight)) dx += 1.0f;
    if (input.IsActionHeld(Action::MoveLeft))  dx -= 1.0f;
    if (input.IsActionHeld(Action::MoveUp))    dy += 1.0f;
    if (input.IsActionHeld(Action::MoveDown))  dy -= 1.0f;

    // R18.3: opposite keys cancel → already handled by the sum above.

    // R1.1: Normalise diagonal movement so diagonal speed == cardinal speed.
    glm::vec2 dir(dx, dy);
    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0.001f) {
        dir /= len;
        state  = PlayerState::Walking;

        // R1.4: Update facing direction from last non-zero input.
        if      (std::fabs(dy) >= std::fabs(dx) && dy > 0.0f)  facing = FacingDirection::Up;
        else if (std::fabs(dy) >= std::fabs(dx) && dy < 0.0f)  facing = FacingDirection::Down;
        else if (dx < 0.0f)                                      facing = FacingDirection::Left;
        else                                                     facing = FacingDirection::Right;
    } else {
        state = PlayerState::Idle;
        // R1.4: facing retained — do not reset.
    }

    // Integrate position (R1.3: constant moveSpeed)
    velocity = dir * MOVE_SPEED;
    transform.position += velocity * static_cast<float>(dt);

    // R1.5: Bounds clamping is applied by LevelScene in later phases.
    // Here (Phase 3, no level bounds yet) the player is free to roam.

    // Tilt arms/legs slightly when walking — a subtle hierarchical animation
    // that demonstrates parent-chain composition without a full animator.
    if (state == PlayerState::Walking) {
        float swingAngle = 20.0f * std::sin(static_cast<float>(glfwGetTime()) * 8.0f);
        leftArmTransform.rotation  =  swingAngle;
        rightArmTransform.rotation = -swingAngle;
        leftLegTransform.rotation  = -swingAngle;
        rightLegTransform.rotation =  swingAngle;
    } else {
        leftArmTransform.rotation  = 0.0f;
        rightArmTransform.rotation = 0.0f;
        leftLegTransform.rotation  = 0.0f;
        rightLegTransform.rotation = 0.0f;
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────
// Each body part is submitted as a DrawQuad using its world matrix (which
// already incorporates the full parent chain, demonstrating hierarchical
// transforms as required by the academic brief).
void Player::Render(Renderer2D& renderer) const {
    // Torso (body root)
    renderer.DrawQuad(transform.GetWorldMatrix(), BODY_COLOR);

    // Head
    renderer.DrawQuad(headTransform.GetWorldMatrix(), HEAD_COLOR);

    // Arms
    renderer.DrawQuad(leftArmTransform.GetWorldMatrix(),  ARM_COLOR);
    renderer.DrawQuad(rightArmTransform.GetWorldMatrix(), ARM_COLOR);

    // Legs
    renderer.DrawQuad(leftLegTransform.GetWorldMatrix(),  LEG_COLOR);
    renderer.DrawQuad(rightLegTransform.GetWorldMatrix(), LEG_COLOR);
}
