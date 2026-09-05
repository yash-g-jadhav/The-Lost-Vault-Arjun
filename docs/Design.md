# Design.md — The Lost Vault (Game Design & Visual Specification)

> **Authority note:** Visual/art specifics live here. Gameplay *behavior* is authoritative in `Rules.md`; where a number here (e.g., gem count) differs from `PRD.md`, treat `PRD.md` §11 as the gating requirement and this document as the placement/visual detail.

---

## 1. Visual Identity

Clean, flat, vector-style 2D look: solid-color polygons and circles with thin (1–2px equivalent) darker outline accents, no gradients, no photographic textures. Think "flat-design island postcard" rather than pixel art or painterly rendering. Everything is achievable with OpenGL primitives (triangle fans/lists) plus at most a handful of small flat-color PNGs for icons (HUD, font atlas).

## 2. Art Direction

- Shapes are simple geometric compositions: circles, rounded-looking polygons (approximated via many-sided fans), rectangles, and triangles.
- Depth is implied through layering (back-to-front draw order) and slight size/shade variation, not perspective (the game is strictly orthographic/2D).
- Silhouettes must read clearly at a glance — the sailor, gems, clues, hazards, and the vault must each be instantly distinguishable by shape+color alone.

## 3. Color Philosophy

| Element | Color (approx. hex) | Notes |
|---|---|---|
| Sky/background | `#8FD3E8` (day) → `#F2C879` (Level 3 dusk tint) | Level 3 shifts warmer to signal climax |
| Water | `#2C8AC1` base, `#5FB6E0` shallows | Two-tone to imply depth without 3D |
| Sand | `#F0DCA0` | |
| Grass/land | `#5BA65B` (L1), `#3F8A4A` (L2, denser) | Darker/denser green signals increasing difficulty |
| Tree trunk | `#7A5230` | |
| Tree foliage | `#3D7A3D` | |
| Rocks/stones | `#8C8C86` | |
| Bushes | `#4E8A4E` | |
| Sun/clouds | `#FFF3B0` sun, `#FFFFFF` clouds at 90% opacity | |
| Gems | Distinct hue per gem "type" for variety: `#E63946` (red), `#2A9D8F` (teal), `#F4A300` (amber) — purely cosmetic, all gems function identically | |
| Clues | `#EADFC0` scroll body, `#5C4530` binding | |
| Hazards | `#C1440E` (danger accent, e.g., spike tips / quicksand ripple lines) | Consistent "danger orange-red" language across all hazard types |
| Locked gate / Vault (locked) | `#4A4A4A` cold gray | |
| Vault (unlocked) | `#D4AF37` gold accent replaces gray | Visually communicates state change instantly |
| Treasure | `#FFD700` gold with `#B8860B` shading | |
| Rescue boat (inert) | `#7A5230` dull wood tone | |
| Rescue boat (active) | Add `#FFFFFF` sail + subtle glow outline | |
| Player (sailor) | `#2E4A62` shirt, `#E8B88A` skin tone, `#3D2B1F` hair, `#C9A15A` neutral trousers | |
| UI panels | `#1E1E1EAA` semi-transparent dark backing, `#FFFFFF` text | |

Danger/hazard colors are reserved exclusively for hazards to keep player threat-recognition unambiguous (no other object category uses `#C1440E`).

## 4. Environment Design

### 4.1 Island Layout Principles

- Each level is built on an implicit grid of 1 world unit = 1 meter (see §10), with hand-authored object placement (no procedural generation).
- Levels read left-to-right / bottom-to-top as a rough progress direction (start near the "shore" edge of the level bounds, objective near the opposite edge), giving the player an intuitive sense of advancement.
- Paths (lighter-toned sand/dirt-colored strips) visually guide the player between key objects without hard-blocking exploration off-path.
- Bridges are simple rectangular plank compositions spanning a water gap, always paired with visible water on both sides to make their function obvious.

### 4.2 Level Bounds & Sizing

| Level | World bounds (W×H, units) | Approx. object count |
|---|---|---|
| 1 — The Landing Shore | 40 × 24 | ~18 static objects, 3–5 gems, 1 clue |
| 2 — The Overgrown Interior | 56 × 32 | ~30 static objects, 5–7 gems, 2 clues, 1 locked gate, 2 hazards |
| 3 — Vault Island | 48 × 28 | ~24 static objects, 4–6 gems, vault, treasure, rescue boat, 1–2 hazards |

## 5. Level Design (Per-Level Object Manifests)

### 5.1 Level 1 — The Landing Shore

- Start position: `(4, 4)` — on the sand, near a wrecked-ship silhouette decoration (cosmetic only, not interactive).
- Static: water border along the bottom edge (solid, blocks movement, represents the ocean), a scatter of 6 rocks and 5 trees forming a loose corridor toward the interior, 2 bushes as minor decoration.
- Gems: 4 gems placed along both the direct path and one small optional detour (rewarding exploration without requiring it — `gemsRequired = 3`).
- Clue: `Clue_1_A` ("A torn page mentions a path guarded by stone... and a second hidden further in.") placed just off the main path behind a rock cluster (light detour, not hidden).
- Checkpoints: checkpoint 1 at the path's midpoint `(20, 12)`.
- Exit: `LevelExitTrigger` at `(38, 20)`, an inland path opening framed by two trees.

### 5.2 Level 2 — The Overgrown Interior

- Start position: `(4, 16)` (carried over narratively from Level 1's exit).
- Static: denser tree/rock placement forming two distinct branching paths (a "safe but longer" route and a "short but hazardous" route) that both reconverge before the locked gate.
- Hazards: one quicksand patch (`(24, 10)`, roughly 3×3 units) on the short route, one patrolling-style hazard represented as a stationary trap with a telegraphed danger zone (spike trap at `(34, 18)`) — both cost 1 life on overlap per Rules.md §11.
- Gems: 6 gems (`gemsRequired = 5`), distributed so at least 2 require taking the short/hazardous route (risk-reward), the rest safely reachable on the long route.
- Clues: `Clue_2_B` on the safe route, `Clue_2_C` guarded near the hazardous route's end.
- Locked gate: at `(44, 16)`, requires both `Clue_2_B` and `Clue_2_C` (via `UnlockCondition`) to open; visually a `#4A4A4A` gate that shifts to an open/gold-trimmed state when unlocked.
- Checkpoints: checkpoint 1 after the path split `(20, 16)`, checkpoint 2 just before the locked gate `(40, 16)`.
- Exit: `LevelExitTrigger` just past the gate at `(54, 16)`.

### 5.3 Level 3 — Vault Island

- Start position: `(4, 14)`.
- Static: the most complex layout — ruins-style rock formations forming a semi-maze, one bridge crossing a water channel at `(24, 14)`.
- Hazards: 2 hazards (one quicksand, one spike trap) reusing Level 2's mechanics (no new hazard *types* introduced here — consistent with "don't over-engineer," per PRD Non-Goals) but placed more centrally along the critical path to raise difficulty.
- Gems: 5 gems (`gemsRequired = 4`), positioned so the last gem requires crossing the bridge.
- Vault: located at `(40, 14)`, `UnlockCondition = { requiredClues: [Clue_1_A, Clue_2_B, Clue_2_C] }` (all three prior clues, tying the whole game's narrative together) — no additional clue is introduced in Level 3 itself.
- Treasure: spawns inside the Vault's footprint at `(41, 14)` once unlocked and interacted with.
- Rescue Boat: at `(46, 4)`, near the level's shore edge, inert until `hasTreasure == true`.
- Checkpoints: checkpoint 1 after the bridge `(28, 14)`, checkpoint 2 just before the Vault `(38, 14)`.

## 6. Player Design

- **Shape:** A simplified sailor built from primitives in a hierarchical transform chain: `Root (position/facing)` → `Torso (rounded rectangle)` → `{Head (circle, child of Torso top), LeftArm (thin rectangle, child of Torso), RightArm (thin rectangle, child of Torso), LeftLeg (thin rectangle, child of Torso bottom), RightLeg (thin rectangle, child of Torso bottom)}`.
- **Approximate dimensions:** Overall bounding box ~0.8 × 1.6 world units (torso 0.5×0.8, head radius 0.2, limbs 0.15×0.6).
- **Transformations:** Root translates/rotates(facing flip via X-scale for left/right, no true rotation needed for a top-down-ish 2D sailor) per input; limbs rotate slightly (±15°) in an alternating walk-cycle pattern around their shoulder/hip pivot (child-local rotation composed with the parent's world matrix) — this is the project's explicit hierarchical-transformation showcase.
- **Animation:** 2-frame (or simple procedural sine-based) walk cycle when `velocity != 0`; idle = neutral pose with a slow "breathing" scale pulse (±2% Y-scale on the torso) when stationary.
- **Collision bounds:** AABB slightly smaller than the visual silhouette, 0.6 × 1.2 units, centered on the root position, so near-misses feel fair.
- **Rendering priority/layer:** Player renders on the "actors" layer, above ground/path decoration, below tree-canopy foliage that is explicitly marked "in front" (rare, used sparingly for depth near tall trees the player can walk behind the base of).

## 7. Tree Design

- **Shape:** Trunk = narrow rectangle (`#7A5230`), foliage = 1–2 overlapping large circles (`#3D7A3D`) offset above the trunk.
- **Composition:** `TreeRoot` → `Trunk (child)`, `TreeRoot` → `FoliageA (child)`, `TreeRoot` → `FoliageB (child, slightly offset)` — a second, simpler hierarchical-transform demonstration object per PRD graphics requirements.
- **Approx. dimensions:** Trunk 0.3 × 1.0 units, foliage circles radius ~0.7–0.9 units.
- **Transformations:** Static in-level (translation only at placement time); no runtime transform beyond a very subtle idle sway (optional NICE TO HAVE — small ±2° foliage rotation oscillation) which, if implemented, is a second animation example beyond the player.
- **Collision bounds:** AABB around the trunk only (0.3 × 0.4 near the base), not the foliage — foliage is purely visual overhang, so the player can partially "tuck under" wide canopies, matching how the collision footprint of a tree trunk should feel.
- **Rendering priority/layer:** Trunk on "ground objects" layer; foliage on a slightly higher layer so it can visually overlap adjacent path tiles.

## 8. Rock Design

- **Shape:** Irregular-looking polygon approximated by a 6–8 sided fan with slight per-vertex radius jitter (baked once at level-load, not per-frame) so rocks don't look like perfect hexagons.
- **Approx. dimensions:** Radius 0.4–0.7 units, varied per instance for visual variety.
- **Transformations:** Static; random rotation applied once at placement (from level data, not runtime-random, to keep level layout deterministic/reproducible for testing).
- **Collision bounds:** AABB tightly fit to the polygon's bounding box.
- **Rendering priority/layer:** "Ground objects" layer, same as tree trunks.

## 9. Gem Design

- **Shape:** A faceted diamond: a triangle fan approximating an 8-sided gem outline with a lighter-tone triangle overlay near the top-left to fake a specular highlight (still flat shading, just a second flat triangle, not a real light calculation).
- **Approx. dimensions:** ~0.4 × 0.4 units.
- **Transformations:** Continuous slow Y-axis "bob" (sine wave, ±0.05 units) and slow rotation (very slight, ±5°) while idle in the world, to read clearly as "collectible" versus static scenery. On pickup: rapid scale-up-then-fade-out over ~0.3 seconds (Design §Feedback Effects).
- **Collision bounds:** Circular-approximated-as-small-AABB trigger, radius 0.3 units.
- **Rendering priority/layer:** "Pickups" layer, above ground/path, below UI.

## 10. Clue Design

- **Shape:** A rolled scroll — a rectangle with two small circles at each end (representing rolled ends) — `#EADFC0` body, `#5C4530` end-caps.
- **Approx. dimensions:** ~0.5 × 0.3 units.
- **Transformations:** Same idle bob as gems (shared "pickup" visual language) but no rotation (scrolls don't spin, to visually distinguish the category from gems at a glance despite the shared bob).
- **Collision bounds:** AABB trigger, 0.4 × 0.3 units.
- **Rendering priority/layer:** "Pickups" layer.

## 11. Treasure Design

- **Shape:** A chest — rectangle base + a slightly smaller arched-lid shape (approximated via a half-circle fan) in gold tones, with 2–3 small rectangle "gem" accents on the lid for flavor.
- **Approx. dimensions:** ~1.0 × 0.7 units.
- **Transformations:** Spawns with a brief scale-in (0→1 over 0.4s) and a one-shot "lid pop" rotation flourish (small child transform on the lid piece, another hierarchical-transform usage) at spawn time; otherwise static until collected.
- **Collision bounds:** AABB trigger, 0.8 × 0.6 units.
- **Rendering priority/layer:** "Pickups"/"special objects" layer, drawn above the Vault's base structure since it spawns "in front of" the vault opening.

## 12. Vault Design

- **Shape:** A large stone archway/door composition: a rectangle body plus a half-circle-fan arch on top, with a distinct door-seam line (a thin darker rectangle down the middle) that visually shifts from closed-gray to open-gold-trimmed when unlocked.
- **Approx. dimensions:** ~3.0 × 3.5 units.
- **Transformations:** Static position; on unlock, plays a ~0.6s color-transition + a subtle "door seam widening" scale effect on the seam-line child element (door seam scales X from 1.0→0.15, implying it's sliding open) — another hierarchical parent/child transform usage (Vault root → door-seam child).
- **Collision bounds:** The Vault body is solid (blocks walking through the wall) except its front-facing interaction trigger zone (a smaller AABB in front of the archway) which is trigger-only and is what the Interact key checks against.
- **Rendering priority/layer:** "Structures" layer, drawn above ground/path, below foliage-overhang layer only if a tree happens to overlap it (rare/avoided by level authoring).

## 13. Boat Design

- **Shape:** A simple hull (a wide, flattened half-circle/trapezoid-fan shape) plus a mast (thin rectangle) and a triangular sail.
- **Approx. dimensions:** ~2.5 × 2.0 units (hull + mast height).
- **Transformations:** Gentle continuous idle bob/rock (sine-based rotation, ±3°, pivoting at the hull's center) at all times, representing it floating; sail color/opacity shifts (dull → full white, per §3 color table) the instant it becomes active (`hasTreasure == true`), giving a clear "ready to go" visual cue without needing new geometry.
- **Collision bounds:** AABB trigger only (never solid — it's meant to be walked up to and boarded, not an obstacle).
- **Rendering priority/layer:** "Structures"/"pickups-tier" layer, drawn near the shore's water-line boundary.

## 14. UI Design

- **Overall style:** Semi-transparent dark rounded-rectangle panels (`#1E1E1EAA`) with white text and simple flat icon glyphs (heart for lives, gem-shape for gem counter, scroll-shape for clue counter) — visually consistent with the in-world gem/clue shapes so the HUD icons feel like "the same objects, just as icons."
- **Layout:** Top-left: lives (3 heart icons) + gem counter (`x / required`). Top-right: level name/number (auto-fades after ~3 seconds of no HUD-relevant events). Bottom-left (appears only after a clue pickup, auto-hides after ~4 seconds unless the player opens the Clues sub-panel via a dedicated small "view clues" icon): the clue text flash.

## 15. HUD (Detail)

- Heart icons: filled `#E63946` when a life remains, outlined/hollow gray `#4A4A4A` when lost, updates instantly on life-loss.
- Gem counter: format `"{collected} / {required}"`; briefly pulses (scale 1.0→1.15→1.0 over 0.2s) on each new pickup for tactile feedback.
- Clue flash: slides in from the left edge, holds, slides out; does not block gameplay input while visible (purely decorative overlay, no pause).

## 16. Menus

- **Main Menu:** Centered vertical button stack (Start Game / Continue / How to Play / Quit) over a static decorative island-vista background (a simplified, non-interactive composition using the same environment primitives — sun, clouds, water, a distant island silhouette).
- **Pause Menu:** Same button-stack visual language, overlaid on a dimmed (70% dark overlay) freeze-frame of the current gameplay view, so the player retains spatial context of where they paused.
- **Level Transition:** A centered card ("Level {N} Complete!") with a "Press Enter/Space to Continue" prompt; background dims to 80% black behind it.
- **Victory Screen:** Full-screen warm-toned (dusk palette) background with a centered "You Escaped the Lost Vault!" headline, a short one-line congratulatory flavor text, and a single "Return to Main Menu" button.

## 17. Transitions

- All screen-level transitions (Menu↔Playing, Playing↔Paused, LevelTransition, Victory) use a simple 0.25s fade-to-black / fade-from-black rather than any directional wipe or 3D-style effect, keeping the transition system trivial to implement and consistent everywhere it's used.

## 18. Animation

- Player walk cycle (Design §6), idle breathing pulse, gem/clue idle bob, treasure spawn flourish, vault unlock transition, boat idle rock + activation color shift, HUD gem-counter pulse, and screen fade transitions constitute the **complete** animation scope for v1 — deliberately bounded so the `AnimatorComponent` stays simple (drives a small enum of named animation states per object type, each a straightforward time-based interpolation, no animation-blending system needed).

## 19. Camera Behavior

- Smooth-follow: camera target position lerps toward the player's position at a fixed smoothing factor (e.g., `lerp(camPos, playerPos, 1 - exp(-k*dt))` with `k ≈ 8`) rather than snapping instantly, to reduce visual jitter, while still feeling responsive (Rules.md §19 governs the hard constraints; this section governs the *feel* within those constraints).

## 20. Feedback Effects

- **Gem/clue pickup:** scale-up-then-fade over 0.3s + HUD pulse (§15).
- **Hazard hit / death:** brief red-tinted screen vignette flash (0.2s) + player silhouette flash white twice, matching the ≤1 second feedback window in Rules.md R8.1.
- **Level completion:** brief full-screen warm flash (0.15s) immediately before the Level Transition card fades in.
- **Vault unlock:** color-shift + door-seam scale animation (§12).
- **Victory sequence:** boat sail color shift (§13) immediately followed by the 0.25s fade into the Victory Screen.

## 21. Death Effects

(See §20 "Hazard hit / death" — intentionally understated, non-graphic, and quick, consistent with the game's approachable, low-punishment tone from `PRD.md` §8.)

## 22. Gem Collection Effects

(See §20 — covered; no additional particle system is introduced, per Non-Goals on avoiding unnecessary complexity.)

## 23. Level Completion Effects

(See §20 — covered.)

## 24. Victory Sequence

1. Player interacts with the active Rescue Boat.
2. Boat sail flashes to full white/active color (§13).
3. Screen fades to black (0.25s).
4. Victory Screen fades in (§16) with headline text.
5. Player may return to Main Menu.

## 25. Coordinate System (Consistent Definition Used Everywhere)

- **World space:** Right-handed 2D, X increases rightward, Y increases **upward** (not screen-down), origin `(0,0)` at the bottom-left corner of each level's bounds. Units are abstract "world units" (~1 unit ≈ 1 meter of in-fiction scale).
- **Pixels-per-unit (PPU):** 64 — used only for texture/asset authoring reference and for sizing the orthographic view (`viewWidthUnits = windowWidthPixels / PPU`), not a runtime-visible concept to gameplay code.
- **Screen/UI space:** Separate space, origin top-left `(0,0)`, X right, Y **downward**, in raw pixels, matching typical UI convention — deliberately different from world space so `Architecture.md` §14's "two separate projection passes" is easy to reason about (never mix the two spaces in one draw call).
- **Transform hierarchy convention:** A child's world matrix = `parent.worldMatrix * child.localMatrix`, where `localMatrix = translate(localPos) * rotate(localRotationDeg) * scale(localScale)`, computed top-down once per frame per dirty transform (or every frame, given the small object counts here — no dirty-flag optimization is required at this scale, per Non-Goals on avoiding unnecessary engineering).
