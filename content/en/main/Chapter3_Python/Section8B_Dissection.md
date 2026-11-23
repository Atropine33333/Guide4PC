# Section 8B: Project Dissection – Taking Apart a Real Tank Battle Game

8A covered the rules of modules and imports. This section zooms in and dissects TankBattle layer by layer: first the directory structure, dependency graph, and complete data flow, then a module‑by‑module analysis of responsibilities and design.

---

## 8B.1 The Project at a Glance

Before reading code line by line, let's build a "sense of the map": how many drawers this project has, and how they connect.

### 8B.1.1 Directory Structure and Running

```
TankBattle/
├── main.py             # Entry point: main loop, terminal init and restore
├── config.py           # All constants: dimensions, speeds, colours, enemy/powerup attributes
├── battlefield.py      # Terrain generation and terrain queries
├── tank.py             # Tank base class
├── player.py           # Player tank
├── enemy.py            # Enemy tanks + AI
├── bullet.py           # Bullets
├── powerup.py          # Power-ups
├── game.py             # Game state machine, collisions, spawning, scoring
├── display.py          # ANSI rendering, HUD, differential refresh
├── input_handler.py    # Cross-platform non-blocking keyboard input
├── README.md           # Gameplay, keys, legend
└── highscore.txt       # High-score save file generated at runtime (may not exist)
```

How to run:

```bash
cd TankBattle
py main.py
```

Requires a terminal at least 78 columns × 19 lines. All files are flat in a single directory – no packages – because there aren't many modules and the dependencies are clear.

### 8B.1.2 Each Module's Responsibility and Size

| Module | Approx. lines | Responsibility | Imports |
|--------|--------------:|----------------|---------|
| `config.py` | 148 | All constants: dimensions, speeds, colours, enemy stats, power-up tables | stdlib `pathlib` |
| `battlefield.py` | 189 | Generate map; check whether a tile is passable; provide base/spawn points | `config`, `random` |
| `tank.py` | 78 | Tank base class: position, direction, HP, movement, fire permission | `config` |
| `player.py` | 94 | Player tank: firepower level, shield, key memory | `tank`, `config` |
| `enemy.py` | 114 | Enemy tanks: type-based init, pathfinding, aiming and firing | `tank`, `config`, `random` |
| `bullet.py` | 24 | Bullets: position, direction, flight timer, owner | `config` |
| `powerup.py` | 18 | Power-ups: type, position, lifetime | `config` |
| `game.py` | 450 | Game state machine, collisions, spawning, power-ups, scoring, save file | All business modules + `config` |
| `display.py` | 213 | Render game data to strings, differential terminal refresh | `config`, `unicodedata`, `shutil`, etc. |
| `input_handler.py` | 126 | Non-blocking keyboard reading, arrow/function key parsing | stdlib `os`, `select`, `sys`, `time` |
| `main.py` | 103 | Entry: assemble objects, main loop, restore terminal on exit | `config`, `display`, `game`, `input_handler` |

Reading these numbers, you'll notice two facts:

1. **The 450-line `game.py` is the only "big file"**, because game rules are inherently complex; all other modules are between 20 and 220 lines, and each can be read in one sitting.
2. **`config.py` is depended on by almost everyone, and depends on no one**. This is intentional – constants go at the bottom layer, so changing one place takes effect globally.

### 8B.1.3 Dependency Graph and Layering

```
                                main.py
                    ┌──────────────┼──────────────┬─────────────┐
                    ▼              ▼              ▼             ▼
               input_handler   display         game        config
                (no local deps) (only config)   │
                    ┌──────────┬──────────┬───────┼────────┬──────────┐
                    ▼          ▼          ▼       ▼        ▼          ▼
              battlefield   player     enemy   bullet  powerup     (config)
                    │          │          │       │        │
                    ▼          ▼          ▼       ▼        ▼
                 config      tank ────── tank   config   config
                               │
                               ▼
                             config
```

Simplified into "layers":

```
Layer 0 (constants)     config.py
Layer 1 (base data)     battlefield.py  bullet.py  powerup.py  tank.py
Layer 2 (entities)      player.py  enemy.py
Layer 3 (world/rules)   game.py
Layer 4 (periphery)     display.py  input_handler.py
Layer 5 (entry)         main.py
```

**Key principles** (consistent with the earlier version, but re-verified with the new project):

- `config.py` **depends on no business module**, so any module can safely import it without creating a cycle.
- `battlefield.py` only produces "terrain data"; it doesn't know what a tank is, nor who will render it.
- `tank.py` is the base class; `player.py` and `enemy.py` inherit from it. Entities don't import `game` – when they need world information, the caller passes `game` in (8A.2.9's dependency inversion).
- `game.py` is the only module that knows all entities at once: it is the "world".
- `display.py` **does not import `game`**: in `render_frame(game)`, `game` is just a parameter, duck typing.
- `main.py` is the only module that "knows everyone", but its code is the thinnest – it only creates objects and runs the loop.

### 8B.1.4 What Happens in One Frame

The biggest difference between a real-time game and the "input → compute → output" pattern from previous sections: it **loops continuously**. Even if you don't press a key, enemies, bullets, and timers keep moving.

Here's one iteration of the loop (one frame) in TankBattle:

```
1. Poll input (main.py)
   Call keys.read_keys() to grab all keys pressed during this short interval
   → game.handle_input("up") / game.handle_input("fire") ...

2. Advance logic (main.py → game.py)
   Add real elapsed time to accumulator
   As long as accumulator >= LOGIC_STEP (1/120 s), call game.update(LOGIC_STEP)
   (may be called 0, 1, or several times per frame)

3. game.update dispatches by state machine
   READY        → countdown ends, enter PLAYING
   PLAYING      → _update_playing()
   STAGE_CLEAR  → countdown ends, next stage
   GAME_OVER    → only update explosion effects, wait for R/Q
   PAUSED       → return immediately

4. Fixed order inside _update_playing
   a. Tick freeze timer
   b. player.update()   —— handle key intent: move, fire, shield countdown
   c. _update_spawning() —— should a new enemy appear
   d. enemy.update() for each —— move, replan, fire
   e. _update_bullets()   —— bullets step tile by tile, resolve collisions
   f. _update_powerups()  —— power-up countdown, pickup
   g. _remove_dead()      —— uniformly clear dead entities
   h. _update_extra_life() / _check_stage_clear()

5. Render (main.py → display.py)
   render_frame(game) assembles post-step-4 data into:
     16 battle rows + 3 HUD rows (score/high score/stage/lives/firepower/enemies/hint)
   Renderer.draw() writes only the lines that "differ from the previous frame"

6. Sleep 2 ms, go back to step 1
```

The three time scales of the main loop:

| Frequency | Purpose | Constant |
|-----------|---------|----------|
| ~500 Hz | Poll keyboard so no key press is lost | `INPUT_POLL_SLEEP = 0.002` |
| 120 Hz | Game logic updates (fixed step) | `LOGIC_STEP = 1/120` |
| 60 Hz | Render the screen | `FRAME_TIME = 1/60` |

Why separate logic from rendering? This is exactly what 8C.19 will fill in.

### 8B.1.5 Why Layer It This Way

Verify with three "what if they were mixed together" scenarios:

- **If map generation and rendering were written together**: you'd want to change the brick colour but might accidentally break the map randomisation algorithm. Now changing colour only touches `config.TERRAIN_COLORS`.
- **If enemy AI were written inside `game.py`**: `game.py` would grow from 450 lines to 600+, and upgrading to a smarter AI would require surgery inside the rule code. Now it's just `enemy.py`.
- **If key reading were written inside `player.py`**: the player tank would depend on `termios`, `msvcrt`, and `select` all at once; running on Windows would mean changing the player class. Now all platform differences are isolated in `input_handler.py`, and game logic knows nothing.

This is **separation of concerns**: each module cares only about its own thing.

### 8B.1.6 When to Keep Splitting

`game.py` at 450 lines is the next candidate for splitting. Rules of thumb:

- A single file exceeds **500 lines**, becoming hard to read;
- A **second obvious responsibility** appears in the file (e.g., "save file" mixed with "collision");
- A function **you want to test alone** can't be imported because of other stuff in the file;
- Two people frequently edit **the same file**.

For `game.py`, candidate split points are: `collision.py` (collision detection), `spawning.py` (spawning and stage difficulty), `scoring.py` (scoring and save). But at 450 lines it's still in the "readable in one sitting" range – **not splitting is also correct** – over-splitting shreds call relationships. Architecture is a trade-off, not "more files is better".

---

## 8B.2 Module-by-Module Analysis: What's Inside Each Drawer

This section reads code from bottom layer to top. Each excerpt shows only **key fragments**, focusing on design intent and easy-to-misunderstand points. I recommend opening the corresponding file alongside reading.

### 8B.2.1 config.py: The Single Source of Truth

**Responsibility**: hold all "numbers" and "character tables", no logic.

**Fragment 1: Time parameters**

```python
# ------------------------------------------------------------------ timing
FRAME_TIME = 1 / 60          # target seconds per rendered frame
LOGIC_STEP = 1 / 120         # fixed simulation step
INPUT_POLL_SLEEP = 0.002     # seconds between input polls in the main loop
TAP_HOLD_TIMEOUT = 0.15      # keep moving briefly after a tap
HOLD_TIMEOUT_MIN = 0.09      # timeout once the key repeat cadence is known
HOLD_GRACE_MAX = 0.60        # cap for the adaptive hold timeout
REPEAT_MAX_GAP = 0.45        # larger gaps are treated as a fresh key press
FIRE_HOLD_TIMEOUT = 0.15     # keep trying to fire after the last key event
```

**Fragment 2: Terrain character table**

```python
TERRAIN_GLYPHS = {
    FLOOR: " ",
    BRICK: "砖",
    STEEL: "钢",
    WATER: "水",
    FOREST: "林",
    BASE: "鹰",
    BASE_RUBBLE: "毁",
}

TERRAIN_COLORS = {
    FLOOR: None,
    BRICK: "yellow",
    STEEL: "bright_white",
    WATER: "bright_blue",
    FOREST: "green",
    BASE: "bright_yellow",
    BASE_RUBBLE: "bright_red",
}
```

**Fragment 3: Enemy type table**

```python
ENEMY_TYPES = {
    "basic": {"char": "敌", "color": "red",
              "hp": 1, "speed": 1.0, "bullet_speed": 1.0, "score": 100},
    "fast":  {"char": "快", "color": "bright_cyan",
              "hp": 1, "speed": 1.8, "bullet_speed": 1.0, "score": 200},
    "power": {"char": "猛", "color": "magenta",
              "hp": 1, "speed": 1.0, "bullet_speed": 2.0, "score": 300},
    "armor": {"char": "甲", "color": "bright_yellow",
              "hp": 4, "speed": 0.7, "bullet_speed": 1.0, "score": 400},
}
# Armour tanks change color as they lose plating.
ARMOR_HP_COLORS = {4: "bright_yellow", 3: "yellow", 2: "red", 1: "gray"}
```

**Analysis**

1. **Why terrain uses string constants instead of writing "砖" directly?**

   ```python
   FLOOR = "floor"
   BRICK = "brick"
   STEEL = "steel"
   ```

   `battlefield.py` writes `grid[r][c] = BRICK`, not `grid[r][c] = "砖"`. This layer of indirection looks redundant, but it separates "**logical terrain type**" from "**displayed character**": to change bricks to `🧱`, only change `TERRAIN_GLYPHS[BRICK]`; to add a new terrain, logic code uses constant names and won't typo strings.

2. **Dictionaries as "small databases" – data-driven design**

   `ENEMY_TYPES` centralises four attributes of "enemy type" (char, colour, HP, speed, bullet speed, score) in one table. `enemy.py` just looks it up after receiving `type_name`:

   ```python
   info = ENEMY_TYPES[type_name]
   ```

   In 8D.2 you'll see: **adding a new enemy type requires only one row in the `config.py` table**, and zero lines in `enemy.py`. Without tables, writing a chain of `if type_name == "basic": ... elif ...` means changing logic code to add types.

3. **Tuple or dictionary?**

   ```python
   PLAYER_BULLETS_BY_POWER = (1, 1, 2, 2)
   PLAYER_BULLET_SPEED_BY_POWER = (1.0, 1.6, 1.6, 2.0)
   ```

   "Firepower level → bullet count" is a contiguous integer mapping starting at 0; a **tuple** is more compact than a dict, accessed as `PLAYER_BULLETS_BY_POWER[self.power]`.

4. **`Path(__file__).resolve().parent`**

   ```python
   HIGH_SCORE_FILE = Path(__file__).resolve().parent / "highscore.txt"
   ```

   `__file__` is the current file's path; `resolve()` makes it absolute; `.parent` gets the containing directory. This way, no matter which directory you run `python /path/to/TankBattle/main.py` from, the high-score file stays next to the code instead of running around with the "current working directory".

5. **Tuning game feel only touches this file**. Change `PLAYER_MOVE_INTERVAL` from `0.11` to `0.3` and the player slows down instantly; change `FRAME_TIME` to `1/30` and the screen drops to 30 FPS. No logic changes needed.

> **Reminder**: `config.py` holds **constants only**. If you write a stateful function there, every module that imports it shares that state, and the bug is hard to track. A constant table is the safest "public dependency".

---

### 8B.2.2 battlefield.py: Produces Maps, Doesn't Know Tanks

**Responsibility**: generate per-stage terrain; answer "can this tile be walked on" and "where is the base".

**Fragment 1: The main generation function**

```python
def create_battlefield(stage: int) -> list[list[str]]:
    """Build the terrain grid for a stage."""
    grid = [[FLOOR] * FIELD_WIDTH for _ in range(FIELD_HEIGHT)]
    _add_border(grid)
    _add_brick_blocks(grid, stage)
    _add_steel_blocks(grid, stage)
    _add_water(grid, stage)
    _add_forest(grid)
    _build_base(grid)
    _clear_spawn_zones(grid)
    return grid
```

**Fragment 2: Query functions**

```python
def in_bounds(row: int, col: int) -> bool:
    """Return True if (row, col) is inside the battlefield."""
    return 0 <= row < FIELD_HEIGHT and 0 <= col < FIELD_WIDTH


def is_tank_passable(tile: str) -> bool:
    """Return True if tanks may drive onto this tile."""
    return tile in (FLOOR, FOREST, BASE_RUBBLE)
```

**Fragment 3: Base and shield**

```python
base_row, base_col = base_position()
# First clear the "doorstep"
for r in range(base_row - 2, base_row + 1):
    for c in range(base_col - 5, base_col + 3):
        if in_bounds(r, c):
            grid[r][c] = FLOOR
# Then place the eagle
grid[base_row][base_col] = BASE
# Finally lay 5 bricks as a shield
shield = ((base_row - 1, base_col - 1), (base_row - 1, base_col),
          (base_row - 1, base_col + 1), (base_row, base_col - 1),
          (base_row, base_col + 1))
```

**Analysis**

1. **All functions, no classes**. Map generation is "input stage → output grid"; it doesn't need to remember anything between calls, so no class is needed. **Not everything belongs in a class** – a common beginner over-design.

2. **2D list comprehension** (recall 4.2):

   ```python
   grid = [[FLOOR] * FIELD_WIDTH for _ in range(FIELD_HEIGHT)]
   ```

   Create one row, then duplicate it `FIELD_HEIGHT` times. Note it must be a **comprehension**; don't write `[[FLOOR] * W] * H` – the latter makes all rows point to the same list, so changing one row changes all (recall 6.7 shallow/deep copy).

3. **Generation order matters**. Lay floor first → add border → random brick/steel/water/forest → **finally** clear base and spawn areas. Reversed (base first, then random bricks), bricks might bury the base. This "generate first, guarantee afterwards" approach is common in procedural generation.

4. **Random but playable**. `_clear_spawn_zones()` ensures the 3 enemy spawn points and player spawn have empty surroundings; `_build_base()` ensures there's standing area and a brick shield around the base. The most common random-map bug is "spawning an enclosed point"; guaranteed clearing is the defence.

5. **`is_tank_passable(tile)` takes one tile**, not the whole map – because "passable" depends only on this tile's terrain. Keeping function parameters as small as possible makes testing easiest.

> **Common question**: why isn't `destroy_brick()` in this file?
>
> Real-time bullet collisions need to "destroy brick" together with "spawn explosion effect" and "check if base was hit", so destruction is written in `game._step_bullet()`: `self.grid[r][c] = FLOOR`. This doesn't break layering – `game.py` already has the right to modify terrain. If "modify terrain" logic ever becomes complex (say bricks have 4 HP), you can extract a `terrain.py`.

---

### 8B.2.3 tank.py: Base Class and Dependency Inversion

**Responsibility**: attributes/methods shared by player and enemy: position, direction, HP, movement, fire permission.

**Fragment 1: Class attribute defaults**

```python
class Tank:
    # Defaults; subclasses may override them with properties.
    bullet_speed = 1.0
    max_bullets = 1
    shoot_cooldown = 0.3
    breaks_steel = False
```

**Fragment 2: Movement**

```python
    def try_move(self, direction: str, game) -> bool:
        """Turn towards *direction* and move one tile when possible."""
        self.direction = direction
        dr, dc = DELTAS[direction]
        new_row, new_col = self.row + dr, self.col + dc
        if game.can_tank_enter(self, new_row, new_col):
            self.row = new_row
            self.col = new_col
            return True
        return False
```

**Analysis**

1. **Why "try_move" not "move"?** A `try_` prefix clearly tells the caller "may fail". It returns `bool`, letting the caller decide what to do on failure (player: stay; enemy: change direction).

2. **It doesn't import `game`, it accepts `game`**. This is 8A.2.9's dependency inversion. Result:
   - Single dependency direction, no circular imports;
   - `Tank` doesn't care what class `Game` is, only that it has a `can_tank_enter(tank, row, col)` method;
   - Testing can pass a small fake object with only this method.

3. **Turning and moving are separate**: even if hitting a wall fails, `self.direction = direction` has already executed, so the tank "turns in place". This is crucial for shooters – pressed against a wall, pressing a direction key should rotate the turret rather than doing nothing.

4. **Class attributes as defaults** (`bullet_speed = 1.0`, etc.). These three are constants for a "basic tank", and `Player`/`Enemy` override them their own way (subclass uses `@property` or assigns in `__init__`). Why use class attributes rather than assigning in `__init__`? 8C.3 covers a real pitfall.

5. **Defensive style in `take_damage`**:

   ```python
   self.hp = max(0, self.hp - amount)
   if self.hp == 0:
       self.alive = False
   ```

   `max(0, ...)` ensures HP doesn't go negative; `alive` is an explicit flag rather than checking `hp > 0` every time. Both work, but the explicit flag makes "death" happen only once (e.g., play explosion only once).

---

### 8B.2.4 player.py: Subclass, Properties, Input Feel

**Responsibility**: player-tank-specific logic: firepower level, shield, turning keys into "move/fire intent".

**Fragment 1: Inheritance and initialisation**

```python
class Player(Tank):
    """The player tank: moves in real time and fires on demand."""

    def __init__(self, row: int, col: int) -> None:
        super().__init__(
            row, col, PLAYER_CHAR, PLAYER_COLOR,
            hp=1, move_interval=PLAYER_MOVE_INTERVAL, faction="player",
        )
        self.power = 0                 # 0..3, raised by star power-ups
        self.shield_time = SPAWN_SHIELD_TIME
        self.held_direction = None
        self.held_timer = 0.0
        self.fire_held = False
        self.fire_timer = 0.0
        self.shoot_cooldown = PLAYER_SHOOT_COOLDOWN
```

**Fragment 2: Three firepower parameters are computed**

```python
    @property
    def max_bullets(self) -> int:
        return PLAYER_BULLETS_BY_POWER[self.power]

    @property
    def bullet_speed(self) -> float:
        return PLAYER_BULLET_SPEED_BY_POWER[self.power]

    @property
    def breaks_steel(self) -> bool:
        return self.power >= 3
```

**Fragment 3: Keys only record "intent"**

```python
    def handle_key(self, key: str) -> None:
        """Remember a direction or the fire key (called on key events)."""
        if key in DELTAS:
            gap = self._clock - self._last_event
            if key == self.held_direction and gap < REPEAT_MAX_GAP:
                # Terminal key auto-repeat: detect the repeat interval so release stops quickly
                ...
                self.held_timer = max(
                    HOLD_TIMEOUT_MIN,
                    min(HOLD_GRACE_MAX, self._repeat_gap * 2.5),
                )
            else:
                # New direction key: move one step immediately for responsiveness
                if key != self.held_direction:
                    self.move_accum = self.move_interval
                self.held_timer = TAP_HOLD_TIMEOUT
            self._last_event = self._clock
            self.held_direction = key
        elif key == "fire":
            self.fire_held = True
            self.fire_timer = FIRE_HOLD_TIMEOUT
```

**Analysis**

1. **`super().__init__(...)`** means "let the parent initialise the common parts first". Player-specific `power`, `shield_time`, etc. are added in the subclass. This is the most common use of inheritance.

2. **`@property` disguises "computed values" as ordinary attributes**. Users write `tank.max_bullets` without knowing it's looked up from `power`. Upgrading firepower only changes `self.power` – bullet count, bullet speed, and steel-breaking all follow automatically. **Single source of truth**.

3. **`handle_key` doesn't move the tank, only remembers intent**. Real movement happens in `update(dt, game)` as time advances. Benefits:
   - Key events may arrive between logic updates (500 Hz polling); storing intent first never loses them;
   - Logic updates are fixed-step, so movement speed is stable, independent of how many keypresses per frame;
   - This is the concrete embodiment of "input → logic → render" separation.

4. **Long-press feel** (a terminal-specific pitfall): the terminal's "key auto-repeat" is sent by the OS; holding a direction key pauses ~0.2–0.6 s before continuous characters begin. If the program moves one tile per character, the feel is stuttery. `Player`'s approach:
   - Record the interval `gap` between two direction key events;
   - If the gap is less than `REPEAT_MAX_GAP` (0.45 s) and the direction is the same, treat it as auto-repeat and record the minimum repeat interval in `_repeat_gap`;
   - After release, allow movement for `2.5 × interval` more time (minimum 0.09 s) – no stutter, no sliding too far;
   - When a new direction is pressed, set `move_accum` to full so it **moves one tile immediately**, fixing "no response on press".

5. **`update` uses `while` to consume `move_accum`**:

   ```python
   self.move_accum += dt
   while self.move_accum >= self.move_interval:
       self.move_accum -= self.move_interval
       self.try_move(self.direction, game)
   ```

   If a `dt` is large (window hitch), `while` catches up with multiple steps; using `if` would make tank speed vary with frame rate. This is a proto-form of "time integration".

---

### 8B.2.5 enemy.py: AI Is Also Just a Subclass

**Responsibility**: enemy tank initialisation, pathfinding, aiming, and firing rhythm.

**Fragment 1: Table-driven initialisation**

```python
class Enemy(Tank):
    def __init__(self, row: int, col: int, type_name: str = "basic") -> None:
        info = ENEMY_TYPES[type_name]
        super().__init__(
            row, col, info["char"], info["color"], hp=info["hp"],
            move_interval=ENEMY_MOVE_INTERVAL / info["speed"],
            faction="enemy",
        )
        self.type_name = type_name
        self.bullet_speed = info["bullet_speed"]
        self.score = info["score"]
        self.shoot_timer = random.uniform(0.5, 1.4)
        self.plan = None                 # direction attempt list
        self.rethink_timer = random.uniform(0.2, 0.8)
```

**Fragment 2: Greedy pathfinding**

```python
    def _step(self, game) -> None:
        if not self.plan:
            self.plan = self._choose_path(game)
        for order, direction in enumerate(self.plan):
            if self.try_move(direction, game):
                if order:
                    self.plan.insert(0, self.plan.pop(order))
                return
        # All four directions blocked: turn around
        self.direction = OPPOSITE[self.direction]
        self.plan = [self.direction]

    def _choose_path(self, game) -> list[str]:
        """Return directions ordered from most to least useful."""
        if random.random() < 0.5:
            target_row, target_col = game.base_position
        else:
            target_row, target_col = game.player.position
        dr = target_row - self.row
        dc = target_col - self.col
        horizontal = "right" if dc > 0 else "left"
        vertical = "down" if dr > 0 else "up"
        ...
        rest = [d for d in DELTAS if d not in preferred]
        random.shuffle(rest)
        return preferred + rest
```

**Analysis**

1. **Data-driven again**: `Enemy` doesn't know "fast enemy speed is 1.8"; it just reads `ENEMY_TYPES`. To add a "heavy enemy", just change the table (8D.2 exercise 2).

2. **AI uses a "plan list" rather than single-step decisions**: `_choose_path` returns a priority-ordered direction list, e.g. `["right", "down", "left", "up"]`. Each step tries in order; on success, moves that direction to the front. This prevents oscillating between two directions and naturally gives "wall-hit rerouting".

3. **Random target selection**: 50% chance to chase the base, 50% to chase the player. Pure player-chasing becomes "hide and seek", pure base-chasing is too mechanical. The random mix varies the situation. Simplest "weighted decision".

4. **Symmetry with the player**: both subclasses inherit `Tank` and implement their own `update(dt, game)`. From `game.py`'s view, it calls `player.update(...)` then loops `enemy.update(...)` – **polymorphism**: different objects, same call style.

5. **Why AI isn't written in game.py**: AI keeps evolving (cover avoidance, coordinated attacks). Isolating it in `enemy.py` means breaking it only affects enemies. `game.py` only needs to shout `enemy.update(dt, self)` each frame.

---

### 8B.2.6 bullet.py and powerup.py: The Value of Small Modules

**Responsibility**: minimal data entities.

**powerup.py in full** (only 18 lines, the smallest module in the project):

```python
"""Collectible power-ups dropped by destroyed enemies."""

from config import POWERUP_LIFETIME


class PowerUp:
    """An item the player can collect by driving over it."""

    def __init__(self, kind: str, row: int, col: int) -> None:
        self.kind = kind
        self.row = row
        self.col = col
        self.ttl = POWERUP_LIFETIME

    @property
    def position(self) -> tuple[int, int]:
        """Return (row, col)."""
        return self.row, self.col
```

**bullet.py key fields**:

```python
class Bullet:
    def __init__(self, row: int, col: int, direction: str, owner,
                 speed: float = 1.0) -> None:
        self.row = row
        self.col = col
        self.direction = direction
        self.owner = owner
        self.faction = owner.faction          # "player" / "enemy"
        self.breaks_steel = owner.breaks_steel
        self.interval = BULLET_MOVE_INTERVAL / speed
        self.accum = 0.0                      # flight timer
        self.alive = True
```

**Analysis**

1. **Small files aren't worthless**. `powerup.py` is only 18 lines, but its existence lets `game.py` write `from powerup import PowerUp`, giving "what a power-up is" a single definition. If it were stuffed into `game.py`, reusing the "power-up" concept later would mean digging it out of a large file.

2. **`owner` back-reference**: bullets remember "who fired me", so `bullet_speed`, `breaks_steel`, `faction` can all be taken from the firer. Benefit: single logic; cost: the bullet holds a reference to the tank (if the tank is deleted but the bullet still flies, Python won't release the tank object – in this project dead tanks are cleared with their bullets, so it's fine).

3. **`position` property unifies the "entity" interface**. Tanks, bullets, power-ups all have `.position`, so `game.py` can write `tank.position == bullet.position` without caring about the class. This is duck typing in action.

4. **Why the "flight timer" lives on the bullet**: each bullet's speed can differ (firepower level, enemy type), and each `interval` and `accum` is independent. This is "put state in the object that needs it most".

---

### 8B.2.7 game.py: The Brain and Physics Engine

**Responsibility**: maintain the whole game world: map, entity lists, state machine, collision rules, spawning, scoring, saves.

**Fragment 1: State machine dispatch (excerpt)**

```python
    def update(self, dt: float) -> None:
        """Advance the whole game by *dt* seconds."""
        self.time += dt
        ...
        if self.state == STATE_PAUSED:
            return

        self._update_effects(dt)

        if self.state == STATE_READY:
            self.timer -= dt
            if self.timer <= 0:
                self.state = STATE_PLAYING
                self.set_message("开始！", "bright_white", 1.0)
        elif self.state == STATE_STAGE_CLEAR:
            self.timer -= dt
            if self.timer <= 0:
                self.stage += 1
                self._start_stage()
        elif self.state == STATE_PLAYING:
            self._update_playing(dt)
```

**Fragment 2: Fixed order per frame**

```python
    def _update_playing(self, dt: float) -> None:
        if self.freeze_time > 0:
            self.freeze_time = max(0.0, self.freeze_time - dt)

        self.player.update(dt, self)
        if self.state != STATE_PLAYING:
            return

        self._update_spawning(dt)
        for enemy in self.enemies:
            if enemy.alive:
                enemy.update(dt, self)
        if self.state != STATE_PLAYING:
            return

        self._update_bullets(dt)
        if self.state != STATE_PLAYING:
            return

        self._update_powerups(dt)
        self._remove_dead()
        self._update_extra_life()
        self._check_stage_clear()
```

**Fragment 3: Bullets step tile by tile**

```python
    def _step_bullet(self, bullet: Bullet) -> None:
        dr, dc = DELTAS[bullet.direction]
        bullet.row += dr
        bullet.col += dc
        if not in_bounds(bullet.row, bullet.col):
            bullet.alive = False
            return

        tile = self.grid[bullet.row][bullet.col]
        if tile == BRICK:
            self.grid[bullet.row][bullet.col] = FLOOR
            self._add_effect(bullet.row, bullet.col, "碎", "yellow", 0.12)
            bullet.alive = False
            return
        if tile == STEEL:
            if bullet.breaks_steel:
                self.grid[bullet.row][bullet.col] = FLOOR
            bullet.alive = False
            return
        if tile == BASE:
            bullet.alive = False
            self._destroy_base(bullet.faction == "player")
            return
        # Water, forest, floor, base rubble don't block bullets; next check tanks...
```

**Fragment 4: Bullet update loop**

```python
    def _update_bullets(self, dt: float) -> None:
        for bullet in self.bullets:
            if not bullet.alive:
                continue
            bullet.accum += dt
            while bullet.alive and bullet.accum >= bullet.interval:
                bullet.accum -= bullet.interval
                self._step_bullet(bullet)
        self._resolve_bullet_duels()
```

**Fragment 5: Uniform corpse cleanup**

```python
    def _remove_dead(self) -> None:
        self.enemies = [e for e in self.enemies if e.alive]
        self.bullets = [b for b in self.bullets if b.alive]
```

**Fragment 6: Difficulty curve (excerpt)**

```python
    def _pick_enemy_type(self) -> str:
        weights = {
            "basic": max(1, 8 - self.stage),
            "fast": 2 + self.stage,
            "power": 1 + self.stage // 2,
            "armor": max(0, self.stage - 1),
        }
        names = [name for name, weight in weights.items() if weight > 0]
        chances = [weights[name] for name in names]
        return random.choices(names, weights=chances, k=1)[0]
```

**Fragment 7: High-score persistence**

```python
def load_high_score() -> int:
    """Read the saved high score, ignoring a missing or broken file."""
    try:
        return int(HIGH_SCORE_FILE.read_text(encoding="utf-8").strip())
    except (OSError, ValueError):
        return 0


def save_high_score(score: int) -> None:
    """Best-effort persistence of the high score."""
    try:
        HIGH_SCORE_FILE.write_text(f"{score}\n", encoding="utf-8")
    except OSError:
        pass
```

**Analysis**

1. **Game is the "world"; entities keep only themselves**. A bullet doesn't know "what to do when it hits something"; it has only position and direction. All resolution happens in `game._step_bullet`. Benefit: rules centralised, easy to balance and debug; entity classes stay simple.

2. **State machine uses string constants** (`STATE_READY`, etc., defined in `config.py`). State decides which code runs this frame and what text `display.py` overlays (ready/paused/over).

3. **Update order is designed**:
   - Player moves first → player can still fire before "bullet resolution";
   - Then spawn, enemies move → enemies also fire this frame;
   - Finally update bullets together → everyone's bullets fly together this frame;
   - This makes interactions fair and avoids visual mismatch from "bullets fly first, enemies move later".

4. **After each step, check `state != STATE_PLAYING` and return**. Because player fire can destroy the base in `player.update()` and end the game; the rest of the logic shouldn't run.

5. **Bullets advance one tile at a time, not in big jumps**. If bullet speed is 2 tiles/frame and it jumps from `col=2` to `col=4`, the middle tile's tank/brick is missed ("through-wall bug"). `_update_bullets` uses `while` to advance one tile and resolve per tile, so even fast bullets can't pass through.

6. **Bullets take a step immediately on "spawn"** (`spawn_bullet` calls `_step_bullet`). This is "muzzle resolution": if the muzzle faces a brick, the bullet should break it immediately instead of waiting a frame inside the tank's tile.

7. **Dead entities cleared with list comprehensions**, not manual `remove` at each destruction point. Manual remove misses cases and can mutate a list while iterating (8C.13 covers this pitfall).

8. **Difficulty curve is a formula, not a config**: enemy count, spawn interval, type weights are all functions of `stage`. Tuning the formula tunes the entire curve, without hardcoding a table per stage.

9. **Save failure must not crash the game**: reading may hit "file doesn't exist" (first play) or "content hand-edited"; writing may hit "read-only directory". `try/except` swallows these two kinds of surprises separately, returning a default or failing silently – for a game, losing a high score is far better than crashing.

---

### 8B.2.8 display.py: Translating Data into "Pixels"

**Responsibility**: read-only game data, produce the terminal screen; handle Chinese width, colours, differential refresh.

**Fragment 1: Display width and padding**

```python
def display_width(text: str) -> int:
    """Return how many terminal columns *text* occupies."""
    return sum(
        2 if unicodedata.east_asian_width(ch) in ("W", "F") else 1
        for ch in text
    )


def pad(text: str, width: int = CELL_WIDTH) -> str:
    """Right-pad *text* with spaces so it fills *width* columns."""
    missing = width - display_width(text)
    if missing > 0:
        return text + " " * missing
    return text
```

**Fragment 2: Differential refresh**

```python
    def draw(self, lines: list[str]) -> None:
        """Write *lines* at the top of the screen, touching only changes."""
        size = shutil.get_terminal_size((80, 24))
        if (size.columns, size.lines) != self._size:
            self._size = (size.columns, size.lines)
            self._lines = []          # Window size changed, force full redraw
        out = []
        for index in range(max(len(lines), len(self._lines))):
            new_line = lines[index] if index < len(lines) else ""
            old_line = self._lines[index] if index < len(self._lines) else None
            if new_line != old_line:
                out.append(f"\033[{index + 1};1H{new_line}\033[K")
        if out:
            sys.stdout.write("".join(out))
            sys.stdout.flush()
        self._lines = list(lines)
```

**Fragment 3: HUD**

```python
def _hud_lines(game) -> list[str]:
    shield = "  护盾" if game.player.shield_time > 0 else ""
    hud = (
        f"{colorize('分数', 'bright_white')} {game.score:06d}  "
        f"{colorize('最高', 'bright_white')} {game.high_score:06d}  "
        f"{colorize('关卡', 'bright_white')} {game.stage}  "
        f"{colorize('生命', 'bright_white')} {game.lives}  "
        ...
    )
```

**Analysis**

1. **Three tasks**: build a character canvas (`board` 2D list) → colour → output. Render order is "terrain → power-ups → bullets → enemies → player → explosion effects", later overwriting earlier; an enemy on a forest tile is **skipped** (forest hides enemies). These rules live entirely in `render_frame`; game logic knows nothing.

2. **Chinese takes two columns**. In the terminal, "砖" occupies 2 columns and "R" occupies 1. If a line mixes 1-column and 2-column characters, the table misaligns. Solution:
   - Render each logical cell at exactly 3 columns (`CELL_WIDTH = 3`): one Chinese char = 2 columns + 1 space; 3 ASCII chars = exactly 3 columns;
   - `display_width()` uses `unicodedata.east_asian_width()` to classify wide (W/F) or narrow; `pad()` fills the gap.
   - HUD doesn't participate in cell alignment; `\033[K` clears the rest of the line.

3. **Differential refresh**. Clearing and redrawing 19 lines every frame makes the terminal flicker and emits lots of output. `Renderer` caches every line of the previous frame and **writes only changed lines**:
   - `\033[{row};1H` moves the cursor to row `row`, column 1;
   - Writes the new line;
   - `\033[K` clears everything after the cursor.
   So static walls are never rewritten; only moving tanks/bullets and the HUD change. 60 FPS is therefore effortless.

4. **Window size change forces full redraw**. Compare `self._size` with the current terminal size; if different, clear the cache – otherwise some unchanged lines never redraw and the screen garbles.

5. **Alternate screen and cursor**. `Renderer.start()` sends `\033[?1049h` to switch to the terminal's "alternate screen", and `\033[?1049l` on exit to switch back – the player's terminal history isn't flooded by the game. Hiding the cursor (`\033[?25l`) prevents flicker; show it again on exit.

6. **`display.py` does not import `game.py`**. `render_frame(game)` only requires the passed object to have `grid`, `player`, `enemies`, `bullets`, `state`, etc. Testing rendering can build a "fake game" without initialising the whole thing.

---

### 8B.2.9 input_handler.py: Dealing with the Terminal

**Responsibility**: on any platform, non-blockingly get "which keys the player pressed" and translate them into action names (`"up"`, `"fire"`, `"pause"`, …).

**Fragment 1: Pure-function parsing**

```python
def parse_keys(buffer: str, now: float, last_data: float) -> tuple[list[str], str]:
    """Split *buffer* into key names, keeping incomplete escapes buffered."""
    keys: list[str] = []
    index = 0
    while index < len(buffer):
        char = buffer[index]
        if char == "\x1b":
            rest = buffer[index:]
            if rest.startswith("\x1b["):
                if len(rest) < 3:
                    break                      # Escape sequence incomplete, wait
                code = rest[2]
                if code in _ARROW_CODES:
                    keys.append(_ARROW_CODES[code])
                    index += 3
                    continue
                ...
            if index == len(buffer) - 1:
                if now - last_data > _ESCAPE_GRACE:
                    keys.append("pause")
                    index += 1
                break
            index += 1
            continue
        action = _KEY_CHARS.get(char.lower())
        if action:
            keys.append(action)
        index += 1
    return keys, buffer[index:]
```

**Fragment 2: Context manager enters raw mode**

```python
class InputHandler:
    """Puts the terminal in raw mode and polls it without blocking."""

    def __enter__(self) -> "InputHandler":
        self._buffer = ""
        self._last_data = time.monotonic()
        if os.name == "nt":
            import msvcrt
            self._msvcrt = msvcrt
        else:
            import termios
            import tty
            self._termios = termios
            self._fd = sys.stdin.fileno()
            self._old_settings = termios.tcgetattr(self._fd)
            tty.setraw(self._fd)
        return self

    def __exit__(self, exc_type, exc, traceback) -> bool:
        if os.name != "nt":
            self._termios.tcsetattr(self._fd, self._termios.TCSADRAIN,
                                    self._old_settings)
        return False
```

**Analysis**

1. **Platform differences are fully isolated**. Unix uses `termios` + `tty` to enter raw mode, Windows uses `msvcrt`; other modules only know "`read_keys()` returns a list of action names".

2. **Raw mode**: the default terminal is "line buffered" – you press a key, but the program only receives it after you press Enter. Raw mode makes each keypress immediately visible, at the cost of **having to restore terminal settings yourself** (otherwise the user's terminal looks odd after exit). This is the duty of `__enter__` / `__exit__`, and the reason the `with` statement exists (8C.7).

3. **Non-blocking polling**: Unix uses `select.select(..., timeout=0)` to ask "is there data now" and only `os.read` if there is; Windows uses `msvcrt.kbhit()`. This way the game loop never blocks waiting for a key.

4. **Arrow keys are escape sequences**. Pressing ↑ once, the terminal actually sends the character sequence `\x1b[A` (ESC + `[` + `A`). And this sequence **may arrive in two pieces** (e.g., network terminal, slow serial). `input_handler` uses a `buffer` to keep "the unparsed tail":
   - Receives `\x1b[`: fewer than 3 chars, keep it;
   - Next receives `A`: concatenate to `\x1b[A`, parse as `"up"`.

5. **A lone ESC key is ambiguous**. Pressing Esc sends one `\x1b`; but the first byte of ↑ is also `\x1b`. The program can't decide immediately. The strategy here: if `\x1b` is the last character in the buffer and more than `_ESCAPE_GRACE` (0.12 s) has passed since the last data, treat it as "user pressed Esc" (mapped to pause); otherwise keep waiting.

6. **`parse_keys` is written as a pure function**: input "buffer string + two timestamps", output "key list + remaining buffer", touching no global state. So it can be **tested without starting the game** (8D.2 exercise 7). A typical move from "it runs" to "it's maintainable".

7. **Ctrl+C becomes ordinary bytes**. Raw mode disables terminal signal handling, so Ctrl+C doesn't raise `KeyboardInterrupt`; it sends byte `\x03`. The program maps it to `"quit"`.

---

### 8B.2.10 main.py: The Thin Entry

**Responsibility**: check terminal, create objects, run main loop, restore scene before exit.

**Main loop (excerpt)**:

```python
    try:
        with InputHandler() as keys:
            renderer.start()
            try:
                last = time.perf_counter()
                accumulator = 0.0
                next_frame = last
                while not game.quit:
                    now = time.perf_counter()
                    for key in keys.read_keys():
                        game.handle_input(key)

                    # Fixed-step simulation, independent of render speed.
                    accumulator += min(now - last, 0.25)
                    last = now
                    while accumulator >= LOGIC_STEP:
                        game.update(LOGIC_STEP)
                        accumulator -= LOGIC_STEP

                    if now >= next_frame:
                        size = shutil.get_terminal_size((80, 24))
                        if (size.columns < MIN_COLS
                                or size.lines < MIN_LINES):
                            renderer.draw([_too_small_message()])
                        else:
                            renderer.draw(render_frame(game))
                        next_frame = now + FRAME_TIME

                    time.sleep(INPUT_POLL_SLEEP)
            finally:
                renderer.stop()
    except KeyboardInterrupt:
        pass
```

**Analysis**

1. **Entry should be thin**. `main.py` has not a single game rule: it doesn't know how bullets fly or how enemies think. It only does "assemble + loop + cleanup". Later, if you want a Python network version, only `main` and `display` need changing; `game.py` is reusable as-is.

2. **Three time scales in one loop**:
   - `keys.read_keys()` every iteration (~500 Hz);
   - Logic advances by fixed steps `LOGIC_STEP` (`while accumulator >= LOGIC_STEP`);
   - Rendering is throttled by `FRAME_TIME` (`if now >= next_frame`).
   This is the "fixed step + frame-rate decoupling" 8C.19 will expand on.

3. **`min(now - last, 0.25)` is a fuse**. If the computer hangs for 10 s (e.g., system sleep), the difference is 10, the accumulator catches up 1200 logic steps at once, the game "fast-forwards", and it may get laggier. Capping a single delta at 0.25 s trades a little time precision for stability.

4. **`try/finally` guarantees cleanup**. Even if the loop raises (or the user Ctrl+C), `renderer.stop()` runs, the cursor restores, the alternate screen exits. The outer `main()` also catches `KeyboardInterrupt` to avoid an ugly traceback.

5. **Print the score after exit**. `game.save_high_score()` is called after the finally, when the terminal has returned to normal, so `print` content stays on the user's normal screen.

---

### 8B.2.11 README.md and highscore.txt: Data Files and Docs Are Part of the Project

- `README.md`: write clearly "how to run, which keys, what the legend means, what each module does". Others read it first.
- `highscore.txt`: a single line of digits automatically generated at runtime. It **shouldn't** be committed to version control; you can `.gitignore` it.
- These two files remind us: a project isn't just `.py`. Docs and runtime data also need planning.

---

### 8B.2.12 Summary: How Module Boundaries Are Decided

Looking back over this tour, the criteria for dividing modules are actually just three:

1. **Data vs. rules**: map data (battlefield) and configuration data (config) are separated from game rules (game);
2. **Common vs. specific**: the tank's common parts (tank) are separated from player/enemy logic (player/enemy);
3. **Core vs. periphery**: game rules (game) are separated from input/output (input_handler/display/main).

Each corresponds to a concrete change scenario: change map, add enemy, change terminal. **Module boundaries should be drawn along the points most likely to change.**

---

The project itself is now reviewed. You've probably noticed: it runs not only because of these modules, but also because of a batch of syntax and standard library features not covered in the first seven sections – the next section (8C) fills them in one by one.