# Phases.md — The Lost Vault (Implementation Roadmap)

> **Authority note:** This document sequences *when* things get built, referencing class/module names defined in `Architecture.md` and behavior defined in `Rules.md`. If a phase seems to require behavior not yet specified, consult `Rules.md`/`PRD.md` before inventing new behavior. Every phase must end with a project that **builds and runs**. Each phase is intentionally small — one phase is roughly one Antigravity work session / one task for the coding agent.

---

## Phase 0 — Project Setup

- **Goal:** A buildable, empty window.
- **Prerequisites:** None.
- **Tasks:** Create directory structure (`Architecture.md` §3). Write `CMakeLists.txt` with `FetchContent` for GLFW/GLM, vendor GLAD + stb_image under `include/`. Write `main.cpp` that creates a GLFW window with an OpenGL 3.3 core context and clears the screen to a solid color each frame.
- **Files affected:** `CMakeLists.txt`, `src/main.cpp`, `include/glad/*`, `include/KHR/*`.
- **Expected output:** Running the executable opens a resizable window (1280×720) that clears to a solid background color and closes cleanly on window-close or Escape.
- **Acceptance criteria:** Builds with zero warnings on at least one of MSVC/GCC/Clang. Window opens and closes without crash.
- **Testing requirements:** Manual: launch, resize, close.
- **Potential risks:** GLFW/GLAD version mismatch; OpenGL context version not supported by dev GPU (mitigate: request 3.3 core explicitly, fail with clear log message if creation fails).
- **Definition of done:** `git`-committable buildable skeleton; README has build instructions.

## Phase 1 — OpenGL Foundation

- **Goal:** Introduce `Application`, `Game`, `Clock`, `Log`, and a fixed-timestep loop.
- **Prerequisites:** Phase 0.
- **Tasks:** Implement `core/Application`, `core/Game` (empty `Update`/`Render` stubs), `core/Clock`, `core/Log`. Move the loop from `main.cpp` into `Application::Run()` using the fixed-timestep pseudocode in `Architecture.md` §8.
- **Files affected:** `src/core/*`, `src/main.cpp`.
- **Expected output:** Same visual result as Phase 0, now driven by the real loop/class structure.
- **Acceptance criteria:** Loop runs at a stable rate (log FPS to console in debug); `Log` macros function.
- **Testing requirements:** Manual FPS check; verify fixed-update ticks are consistent when window is dragged/resized (no runaway "spiral of death" — clamp `frameTime` per iteration).
- **Potential risks:** Spiral of death if `frameTime` isn't clamped after a breakpoint/stall. Mitigate with a max-frametime clamp.
- **Definition of done:** No regression from Phase 0; new classes documented with brief header comments.

## Phase 2 — Rendering Primitives

- **Goal:** `Renderer2D`, `Shader`, shared quad VAO/VBO/EBO, `primitive.vert/frag`, ability to draw a colored rectangle and a triangle-fan circle.
- **Prerequisites:** Phase 1.
- **Tasks:** Implement `graphics/Shader` (compile/link + uniform helpers), `graphics/Renderer2D` (`DrawQuad`, batching per `Architecture.md` §5–6), `graphics/PrimitiveFactory` (circle via triangle fan, per `Design.md`/academic requirement on circle rasterization concepts), `assets/shaders/primitive.vert/.frag`.
- **Files affected:** `src/graphics/*`, `assets/shaders/primitive.*`.
- **Expected output:** The screen shows a handful of static colored rectangles and circles at known world positions.
- **Acceptance criteria:** At least one quad and one circle render correctly with correct color and position; resizing the window preserves aspect ratio without stretching.
- **Testing requirements:** Manual visual check against expected coordinates; unit test for `Shader` compile error handling (feed a deliberately broken shader string, assert it logs and doesn't crash).
- **Potential risks:** VAO/VBO state leakage between draws — mitigate by always explicitly binding before each draw and unbinding after (or centralizing all binds inside `Renderer2D`).
- **Definition of done:** Renderer API is stable enough that later phases only call `DrawQuad`/circle helpers, never touch raw GL.

## Phase 3 — Player

- **Goal:** A controllable player character on screen.
- **Prerequisites:** Phase 2, `input/InputManager` stub.
- **Tasks:** Implement `input/InputManager` (Action enum + GLFW polling per `Architecture.md` §9). Implement `entities/GameObject`, `entities/TransformComponent`, `entities/Player` with hierarchical rendering (body root + limb child transforms, per academic requirement on hierarchical transformations). Wire WASD/arrow movement (no collision yet).
- **Files affected:** `src/input/*`, `src/entities/GameObject.*`, `src/entities/TransformComponent.*`, `src/entities/Player.*`.
- **Expected output:** A stick-figure/sailor shape built from primitives moves smoothly around the screen with WASD/arrows.
- **Acceptance criteria:** Movement matches `Rules.md` §1 (8-directional, normalized diagonal speed, facing retained when idle).
- **Testing requirements:** Manual movement test in all 8 directions; verify diagonal speed is not faster than cardinal (log/measure distance over 1 second).
- **Potential risks:** Naive diagonal movement summing to `sqrt(2)`x speed — mitigate by normalizing the input vector before scaling by `moveSpeed`.
- **Definition of done:** Player entity + input fully decoupled from any specific level (works against an empty world).

## Phase 4 — Camera and World

- **Goal:** `Camera2D`, level bounds, and a minimal static Level 1 layout (visual only, no gameplay logic yet).
- **Prerequisites:** Phase 3.
- **Tasks:** Implement `graphics/Camera2D` (ortho projection + follow + bounds clamp, `Architecture.md` §13). Implement `levels/LevelData` (struct only) and a hardcoded/minimal `assets/levels/level1.lvl` with just bounds + a few decorative static objects (trees, rocks, sand/water tiles) rendered via sprites/primitives.
- **Files affected:** `src/graphics/Camera2D.*`, `src/levels/LevelData.*`, `assets/levels/level1.lvl`.
- **Expected output:** Player moves within a larger-than-screen world; camera follows and clamps at edges.
- **Acceptance criteria:** Camera never shows area outside `LevelData::bounds` (Rules.md R19.2); player stays centered except near edges.
- **Testing requirements:** Manual — walk to all four edges of the level and confirm camera clamp behavior.
- **Potential risks:** Off-by-one in ortho projection producing sub-pixel jitter — mitigate by keeping world-to-pixel scale as a clean integer (`PixelsPerUnit`).
- **Definition of done:** World feels like a real 2D game space, not just objects floating on screen.

## Phase 5 — Collision

- **Goal:** Solid obstacle collision (AABB) between player and static geometry.
- **Prerequisites:** Phase 4.
- **Tasks:** Implement `utils/AABB`, `entities/ColliderComponent`, `gameplay/CollisionSystem` (axis-separated resolution per `Architecture.md` §12 / `Rules.md` §2). Mark some Level 1 objects (trees, rocks) as solid.
- **Files affected:** `src/utils/AABB.h`, `src/entities/ColliderComponent.*`, `src/gameplay/CollisionSystem.*`.
- **Expected output:** Player can no longer walk through trees/rocks; sliding along a wall/obstacle edge feels smooth (no snagging on corners).
- **Acceptance criteria:** Rules.md §2 satisfied (R2.1–R2.3); unit test for `AABB` intersection passes.
- **Testing requirements:** Unit test (`tests/test_collision.cpp`); manual corner-sliding test.
- **Potential risks:** Simultaneous-axis resolution causing corner snagging — mitigate with the documented axis-separated approach (resolve X then Y).
- **Definition of done:** Collision API generic enough to reuse for triggers in Phase 6+ (triggers just skip position correction).

## Phase 6 — Gems

- **Goal:** Working gem pickup system end-to-end for Level 1.
- **Prerequisites:** Phase 5.
- **Tasks:** Implement `gameplay/GemSystem`, gem `GameObject` prefab (circle/faceted-polygon shape per `Design.md`), trigger-only overlap detection (extend `CollisionSystem` to report trigger overlaps without resolving position). Add gem spawns to `level1.lvl`. Track `Player::collectedGemsThisLevel`.
- **Files affected:** `src/gameplay/GemSystem.*`, `src/levels/LevelLoader.*` (gem parsing), `assets/levels/level1.lvl`.
- **Expected output:** Walking over a gem removes it and increments a (temporary, console-logged or placeholder on-screen) counter.
- **Acceptance criteria:** Rules.md §3 (R3.1–R3.4) satisfied; a gem cannot be collected twice (unit-testable).
- **Testing requirements:** Unit test for "collect same gem ID twice is a no-op"; manual pickup test for all Level 1 gems.
- **Potential risks:** Double-counting due to overlap firing multiple frames before removal — mitigate by removing the `GameObject` from the active list in the same frame as the pickup event, before the next collision pass.
- **Definition of done:** GemSystem API ready for HUD wiring in Phase 12.

## Phase 7 — Clues

- **Goal:** Working clue pickup + gating logic (locked gate demo in Level 2 groundwork).
- **Prerequisites:** Phase 6.
- **Tasks:** Implement `gameplay/ClueSystem`, clue `GameObject` prefab (scroll icon), `utils/StringTable` for clue text, and a generic `UnlockCondition` struct (list of required clue IDs + optional min gem count) usable by both a "locked gate" object and (later) the Vault. Add Clue A to `level1.lvl`.
- **Files affected:** `src/gameplay/ClueSystem.*`, `src/utils/StringTable.*`, `src/gameplay/UnlockCondition.h`.
- **Expected output:** Picking up a clue logs/shows its text; a test locked-gate object (temporary, can be simple) becomes passable once the right clue is held.
- **Acceptance criteria:** Rules.md §4 (R4.1–R4.4) satisfied.
- **Testing requirements:** Unit test for `UnlockCondition::IsSatisfied()` against various clue-set/gem-count combinations.
- **Potential risks:** Over-generalizing `UnlockCondition` — keep it to "set of clue IDs + optional min gem count," nothing more, per Non-Goals (avoid over-engineering).
- **Definition of done:** Same `UnlockCondition` type will be reused unmodified by `VaultSystem` in Phase 11.

## Phase 8 — Lives / Checkpoints

- **Goal:** Full life-loss, respawn, and level-restart loop.
- **Prerequisites:** Phase 7.
- **Tasks:** Implement `gameplay/LivesSystem`, `gameplay/CheckpointSystem`, checkpoint `GameObject` prefab, a temporary test hazard trigger in `level1.lvl`, `LevelScene::RestartLevel()`. Implement the brief death-feedback window (Rules.md §8) and hazard invulnerability window (Rules.md §11.2).
- **Files affected:** `src/gameplay/LivesSystem.*`, `src/gameplay/CheckpointSystem.*`, `src/levels/LevelScene.*`.
- **Expected output:** Touching the test hazard costs a life and respawns the player at the nearest prior checkpoint; losing all 3 lives restarts the level and resets counters per Rules.md §16.2.
- **Acceptance criteria:** Rules.md §7–§12, §16 satisfied for the single-level case.
- **Testing requirements:** Unit test for `LivesSystem` state transitions (lose life with lives>0 → respawn event; lose life at lives==1 → restart event). Manual full-loop test (die 3 times, confirm restart resets gems/clues).
- **Potential risks:** Off-by-one in "3rd life lost" detection — write the unit test first (TDD-lite) to avoid ambiguity.
- **Definition of done:** This is the first phase where the *entire* Rules.md single-level gameplay loop is real and testable.

## Phase 9 — Level 1 (Complete)

- **Goal:** Level 1 fully authored and completable start-to-finish.
- **Prerequisites:** Phase 8.
- **Tasks:** Finalize `level1.lvl` per `Design.md` §5 (all gems, Clue A, checkpoints, obstacles, `LevelExitTrigger`). Implement level completion condition check (Rules.md §6) and wire `GameStateManager` transition to `LevelTransition` on completion.
- **Files affected:** `assets/levels/level1.lvl`, `src/levels/LevelScene.*`, `src/core/GameStateManager.*`.
- **Expected output:** A player can play Level 1 from start to a "Level Complete" trigger, fully following Rules.md.
- **Acceptance criteria:** PRD.md §28 acceptance items 1–2 (partially, Level 1 scope) pass.
- **Testing requirements:** Full manual playthrough of Level 1 including at least one intentional death.
- **Potential risks:** Level authoring producing an uncompletable layout (gem placed unreachably) — mitigate with a manual playtest checklist before marking done.
- **Definition of done:** Level 1 is playable in isolation and is the reference implementation pattern for Levels 2–3.

## Phase 10 — Level 2

- **Goal:** Second level: more complexity, real hazards, a locked gate.
- **Prerequisites:** Phase 9.
- **Tasks:** Author `level2.lvl` (Design.md §5 spec): more obstacles, multiple paths, Clues B & C, a locked gate using `UnlockCondition`, at least one real hazard type (e.g., quicksand or spike trap) using `HazardSystem` (new: `gameplay/HazardSystem` generalized from the Phase 8 test hazard). Wire level-to-level transition (Level 1 complete → load Level 2).
- **Files affected:** `assets/levels/level2.lvl`, `src/gameplay/HazardSystem.*`, `src/levels/LevelLoader.*` (hazard/gate parsing).
- **Expected output:** Completing Level 1 loads Level 2; Level 2 is playable and completable per its own completion condition.
- **Acceptance criteria:** Rules.md §5 (unlock ordering) verified — Level 2 is unreachable directly from Main Menu before Level 1 is completed.
- **Testing requirements:** Manual full playthrough; verify locked gate cannot be passed before both clues collected (Rules.md §12.3-equivalent for the gate).
- **Potential risks:** Hazard difficulty miscalibrated (too easy/hard) — treat as a design tuning pass, not a blocker for phase completion.
- **Definition of done:** Two-level progression works end-to-end from Main Menu.

## Phase 11 — Level 3 (Vault, Treasure, Rescue Boat)

- **Goal:** Final level with Vault/Treasure/Rescue Boat and Victory.
- **Prerequisites:** Phase 10.
- **Tasks:** Author `level3.lvl`. Implement `gameplay/VaultSystem` (reusing `UnlockCondition` from Phase 7), Treasure spawn-on-unlock-and-interact (Rules.md §12.5), Rescue Boat inert/active states (Rules.md §14), Victory transition (Rules.md §15) and `levels/VictoryScene`.
- **Files affected:** `src/gameplay/VaultSystem.*`, `src/levels/VictoryScene.*`, `assets/levels/level3.lvl`.
- **Expected output:** Full game is completable start-to-finish, ending on a Victory screen.
- **Acceptance criteria:** PRD.md §28 acceptance items 1–5 all pass.
- **Testing requirements:** Unit test for `VaultSystem` unlock evaluation with the actual Level 3 clue set; full manual playthrough of all 3 levels back-to-back.
- **Potential risks:** Player reaching the Rescue Boat area before the Vault sequence is complete and getting confused — mitigate with the flavor-prompt fallback (Rules.md R14.4) and level layout gating.
- **Definition of done:** The core game is feature-complete per PRD MUST HAVEs FR-1 through FR-11.

## Phase 12 — UI

- **Goal:** Real Main Menu, Pause Menu, HUD, Level Transition card, Victory screen (replacing any placeholder/console-logged UI from earlier phases).
- **Prerequisites:** Phase 11 (needs all gameplay systems to have real data to display), but UI *code* (TextRenderer, UIButton/UILabel) can be started as early as Phase 9 in parallel if the team splits work — sequenced here for a single-agent incremental build.
- **Tasks:** Bake a bitmap font atlas (offline `stb_truetype` step, documented in README), implement `ui/TextRenderer`, `ui/UIButton`, `ui/UILabel`, `ui/HUD`, `ui/PauseMenu`, `ui/MainMenuUI`, screen-space projection pass in `Renderer2D` (Architecture.md §14).
- **Files affected:** `src/ui/*`, `assets/fonts/*`, `assets/shaders/text.*`.
- **Expected output:** Lives/gems/clue counters visible during play; functional Main Menu (Start/How to Play/Quit) and Pause Menu (Resume/Restart/How to Play/Main Menu/Quit); Level Transition and Victory screens replace placeholders.
- **Acceptance criteria:** PRD.md §19–20 satisfied; Rules.md §17 (pause) fully functional through real menu UI.
- **Testing requirements:** Manual click/keyboard-nav test of every menu path listed in PRD.md §20.
- **Potential risks:** Screen-space vs. world-space projection mixing bugs (UI scrolling with camera by mistake) — mitigate by keeping two entirely separate `Renderer2D::BeginScene` calls per frame (world pass, then UI pass with identity/ortho-screen camera).
- **Definition of done:** No system in the game relies on console-logged debug text for player-facing information anymore.

## Phase 13 — Audio (SHOULD HAVE)

- **Goal:** Basic SFX + music via `AudioManager`.
- **Prerequisites:** Phase 12.
- **Tasks:** Vendor a minimal audio backend (e.g., `miniaudio.h`), implement `audio/AudioManager` per the interface in `Architecture.md` §18, wire SFX to gem pickup, clue pickup, hazard hit, vault unlock, victory; background music per menu/level.
- **Files affected:** `src/audio/AudioManager.*`, `assets/audio/*`.
- **Expected output:** Audible feedback for key gameplay events; background music loops appropriately per scene.
- **Acceptance criteria:** PRD.md FR-14; muting works; no audio glitches (popping/looping seams) on the reference build.
- **Testing requirements:** Manual audio pass through a full playthrough.
- **Potential risks:** Platform-specific audio device init failures — mitigate by having `AudioManager` fail soft (log + continue silently) rather than crash if no audio device is available (e.g., CI/headless environment).
- **Definition of done:** Game is fully playable with sound; also fully playable (silently) with audio device unavailable.

## Phase 14 — Polish

- **Goal:** Visual/feel polish pass without new systems.
- **Prerequisites:** Phase 13 (or Phase 12 if audio is deferred).
- **Tasks:** Add gem-collection scale/fade effect, hazard-hit flash, vault-unlock visual transition, level-transition card animation, simple idle/walk animation frames for the player (Architecture.md/Design.md animation spec), tune camera follow smoothing.
- **Files affected:** `src/entities/AnimatorComponent.*`, various `Design.md`-specified visual tweaks across `graphics/`.
- **Expected output:** The game feels noticeably more alive without any new gameplay rules being introduced (Rules.md is unchanged by this phase).
- **Acceptance criteria:** No regression in any earlier phase's acceptance criteria; PRD.md FR-12, FR-17 (if attempted) satisfied.
- **Testing requirements:** Full manual playthrough focused on "feel," not correctness (correctness is already locked in by earlier phases' tests).
- **Potential risks:** Polish scope creep — cap this phase's time box; anything not finished rolls to Future Improvements, not into new Rules.md behavior.
- **Definition of done:** Team agrees the game "feels done," not just "functions."

## Phase 15 — Testing

- **Goal:** Consolidate and complete the automated test suite; run a full manual QA pass.
- **Prerequisites:** Phase 14.
- **Tasks:** Fill out `tests/test_collision.cpp`, `tests/test_levelloader.cpp`, `tests/test_gem_clue_rules.cpp` to cover all Rules.md numbered rules that are unit-testable (movement normalization, collision resolution, gem/clue no-double-collect, UnlockCondition evaluation, LivesSystem transitions, VaultSystem evaluation). Run a debug build under a memory sanitizer (ASan) or Valgrind across a full playthrough.
- **Files affected:** `tests/*`.
- **Expected output:** Green test suite; no memory errors/leaks reported across a full playthrough.
- **Acceptance criteria:** PRD.md §28 item 7 satisfied.
- **Testing requirements:** This phase *is* the testing requirement.
- **Potential risks:** Sanitizer build performance too slow for a full real-time playthrough — mitigate by scripting/automating a "bot" input sequence for the sanitizer pass if manual real-time play is impractical under ASan.
- **Definition of done:** CI (if set up) or a documented manual test script passes cleanly.

## Phase 16 — Final Build / Documentation

- **Goal:** Ship-ready build and finished documentation.
- **Prerequisites:** Phase 15.
- **Tasks:** Produce optimized (Release) CMake build; finalize `README.md` (build/run/controls); finalize `Memory.md` "CURRENT PROJECT STATE" to reflect completion; write a short "Known Limitations" section (from PRD.md Future Improvements).
- **Files affected:** `README.md`, `docs/Memory.md`, `CMakeLists.txt` (Release flags).
- **Expected output:** A distributable Release build and complete documentation set.
- **Acceptance criteria:** All PRD.md §28 acceptance criteria pass on a clean checkout + fresh build.
- **Testing requirements:** Fresh-machine (or fresh clone + fresh build directory) smoke test.
- **Potential risks:** Undocumented local machine state masking a build issue — mitigate by testing from a completely clean clone.
- **Definition of done:** Project is handoff-ready for academic submission/evaluation.

---

### Phase Sequencing Notes for the Antigravity Agent

- Phases 0–8 must be done strictly in order (each depends on the previous).
- Phases 9, 10, 11 must be done in order (level content depends on all core systems from Phase 8, and Level 3 depends on the `UnlockCondition` type introduced in Phase 7).
- Phase 12 (UI) can technically begin once Phase 9 exists, but is sequenced after Phase 11 for a single-agent incremental build to avoid rework as gameplay data shapes stabilize.
- Phases 13–16 are sequential polish/finalization and can be reordered slightly (e.g., Audio and Polish swapped) without breaking dependencies, but Testing (15) must always precede Final Build (16).
- **Never skip ahead**: the agent should always identify the current phase from `Memory.md`'s "CURRENT PROJECT STATE" before starting new work, per the Master Prompt.
