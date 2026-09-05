# Memory.md — The Lost Vault (Agent Persistent Memory)

> **Purpose:** This is the persistent context file for the Antigravity coding agent. Read this FIRST, every session, before touching code. Update the **CURRENT PROJECT STATE** section at the END of every implementation session. This file is the fastest way to reconstruct "where are we" without re-reading the whole repo.

---

## 1. Project Identity

- **Name:** The Lost Vault
- **Genre:** 2D Treasure Hunt / Adventure / Exploration
- **Platform:** Desktop PC (Windows/Linux)
- **Structure:** 3 levels, 3 lives per level attempt, single-player, no networking.
- **Authority order for conflicts:** `Rules.md` (gameplay behavior) > `PRD.md` (scope/requirements) > `Architecture.md` (implementation approach) > `Design.md` (visual/placement detail) > `Phases.md` (sequencing) > this file (status tracking).

## 2. Technology Stack

- Language: **C++17**
- Rendering: **OpenGL 3.3 core profile**
- Windowing/Input: **GLFW 3.x**
- GL loader: **GLAD** (vendored generated source, not fetched)
- Math: **GLM**
- Textures: **stb_image.h** (vendored)
- Fonts: **stb_truetype.h** (vendored, offline atlas-baking step)
- Audio (if implemented): **miniaudio.h** (vendored)
- Build: **CMake ≥ 3.16**, dependencies via `FetchContent` (GLFW, GLM) + vendored headers (`include/`)
- No game engine (no Unity/Unreal/Godot), no SDL/SFML rendering backend, no full ECS framework.

## 3. Current Architecture (Summary)

`Application → Game → GameStateManager → Scene(MainMenu/Level/Victory) → GameObjects (composition-based, fixed component slots) → Renderer2D → OpenGL 3.3`

Full detail: `Architecture.md`. Do not restructure this layering without explicit approval (see §13 below).

## 4. Directory Structure

See `Architecture.md` §3 for the authoritative full tree. Key top-level folders: `src/{core,graphics,gameplay,entities,levels,ui,input,audio,utils}`, `assets/{textures,fonts,audio,shaders,levels}`, `include/` (vendored third-party), `tests/`, `docs/` (this document set).

## 5. Important Classes

| Class | File | Role |
|---|---|---|
| `Application` | `core/Application` | Window/context/main loop owner |
| `Game` | `core/Game` | Owns global systems, `Update`/`Render` entry points |
| `GameStateManager` | `core/GameStateManager` | FSM: MainMenu/Playing/Paused/LevelTransition/Victory |
| `Scene` (+ subclasses) | `levels/Scene`, `MainMenuScene`, `LevelScene`, `VictoryScene` | Per-mode owner of active game objects/UI |
| `LevelData` / `LevelLoader` | `levels/LevelData`, `levels/LevelLoader` | Data-driven level definitions parsed from `.lvl` files |
| `GameObject` | `entities/GameObject` | Base entity: transform + fixed optional component slots |
| `Player` | `entities/Player` | Specialized entity: movement, lives, checkpoint, clues, treasure |
| `Renderer2D` | `graphics/Renderer2D` | All OpenGL draw calls funnel through here |
| `Camera2D` | `graphics/Camera2D` | Orthographic projection + follow + bounds clamp |
| `CollisionSystem` | `gameplay/CollisionSystem` | AABB resolution + trigger overlap detection |
| `GemSystem` / `ClueSystem` | `gameplay/GemSystem`, `ClueSystem` | Pickup logic + counters |
| `LivesSystem` / `CheckpointSystem` | `gameplay/LivesSystem`, `CheckpointSystem` | Life loss, respawn, restart |
| `VaultSystem` | `gameplay/VaultSystem` | Unlock-condition evaluation, Treasure spawn |
| `HazardSystem` | `gameplay/HazardSystem` | Hazard trigger → life loss |
| `UnlockCondition` | `gameplay/UnlockCondition.h` | Shared clue-set/gem-threshold gating type (used by gates AND the Vault) |
| `InputManager` | `input/InputManager` | GLFW key polling → `Action` enum abstraction |
| `HUD` / `PauseMenu` / `MainMenuUI` | `ui/*` | Player-facing UI |
| `ResourceManager` | owned by `Game` | Cached shader/texture/font loading |

## 6. Important Systems (Cross-Reference to Rules.md)

- Movement: Rules.md §1
- Collision: Rules.md §2
- Gems: Rules.md §3
- Clues: Rules.md §4
- Level unlock/completion: Rules.md §5–6
- Lives/death/respawn/checkpoints: Rules.md §7–10
- Hazards: Rules.md §11
- Vault/Treasure/Boat/Victory: Rules.md §12–15
- Game-over (= level restart, no terminal state): Rules.md §16
- Pause/Input/Camera/Interaction: Rules.md §17–20

## 7. Coding Conventions

- C++17, no exceptions used for control flow (only for unrecoverable init failure in `main.cpp`).
- Prefer `std::unique_ptr` for owned component slots on `GameObject`; raw non-owning pointers/references for borrowed access (e.g., `ResourceManager` returns non-owning pointers valid for process lifetime).
- No global mutable singletons except the `Log` macros; everything else is owned by `Game`/`Application` and passed down explicitly (dependency injection via constructor/reference, not service-locator globals).
- Header files use `#pragma once`.
- One class per header/source pair, matching filenames exactly (`ClassName.h` / `ClassName.cpp`).

## 8. Naming Conventions

- Classes: `PascalCase` (`GameObject`, `LevelScene`).
- Methods/functions: `PascalCase` for public API (`Update`, `DrawQuad`), matching the style already used in `Architecture.md` code samples.
- Member variables: `camelCase` (no Hungarian prefixes like `m_`).
- Constants/enums: `PascalCase` enum values inside a scoped `enum class` (e.g., `GameState::Playing`, `Action::MoveUp`).
- Files: match class name exactly; level asset files lowercase (`level1.lvl`).

## 9. Rendering Conventions

- All GL calls live inside `graphics/` classes — gameplay/entity code never calls raw `gl*` functions directly.
- World-space rendering and UI/screen-space rendering are always two separate `Renderer2D::BeginScene`/`EndScene` passes per frame, in that order (world first, UI last).
- Shared unit-quad VAO/VBO/EBO reused for all quads/sprites; per-object variation via uniform transform + color/texture only.

## 10. Coordinate System (Authoritative Summary — full detail in Design.md §25)

- World space: origin bottom-left, X right, Y **up**, units ≈ meters, PPU = 64.
- Screen/UI space: origin top-left, X right, Y **down**, raw pixels.
- Child world matrix = `parent.worldMatrix * child.localMatrix`.

## 11. Gameplay Rules Summary (Quick Reference — Rules.md is authoritative)

- 3 lives per level attempt; hazard hit → lose 1 life → respawn at last checkpoint (gems/clues retained).
- 0 lives → full level restart (gems/clues/vault/treasure state reset to that level's start; other levels' completion status unaffected).
- Gems/clues auto-collect on overlap (no key press); Vault and Rescue Boat require an explicit Interact press.
- Levels unlock strictly in order 1→2→3; no level-select for locked levels in v1.
- Victory = Level 3 Rescue Boat interaction while `hasTreasure == true`.

## 12. Level Summary

| Level | Bounds | Gems (required/total) | Clues | Hazards | Special objects |
|---|---|---|---|---|---|
| 1 — Landing Shore | 40×24 | 3/4 | Clue_1_A | none | — |
| 2 — Overgrown Interior | 56×32 | 5/6 | Clue_2_B, Clue_2_C | quicksand, spike trap | locked gate |
| 3 — Vault Island | 48×28 | 4/5 | (uses all 3 prior clues) | quicksand, spike trap (reused) | Vault, Treasure, Rescue Boat |

## 13. Decisions That Must NOT Be Changed Without Explicit Approval

> **DO NOT CHANGE WITHOUT EXPLICIT APPROVAL**

1. No external game engine and no SDL/SFML rendering backend — hand-rolled OpenGL only.
2. No full ECS framework — `GameObject` keeps its fixed, small set of optional component slots.
3. World coordinate system is Y-up with origin bottom-left; UI coordinate system is Y-down with origin top-left. Never unify these into one space.
4. `UnlockCondition` stays limited to "set of required clue IDs + optional minimum gem count" — do not generalize into a full scripting/quest system.
5. There is no terminal "Game Over" state — losing all lives always results in a Level Restart, never a return-to-menu or permanent failure state (Rules.md §16).
6. Gems/clues are collected on overlap alone (no Interact press); Vault/Rescue Boat require an explicit Interact press. This asymmetry is intentional (Rules.md §20.1) — do not make it consistent "for simplicity" without approval.
7. Fixed-timestep gameplay update (60Hz) decoupled from render rate — do not switch to pure variable-timestep movement.
8. Level content is authored data (`.lvl` files), never procedurally generated.

## 14. Dependencies (Pinned Expectations)

- GLFW: latest 3.x stable at project start, fetched via CMake `FetchContent`.
- GLM: latest stable, header-only, fetched via `FetchContent`.
- GLAD: generated once for GL 3.3 core + compatibility with the GLFW loader pattern, vendored (not regenerated automatically by CI).
- stb_image.h / stb_truetype.h: vendored single-header, pinned copy in `include/`.
- miniaudio.h (only if Phase 13 is undertaken): vendored single-header.

## 15. Build Instructions

```bash
# From repository root
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

On Windows with MSVC, open the generated `.sln` or use `cmake --build . --config Debug` from a Developer Command Prompt. On Linux, ensure `libglfw3-dev`, a working OpenGL driver, and standard build tools (`build-essential`, `cmake`) are installed before configuring.

## 16. Run Instructions

Run the produced executable **from the repository root** (or ensure the working directory contains `assets/` alongside the executable, e.g., via a post-build copy step in `CMakeLists.txt`), since asset paths are resolved relative to `assets/`. Example:

```bash
./build/TheLostVault
```

## 17. Testing Instructions

```bash
cd build
ctest --output-on-failure
```

Or run the test binary directly: `./build/TheLostVaultTests`. See `Architecture.md` §22 and `Phases.md` Phase 15 for what must be covered.

## 18. Common Troubleshooting

| Symptom | Likely Cause | Fix |
|---|---|---|
| Black window, no draws | Shader compile/link failure | Check `Log` output for GLSL compile errors printed by `Shader` |
| Window fails to open | Requested GL 3.3 core not supported | Update GPU drivers; verify `glfwWindowHint` values match `Architecture.md` §6 |
| Textures appear magenta/missing | `stb_image` failed to load path | Verify working directory contains `assets/`; check `ResourceManager` fallback log |
| Player falls through obstacles | Collider not added to static list, or trigger flag set incorrectly | Verify `.lvl` parsing marks the object solid, check `ColliderComponent::isTrigger` |
| Level never completes | Completion condition checking wrong `gemsRequired`/clue set | Compare against `PRD.md` §11 and `Design.md` §5 for that level |
| Camera shows outside level | Bounds clamp not applied or `LevelData::bounds` wrong | Check `Camera2D` clamp logic against `Architecture.md` §13 |

---

## 19. CURRENT PROJECT STATE

> **Update this section at the end of every implementation session.**

- **Current phase:** Phase 0 — Project Setup (Completed)
- **Last completed task:** Phase 0 — Project Setup: Initialized repository structure (`src/`, `assets/`, `include/`, `tests/`), `CMakeLists.txt` with GLFW 3.4 and GLM 1.0.1 via `FetchContent`, vendored GLAD OpenGL 3.3 core profile loader (`include/glad/glad.h`, `include/KHR/khrplatform.h`, `src/glad.c`), `stb` headers (`include/stb_image.h`, `include/stb_truetype.h`), `src/main.cpp` (1280×720 resizable window, OpenGL 3.3 core context, clear color loop `#8FD3E8`, Escape key exit), and `README.md`.
- **Current task:** Phase 0 — Project Setup (Complete).
- **Next recommended task:** Phase 1 — OpenGL Foundation: Implement `core/Application`, `core/Game`, `core/Clock`, `core/Log`, and move the main loop into `Application::Run()` using the fixed-timestep loop pattern per `Architecture.md` §8 and `Phases.md` Phase 1.
- **Build status:** Clean build (`cmake --build build` produces `build/TheLostVault.exe` with zero errors).
- **Test status:** Verified executable launches, clears screen, and exits cleanly.
- **Known issues:** None.
- **Known limitations:** See `PRD.md` §29 Future Improvements.
- **Recent architectural decisions:** Standardized GLAD header API bindings with static `extern` declarations for target `TheLostVaultCore`.

