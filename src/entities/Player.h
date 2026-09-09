#pragma once

#include "entities/GameObject.h"
#include "input/InputManager.h"
#include "graphics/Renderer2D.h"
#include <glm/glm.hpp>

// Player state enum
enum class PlayerState {
    Idle,
    Walking
};

// Facing direction — retained when idle (Rules.md R1.4)
enum class FacingDirection {
    Up,
    Down,
    Left,
    Right
};

// Player: specialized GameObject with movement, facing, lives, and hierarchical
// body rendering (body root → head / left arm / right arm / left leg / right leg
// as child TransformComponents).  This satisfies the academic requirement for
// demonstrating hierarchical transformations.
class Player : public GameObject {
public:
    // Gameplay constants
    static constexpr float MOVE_SPEED   = 5.0f;   // world units / second
    static constexpr int   START_LIVES  = 3;

    Player();
    ~Player() override = default;

    // Called every fixed update (60 Hz).  Reads input and integrates position.
    void HandleInput(const InputManager& input, double dt);

    // Forwards to HandleInput; kept for uniformity with the Update chain.
    void Update(double dt) override;

    // Draws the player as a stick-figure sailor built from primitives.
    // Each body part is rendered using its own world matrix derived from the
    // TransformComponent parent chain (hierarchical transforms).
    void Render(Renderer2D& renderer) const;

    // Accessors
    glm::vec2      GetPosition() const { return transform.position; }
    PlayerState    GetState()    const { return state; }
    FacingDirection GetFacing()  const { return facing; }
    int collectedGemsThisLevel{0};
    int GetCollectedGems() const { return collectedGemsThisLevel; }
    void IncrementCollectedGems() { ++collectedGemsThisLevel; }

    // Mutators used by LivesSystem / CheckpointSystem (later phases)
    void SetPosition(const glm::vec2& pos) { transform.position = pos; }
    void SetLives(int l)                   { lives = l; }

private:
    // ── Gameplay state ───────────────────────────────────────────────────────
    glm::vec2       velocity{0.0f, 0.0f};
    FacingDirection facing{FacingDirection::Down};
    PlayerState     state{PlayerState::Idle};
    int             lives{START_LIVES};

    // ── Child transforms for hierarchical body parts ─────────────────────────
    // Each is parented to transform (the body root).
    TransformComponent headTransform;
    TransformComponent leftArmTransform;
    TransformComponent rightArmTransform;
    TransformComponent leftLegTransform;
    TransformComponent rightLegTransform;

    // ── Colours ──────────────────────────────────────────────────────────────
    static const glm::vec4 BODY_COLOR;
    static const glm::vec4 HEAD_COLOR;
    static const glm::vec4 LEG_COLOR;
    static const glm::vec4 ARM_COLOR;
};
