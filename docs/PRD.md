# PRD.md — The Lost Vault (Product Requirements Document)

> **Authority note:** For product/feature scope, this document governs. For exact gameplay *behavior* (edge cases, sequencing, what-happens-when), `Rules.md` is authoritative and overrides this document if there is ever a conflict.

---

## 1. Product Overview

**The Lost Vault** is a single-player, desktop 2D treasure-hunt adventure game built with C++ and OpenGL 3.3 (core profile), using GLFW for windowing/input, GLAD for the OpenGL loader, and GLM for math. The player controls a shipwrecked sailor who explores a mysterious island across three levels, collects gemstones, discovers clues, unlocks progression, finds the Lost Vault, retrieves the treasure, and escapes on a rescue boat.

The project is simultaneously:
1. A playable, self-contained game.
2. A teaching vehicle for 2D computer graphics fundamentals (transformations, orthographic projection, shaders, rasterization concepts, animation).

## 2. Vision

Deliver a small, polished, fully hand-rolled OpenGL 2D game that a student team can build incrementally, understand completely, and defend line-by-line in an academic evaluation — while still feeling like a real, satisfying treasure-hunt adventure.

## 3. Goals

- Ship a complete, winnable game with 3 levels, a start-to-finish narrative arc, and a victory screen.
- Demonstrate core CG concepts explicitly and traceably (see `Design.md` §9 and `Architecture.md` §2).
- Keep the codebase small enough for a student team (~5,000–9,000 lines of C++) to fully understand.
- Support incremental, AI-assisted development via Antigravity, with clear phase gates (`Phases.md`).
- Avoid any external game engine; the rendering, input, and game loop are hand-written.

## 4. Non-Goals

- No multiplayer or networking.
- No physically-based physics engine (only simple AABB/circle collision + kinematic movement).
- No procedural level generation (levels are hand-authored data).
- No mobile/console ports.
- No monetization, analytics, or telemetry.
- No full ECS framework — a lightweight composition-based GameObject/Component model is used instead.
- No advanced lighting/3D pipeline (project is strictly 2D, orthographic).

## 5. Target Users

- Primary: the student development team building and evaluating this project for a Computer Graphics / Software Engineering course.
- Secondary: an instructor/evaluator who will read the code and watch/play the game.
- Tertiary: casual players who enjoy short (20–40 minute) 2D exploration games.

## 6. Game Concept & Story

A sailor's ship is wrecked in a storm and runs aground on an uncharted island. Waking on the beach, the sailor must:

1. **Level 1 — The Landing Shore:** Explore the beach and nearby jungle edge, collect gems, find the first clue (a fragment of a map/journal), and locate the path inland.
2. **Level 2 — The Overgrown Interior:** Navigate a denser, more hazardous jungle/ruins area with multiple paths, more gems, additional clue fragments, and environmental hazards (e.g., quicksand patch, patrolling hazard, spike trap) that can cost a life.
3. **Level 3 — The Vault Island:** Reach the final area containing the Lost Vault. Combine the clue fragments to unlock the vault, collect the required number of gems as a key requirement, retrieve the treasure, and reach the rescue boat to trigger the victory sequence.

## 7. Core Gameplay Loop

1. Move and explore the current level.
2. Avoid/overcome obstacles and hazards; do not lose all 3 lives.
3. Collect gems (used as a soft currency / gating requirement).
4. Find and collect clue objects (used as hard gating requirement for the vault).
5. Reach the level's exit/progression trigger once its completion condition is met.
6. Repeat across Level 1 → Level 2 → Level 3.
7. In Level 3: unlock the Vault, obtain the Treasure, reach the Rescue Boat → Victory.

## 8. Player Experience

The game should feel calm and exploratory most of the time, with light tension from hazards and a satisfying "aha" moment when clues/gems unlock new areas. Failure should feel low-stakes early (Level 1) and increasingly consequential (Level 3), but never punishing enough to force starting the whole game over — only the current level restarts, and only after all 3 lives are lost.

## 9. Controls

| Action | Primary Key | Alternate |
|---|---|---|
| Move Up | W | Up Arrow |
| Move Down | S | Down Arrow |
| Move Left | A | Left Arrow |
| Move Right | D | Right Arrow |
| Interact (pick up / open / talk) | Space | Enter |
| Pause / Menu | Escape | — |
| Confirm (menus) | Enter | Space |
| Navigate menu | Up/Down Arrow, W/S | — |
| Toggle debug overlay (dev builds only) | F1 | — |

Movement is 8-directional, smooth (velocity-based, not per-tile snapping).

## 10. Game Mechanics

- **Gems:** Collectible pickups scattered per level. Each level defines a `gemsRequired` count (may be less than total gems present) needed to satisfy the level's completion condition together with clues. Gems are tracked per-level and cumulatively for HUD display.
- **Clues:** Narrative/gating pickups. Each level has 1–3 clue objects. Clues are required to unlock specific gated objects (locked gate in Level 2, the Vault in Level 3).
- **Locked Areas:** Doors/gates/vault that check an unlock condition (specific clue IDs collected, and/or gem count) before allowing passage.
- **Lives:** Player starts each level with 3 lives. Contact with a hazard or falling into a fail-state (e.g., deep water, spike trap, quicksand) costs 1 life and respawns the player at the last checkpoint. Reaching 0 lives restarts the current level from its beginning (gems/clues reset for that level).
- **Checkpoints:** Invisible or lightly-marked trigger volumes that update the player's respawn point as they progress through a level.
- **Vault & Treasure (Level 3 only):** The Vault is a locked structure that opens only when all required clues (and, if configured, gems) have been collected. Opening it spawns/reveals the Treasure object, which the player must pick up.
- **Rescue Boat (Level 3 only):** The final progression trigger. Reachable only after the Treasure has been collected. Triggers the Victory sequence.

## 11. Level Descriptions & Objectives

| Level | Theme | Objective | Completion Condition |
|---|---|---|---|
| 1 — The Landing Shore | Beach + jungle edge, simple layout | Collect gems, find Clue A, reach the inland path | `gemsCollected >= gemsRequired(3)` AND `Clue_1_A collected` AND player enters `LevelExitTrigger` |
| 2 — The Overgrown Interior | Denser jungle/ruins, multiple paths, hazards | Collect more gems, find Clues B & C, avoid hazards, unlock the inner gate | `gemsCollected >= gemsRequired(5)` AND both clues collected AND gate unlocked AND player enters `LevelExitTrigger` |
| 3 — Vault Island | Vault, treasure, rescue boat | Collect final gems, unlock Vault with all clues, obtain Treasure, reach Rescue Boat | `gemsCollected >= gemsRequired(4)` AND Vault unlocked AND Treasure collected AND player enters `RescueBoatTrigger` |

Exact object counts and positions are defined per-level in `Design.md` §5 and implemented as level data assets described in `Architecture.md` §7.

## 12. Progression

Levels are unlocked strictly in order: Level 2 is unlocked only after Level 1's completion condition is met; Level 3 only after Level 2's. There is no level select for locked levels in v1 (see Future Improvements).

## 13. Lives

- 3 lives per level attempt.
- Life lost → respawn at last checkpoint within the same level, gems/clues already collected in that attempt are **retained**.
- Lives exhausted (0 remaining) → level restarts from the beginning; all gems/clues collected during that failed attempt are reset for that level.

## 14. Checkpoints

- Each level has at least 2 checkpoints (start-of-level counts as checkpoint 0).
- Checkpoints are simple axis-aligned trigger volumes; entering one updates `Player::currentCheckpoint`.
- Checkpoints do not persist across level restarts caused by exhausting all lives (restart always returns to checkpoint 0).

## 15. Gems

- Visually distinct colored gem primitives (see `Design.md` §5.4).
- Picking one up: increments level + total gem counters, removes it from the world, plays a small scale/fade collection effect, updates the HUD.
- A gem cannot be collected twice (removed from the active object list on pickup).

## 16. Clues

- Represented as a scroll/journal-page icon primitive.
- Picking one up adds its ID to `Player::collectedClues` and shows a short on-screen clue text (from a static string table) in the HUD clue panel.
- Clues persist for the remainder of the level attempt; lost only on full level restart (lives exhausted) or returning to main menu.

## 17. Vault & Treasure

- The Vault is a static object with an `unlocked` boolean, evaluated against a configurable `UnlockCondition` (list of required clue IDs + optional minimum gem count).
- On the frame the condition first becomes true, the Vault plays an "unlock" visual transition and becomes enterable/interactable.
- Interacting with the unlocked Vault (Space/Enter while overlapping, or auto-trigger by overlap — see `Rules.md`) spawns the Treasure pickup.
- The Treasure, once collected, sets `Player::hasTreasure = true`, which is required to activate the Rescue Boat trigger.

## 18. Rescue Boat

- Present only in Level 3.
- Its trigger is inert (visually present but non-functional) until `Player::hasTreasure == true`.
- Once active, player overlap + interact triggers the Victory sequence.

## 19. UI / HUD

**In-game HUD (always visible during Playing state):**
- Lives indicator (3 icons, filled/empty).
- Gem counter: `collected / required` for the current level.
- Clue indicator: number of clues collected this level, with a small "!" flash when a new clue is picked up.
- Current level name/number (top corner, fades after a few seconds).

**Menus:**
- Main Menu: Start Game, Continue (if a save exists), How to Play, Quit.
- Pause Menu (Escape during Playing): Resume, Restart Level, How to Play, Main Menu, Quit.
- Level Transition screen: brief "Level Complete" card with a Continue prompt.
- Game Over screen: shown when a level restarts due to lives exhausted is NOT itself a game-over — true Game Over only occurs per `Rules.md` (v1: there is no permanent game-over state other than returning to Main Menu; see Rules.md §Game Over Rules for the authoritative definition).
- Victory Screen: shown after the Rescue Boat trigger in Level 3, with a short congratulatory message and Return to Main Menu option.

## 20. Menus (Flow Summary)

`MainMenu → Playing(Level 1) → LevelTransition → Playing(Level 2) → LevelTransition → Playing(Level 3) → Victory → MainMenu`

`Playing → (Escape) → Paused → (Resume) → Playing`
`Paused → (Restart Level) → Playing (level reset to attempt start)`
`Paused → (Main Menu) → MainMenu`

## 21. Victory Condition

The player wins the game when, in Level 3, `Player::hasTreasure == true` AND the player overlaps and interacts with the Rescue Boat trigger. This transitions the game to the `Victory` state.

## 22. Game-Over Condition (Level-Restart Condition)

There is no permanent "Game Over" ending in v1 other than the player choosing Quit. Losing all 3 lives in a level triggers a **Level Restart**, not a terminal game over. This is intentional (see PRD §4 Non-Goals — the game should remain approachable). See `Rules.md` for the precise state-machine transition.

## 23. Functional Requirements

**MUST HAVE**
- FR-1: Render a 2D world using OpenGL 3.3 core profile via GLFW/GLAD.
- FR-2: Smooth 8-directional keyboard-controlled player movement.
- FR-3: AABB-based collision against static level geometry (trees, rocks, water edges, walls).
- FR-4: Gem pickup system with per-level and total counters.
- FR-5: Clue pickup system with gating logic for locked objects.
- FR-6: Lives system (3 lives), checkpoint respawn, and level-restart-on-zero-lives.
- FR-7: Three distinct, data-defined levels with increasing difficulty.
- FR-8: Vault unlock logic gated by clues (+ optional gem threshold).
- FR-9: Treasure pickup and Rescue Boat victory trigger in Level 3.
- FR-10: Main Menu, Pause Menu, Level Transition screen, Victory screen.
- FR-11: 2D camera that follows the player and clamps to level bounds.
- FR-12: Basic sprite/primitive-based animation (idle/walk cycles at minimum for the player).

**SHOULD HAVE**
- FR-13: Simple hazard objects that cost a life on contact (Level 2+).
- FR-14: Basic sound effects and background music via a minimal audio abstraction.
- FR-15: Local save file storing highest unlocked level.
- FR-16: On-screen key-rebinding-free "How to Play" screen.

**NICE TO HAVE**
- FR-17: Screen-shake / particle-style feedback on gem pickup and hazard hits (simple, GPU-cheap effects only).
- FR-18: A level-select screen for already-unlocked levels.
- FR-19: Simple minimap or compass hint toward the next objective.

## 24. Non-Functional Requirements

- NFR-1: Runs at a stable 60 FPS on a mid-range laptop iGPU at 1280×720.
- NFR-2: Codebase compiles cleanly with CMake on Windows (MSVC) and Linux (GCC/Clang) at minimum.
- NFR-3: No external game engine dependencies; only GLFW, GLAD, GLM, stb_image (textures), and optionally a small header-only audio library (e.g., miniaudio) if audio is implemented.
- NFR-4: All shaders are GLSL 330 core.
- NFR-5: Codebase organized so any single system can be explained by a team member in under 5 minutes.
- NFR-6: No crashes on malformed/missing level asset files — fail gracefully with a logged error and fallback to Main Menu.

## 25. Graphics Requirements

- Orthographic 2D projection with a defined world-unit coordinate system (`Design.md` §10).
- All game objects rendered via a batched 2D quad/primitive renderer using VAOs/VBOs/EBOs.
- All objects composed from OpenGL primitives (triangles/quads via `glDrawElements`) — vector-style look, optionally with flat-color or small texture fills.
- Hierarchical transforms (parent→child matrices) used for at least the player character (body → limbs) and one environment object (e.g., tree trunk → foliage), to explicitly demonstrate hierarchical transformations.
- Distinct shader programs (or shared shader + uniforms) for: flat-color primitives, textured sprites, and simple UI text.

## 26. Technical Constraints

- Language: C++17.
- Rendering: OpenGL 3.3 core profile only (no legacy fixed-function calls).
- Windowing/Input: GLFW 3.x.
- OpenGL loader: GLAD (generated for GL 3.3 core).
- Math: GLM.
- Build: CMake ≥ 3.16, dependencies fetched via `FetchContent` or vendored in `third_party/`.
- No engine frameworks (Unity, Unreal, Godot) and no SDL/SFML rendering backends.

## 27. Performance Expectations

- Target: 60 FPS, minimum acceptable: 30 FPS on the reference dev machine.
- Draw calls per frame budget: under 50 in the current architecture (batched renderer keeps this low even with dozens of on-screen objects).
- Level load time: under 1 second for any of the three levels.

## 28. Acceptance Criteria

A build satisfies the PRD when:
1. The game launches to a Main Menu and can start Level 1.
2. The player can move, collide with obstacles, collect gems, collect clues, lose a life to a hazard, respawn at a checkpoint, and — if lives reach 0 — restart the level.
3. Level 1 → Level 2 → Level 3 progression works in order and cannot be skipped.
4. In Level 3, the Vault remains locked until required clues are collected, then unlocks, spawns the Treasure, and the Rescue Boat becomes active only after the Treasure is collected.
5. Reaching the Rescue Boat with the Treasure shows the Victory screen.
6. Pause/Resume/Restart Level/Main Menu/Quit all function without crashing.
7. The game runs at a stable frame rate with no memory leaks detected across a full playthrough (checked via a debug build + sanitizer or Valgrind pass).

## 29. Future Improvements (Out of Scope for v1)

- Level select menu for unlocked levels.
- Additional levels / procedurally-varied gem placement.
- Controller support.
- More elaborate particle/lighting effects.
- Localization of UI text.
- Cloud save / multiple save slots.
