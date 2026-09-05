# Rules.md — The Lost Vault (Authoritative Gameplay Rules)

> **This document is the single source of truth for gameplay behavior.** If `PRD.md`, `Architecture.md`, `Design.md`, or any other document conflicts with this file on how the game behaves, **Rules.md wins**. Architecture/design documents describe *how it's built*; this document describes *what must happen*.

---

## 1. Movement Rules

- R1.1 Player movement is 8-directional and continuous (not tile-snapped); pressing two perpendicular movement keys moves diagonally. Diagonal speed is normalized (magnitude equal to cardinal speed), not `sqrt(2)` faster.
- R1.2 Movement input is read every fixed update tick (60Hz) via `InputManager`.
- R1.3 The player has a single configurable `moveSpeed` (world units/second), constant across all 3 levels unless a specific hazard/effect explicitly modifies it (none does in v1).
- R1.4 Facing direction updates to match the last non-zero movement input direction; if no movement key is held, facing direction is retained (does not reset to a default).
- R1.5 The player cannot move outside `LevelData::bounds`; attempting to do so simply stops the player at the boundary (bounds act as an implicit solid wall on all four sides).

## 2. Collision Rules

- R2.1 Static obstacles (trees, rocks, walls, boulders, water edges marked solid) block player movement; the player's AABB may not overlap a solid obstacle's AABB after resolution.
- R2.2 Collision resolution is axis-separated: resolve horizontal penetration first, then vertical, each frame, to prevent the player sticking on inside corners.
- R2.3 Trigger volumes (gems, clues, checkpoints, hazards, vault, treasure, rescue boat, level-exit) are **never** solid; the player passes through them freely while overlap events fire.
- R2.4 If a static obstacle is misconfigured such that the player spawns already overlapping it, the player is not forcibly ejected mid-frame in a way that teleports them outside the visible screen; instead, resolution is capped to a maximum correction per frame (implementation detail in `Architecture.md`, but the behavioral guarantee — no instant unbounded teleport — is a rule).

## 3. Gem Collection Rules

- R3.1 A gem is collected the instant the player's AABB overlaps the gem's trigger AABB — no explicit interact key press is required for gems.
- R3.2 A collected gem is immediately removed from the level's active object list. **A gem cannot be collected twice** — if the underlying pickup is somehow queried again (e.g., due to a bug), the system must treat it as already-collected and a no-op, never double-count.
- R3.3 Gem counts are tracked per-level-attempt (`Player::collectedGemsThisLevel`) and reset to 0 whenever the level restarts due to lives reaching zero (see §12). They are **not** reset on checkpoint respawn (a life lost to a hazard does not remove already-collected gems).
- R3.4 A level's completion condition may require `collectedGemsThisLevel >= gemsRequired`, where `gemsRequired <= totalGemsInLevel` (not all gems are mandatory; see `Design.md`/`PRD.md` per-level counts). Collecting more than `gemsRequired` has no additional gameplay effect beyond the HUD counter.

## 4. Clue Rules

- R4.1 A clue is collected the instant the player's AABB overlaps the clue's trigger AABB (same rule as gems — no explicit interact key needed), and its ID is added to `Player::collectedClues`.
- R4.2 A clue cannot be collected twice; once its ID is in `collectedClues`, the world object is removed and re-overlap (if it somehow still existed) is a no-op.
- R4.3 `collectedClues` resets on full level restart (lives exhausted), same as gems (§3.3), and is **not** reset on checkpoint respawn.
- R4.4 Clue text is shown once immediately on pickup (HUD flash + clue panel entry) and remains viewable in a "Clues" HUD sub-panel for the remainder of the level attempt.

## 5. Level Unlocking Rules

- R5.1 Levels unlock strictly in order: Level 2 is locked until Level 1's completion condition (§7) has been met at least once; Level 3 is locked until Level 2's has been met. This is enforced at the `MainMenu`/`LevelTransition` layer, not by in-level geometry.
- R5.2 Once a level has been completed in the current game session (or, if SaveSystem is implemented, in any prior session), it remains unlocked; replaying an earlier unlocked level does not lock later levels.
- R5.3 There is no way to enter Level 2 or Level 3 directly from the Main Menu unless the prerequisite level(s) have been completed (v1 has no level-select for locked levels — see `PRD.md` Non-Goals).

## 6. Level Completion Rules

- R6.1 A level's completion condition (defined per-level in `PRD.md` §11 and `Design.md`) is evaluated continuously (every fixed update) while `GameStateManager` is in `Playing` state for that level.
- R6.2 The moment all required sub-conditions (gems, clues, and — for Level 3 — Vault/Treasure/Boat) are simultaneously true, the level is marked complete and the game transitions to `LevelTransition`.
- R6.3 For Levels 1 and 2, completion additionally requires the player to physically enter the level's `LevelExitTrigger` volume after the gem/clue conditions are already satisfied (the exit does not "auto-trigger" completion the instant the last gem/clue is collected elsewhere on the map) — this gives the player a clear, deliberate "I'm done, moving on" action.
- R6.4 For Level 3, completion is equivalent to Victory (§10) — there is no separate `LevelExitTrigger` distinct from the Rescue Boat.
- R6.5 The player **cannot** leave a level (trigger its exit / advance) before its completion condition is met — the exit trigger is inert (visually present, but does not fire the transition) until all required sub-conditions are true.

## 7. Life Rules

- R7.1 The player begins each level attempt with exactly 3 lives (`Player::lives = 3` on `OnEnter` of a fresh `LevelScene`, and again on any full level restart per §12).
- R7.2 A life is lost when the player triggers a fatal condition: overlapping a designated hazard trigger (spikes, deep water, patrolling hazard, quicksand fully-submerged state — see `Design.md` for the exact hazard catalog per level) or failing a required timed/skill challenge if one exists in a given level.
- R7.3 Losing a life never removes already-collected gems or clues for the current attempt (see §3.3, §4.3) — only a full level restart does that.
- R7.4 Losing a life always triggers a respawn (§9), never an instant game-over, regardless of which life number was just lost (including the 3rd).

## 8. Death Rules

- R8.1 "Death" in this game is represented purely as "lose a life and respawn" — there is no separate death animation state blocking gameplay beyond a brief (≤1 second) visual/audio feedback cue (see `Design.md` §Death Effects) before control returns to the player at the checkpoint.
- R8.2 Input during the death feedback window is ignored (movement/interact) to prevent the player acting mid-transition; Pause (Escape) remains available.
- R8.3 Death has no permanent penalty beyond the life count and, if lives reach 0, the level restart consequences in §12.

## 9. Respawn Rules

- R9.1 On losing a life with `lives > 0` remaining after the loss, the player is teleported to `CheckpointSystem::GetRespawnPosition()` (the last checkpoint entered, or the level start if none has been reached yet), velocity reset to zero, facing direction reset to a level-defined default (typically "facing inland"/up).
- R9.2 Respawn does not reset gem/clue counters (§3.3, §4.3) and does not reset hazard states that are one-shot (e.g., a spike trap already triggered does not "un-trigger"); hazards that are persistent environmental features (e.g., a permanent quicksand patch) remain active and can cost another life if re-entered.
- R9.3 Respawn position is never inside a solid obstacle or hazard trigger (checkpoints are authored/validated to guarantee this — an authoring rule, not a runtime rule, but a hard requirement).

## 10. Checkpoint Rules

- R10.1 Entering a checkpoint trigger updates `Player::currentCheckpoint` to that checkpoint's ID **only if** its order index is higher than the current one (checkpoints cannot move the respawn point backward if the player re-enters an earlier one).
- R10.2 Checkpoint 0 (implicit) is always the level's start position and is the respawn point if no explicit checkpoint has been reached yet.
- R10.3 Checkpoints are per-level; they do not carry over between levels or survive a full level restart (§12 always resets to checkpoint 0).

## 11. Hazard Rules

- R11.1 A hazard trigger causes exactly one life-loss event per "entry" — i.e., while continuously overlapping the same hazard after a life-loss-triggered respawn/invulnerability window, it does not repeatedly subtract lives every tick.
- R11.2 After a hazard-caused life loss, the player is granted a brief invulnerability window (recommended 1 second) during which no further life loss can occur, to prevent instantly losing multiple lives at the respawn point if it is near another hazard.
- R11.3 Hazards introduced starting in Level 2 (per `PRD.md` §11); Level 1 contains no fatal hazards (only solid obstacles), consistent with "Level 1 = simplest."

## 12. Vault Rules

- R12.1 The Vault exists only in Level 3.
- R12.2 The Vault's `unlocked` state is false by default and becomes true the instant its `UnlockCondition` (a specific set of required clue IDs, optionally plus a minimum gem count — defined in that level's `LevelData`) is fully satisfied.
- R12.3 The player **cannot** enter/interact with the Vault's interior (i.e., cannot trigger Treasure spawning) while `unlocked == false`; attempting to interact while locked plays a "locked" feedback cue (see `Design.md`) and has no other effect.
- R12.4 The Vault, once unlocked, stays unlocked for the remainder of that level attempt (does not re-lock if a clue is somehow "un-collected," which cannot happen under normal play).
- R12.5 Entering the unlocked Vault's interaction trigger and pressing Interact spawns exactly one Treasure object, once. Repeating the interaction after the Treasure has already spawned/been collected has no further effect.

## 13. Treasure Rules

- R13.1 The Treasure exists only in Level 3 and only after the Vault is unlocked and interacted with (§12.5).
- R13.2 Collecting the Treasure follows the same overlap-pickup rule as gems/clues (R3.1) — no separate interact press required once it has spawned.
- R13.3 Collecting the Treasure sets `Player::hasTreasure = true` for the remainder of the level attempt; this flag resets to false only on a full level restart (§12/§Life Rules interplay — losing all lives after obtaining the Treasure but before reaching the boat means the Treasure must be re-obtained after the level restarts from the beginning, since the Vault/Treasure state resets along with everything else on full restart).

## 14. Rescue Boat Rules

- R14.1 The Rescue Boat exists only in Level 3.
- R14.2 The Rescue Boat's interaction trigger is present from the start of Level 3 but **inert** (visible, non-functional) while `Player::hasTreasure == false`.
- R14.3 Once `hasTreasure == true`, the trigger becomes active; the player must both overlap it and press Interact (unlike gems/clues, this final action requires an explicit confirm, matching its narrative weight as the game's climax) to trigger Victory.
- R14.4 Reaching the Rescue Boat's location without the Treasure has no special effect beyond an optional flavor prompt (e.g., "I need the treasure first") — it never causes a life loss or error state.

## 15. Victory Rules

- R15.1 Victory triggers exactly once per playthrough, on the frame the player interacts with an active Rescue Boat trigger (R14.3).
- R15.2 On Victory, `GameStateManager` transitions to the `Victory` state; the `LevelScene` for Level 3 is torn down; no further gameplay input (movement/interact) is processed except menu navigation on the Victory screen.
- R15.3 From the Victory screen, the only available action is returning to the Main Menu (R20 governs further menu behavior); there is no "New Game+" or replay-from-victory shortcut in v1.

## 16. Game-Over Rules

- R16.1 **There is no permanent, terminal "Game Over" state in v1.** Losing all 3 lives in a level (`lives == 0`) always results in a **Level Restart** (§12 of this section is actually covered under Life Rules — cross-reference), never a return to Main Menu and never a "insert coin"-style dead end.
- R16.2 A Level Restart: reloads that level's `LevelData` fresh, resets `Player::lives = 3`, `collectedGemsThisLevel = 0`, `collectedClues = {}`, `hasTreasure = false` (if applicable), `currentCheckpoint = 0`, and repositions the player at the level's start position.
- R16.3 A Level Restart does **not** affect progression already made in *other* levels (e.g., restarting Level 2 does not lock Level 1 or erase the fact that Level 1 was completed).
- R16.4 If the player explicitly chooses "Restart Level" from the Pause Menu, the exact same reset in R16.2 applies, regardless of remaining lives at the time.

## 17. Pause Rules

- R17.1 Pressing Escape while `GameStateManager` is in `Playing` transitions to `Paused`; the `LevelScene` update loop (movement, collision, timers, hazards) is fully frozen — no gameplay state changes while paused.
- R17.2 Pressing Escape while `Paused` (or selecting "Resume") returns to `Playing` with all state exactly as it was the instant before pausing.
- R17.3 Pause is unavailable during `LevelTransition` and `Victory` states (Escape is ignored in those states in v1) and during the brief death-feedback window (R8.2) — Escape presses during that window are queued/ignored, not dropped in a way that causes a stuck state; the very next frame after the window ends, Pause becomes available again.
- R17.4 Menus reachable from Pause: Resume, Restart Level (R16.4), How to Play, Main Menu (which ends the current level attempt without saving level-attempt progress, though already-completed levels remain unlocked per R5.2), Quit.

## 18. Input Rules

- R18.1 During `LevelTransition` and the initial load frames of `Victory`, all gameplay input (movement, interact) is ignored except whatever input is explicitly needed to dismiss/continue the transition card (Interact/Confirm).
- R18.2 Input pressed during a state transition is **not** buffered into the next state — e.g., holding a movement key while a "Level Complete" card is showing does not cause the player to start moving the instant the next level loads; the key must be actively held/pressed again once `Playing` resumes. (Exception: this does not apply to simply *holding* a key that remains held — GLFW will report it as still down, and the new level will honor it as of the first fixed update in the new `Playing` state; the guarantee is only that no queued/buffered discrete "press" event carries over.)
- R18.3 Multiple simultaneous opposite-direction keys (e.g., A and D both held) cancel to zero net horizontal movement, rather than picking one arbitrarily or summing to a larger value.

## 19. Camera Rules

- R19.1 The camera always keeps the player within the visible viewport, centered when possible.
- R19.2 The camera's visible rectangle is clamped to `LevelData::bounds`; near level edges the player may appear off-center rather than the camera showing area outside the level.
- R19.3 The camera never rotates and never zooms during normal gameplay in v1.

## 20. Interaction Rules

- R20.1 "Interaction" (Space/Enter) is required only for: the Vault (§12.5) and the Rescue Boat (§14.3), plus dismissing dialog/clue-text popups and confirming menu selections. Gems, clues, and checkpoints are overlap-triggered automatically (no key press).
- R20.2 If the player presses Interact while overlapping more than one interactable simultaneously (should not normally occur given level authoring, but must be handled), only the interactable with the smallest object ID (deterministic tie-break) responds that frame.

## 21. Edge Cases (Explicit)

- E1. **What happens when the player touches a gem?** It is collected immediately (§3.1); no key press needed.
- E2. **Can collected gems be collected again?** No (§3.2) — removed from the world on first pickup.
- E3. **What happens after death?** A brief feedback cue plays, life count decrements, and the player respawns at the last checkpoint with gems/clues retained (§7–§9).
- E4. **What happens when lives reach zero?** The current level fully restarts from the beginning; all progress made in that specific attempt (gems, clues, vault/treasure state) resets, but progress in *other* levels is unaffected (§16).
- E5. **Can the player enter the vault without the required clue?** No — the Vault remains non-interactive/locked until its unlock condition is fully met (§12.3).
- E6. **Can the player leave a level before completing it?** Not via the level's exit trigger (it is inert until the completion condition is met, §6.5); the player *can* return to the Main Menu via the Pause Menu at any time, which ends the current attempt without penalty to previously-completed levels (§17.4, §5.2).
- E7. **What happens when the player reaches the rescue boat (without treasure)?** Nothing beyond an optional flavor prompt; no life loss, no error (§14.4).
- E8. **What happens when the player reaches the rescue boat with the treasure and interacts?** Victory is triggered (§15.1).
- E9. **What happens if the player presses keys during a transition?** Movement/interact input is ignored and not buffered forward except as clarified in R18.2; only the designated continue/confirm action advances the transition.
- E10. **What happens if the player moves outside the playable area?** They cannot — level bounds act as an implicit solid boundary (R1.5).
- E11. **What happens if a hazard is touched repeatedly while overlapping?** Only one life-loss event fires per entry, followed by a brief invulnerability window (R11.1–R11.2).
- E12. **What happens if the player pauses mid-hazard-overlap?** Gameplay state (including overlap timers) freezes entirely; no life loss can occur while paused (R17.1).
