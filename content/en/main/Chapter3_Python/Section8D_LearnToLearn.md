# Section 8D: Reading Methods and Extension Exercises

The previous sections walked through the project and filled in the syntax. This section is about "method": when you're handed a completely unfamiliar multi-file project, where should you start; then we'll use 8 exercises to truly make TankBattle your own project.

---

## 8D.1 How to Read a Multi-File Project

Reading code is as important as writing it. When given an unfamiliar multi-file project, don't start grinding from the first line of the first file. Here's a method, demonstrated with TankBattle.

### 8D.1.1 Step 1: Read the Docs and the Entry Point

1. Read `README.md` first: know what it is, how to run it, which keys to press;
2. Then read `main.py`: this is the entry point, and also the **master table of contents for call relationships**. Whoever it imports tells you which pieces the game is assembled from.

`main.py` is only 100 lines; after reading it you know: input comes from `InputHandler`, logic lives in `Game`, and the screen is drawn by `render_frame` + `Renderer`.

### 8D.1.2 Step 2: Draw the Dependency Graph

Copy down the `import` statements from every file, and you have a dependency graph (the one in 8B.1.3 was drawn exactly this way). After drawing it you'll naturally notice:

- `config.py` is depended on by everyone, and depends on no one;
- `game.py` is the only module that depends on all entities at once;
- No module imports `main.py` or the "upper layers" of `display.py`.

The dependency graph answers "changing this file affects whom", which is extremely useful during maintenance.

### 8D.1.3 Step 3: Trace a Call Chain

Pick a feature you're most interested in, and trace it from keypress to result. **Tracing call chains is the most efficient way to read code**, because it only walks the path you care about, without getting lost in unrelated code.

**Exercise 1: What happens after pressing `J`?**

```
1. main.py main loop
   keys.read_keys() returns ["fire"]
2. game.handle_input("fire")
   → self.player.handle_key("fire")
   → player.fire_held = True (just records intent)
3. Next game.update(1/120)
   → _update_playing → player.update(dt, self)
   → if self.fire_held and self.can_fire(game): self.shoot(game)
4. Tank.shoot(game) → game.spawn_bullet(self)
5. Game.spawn_bullet
   → bullet = Bullet(...)  created instantly
   → tank.cooldown = tank.shoot_cooldown
   → self._step_bullet(bullet)   muzzle resolves one tile immediately
6. Every subsequent frame _update_bullets
   → bullet.accum accumulates past bullet.interval
   → _step_bullet(bullet)
   → check out-of-bounds → check terrain (brick/steel/base) → check enemy tanks
7. Hits an enemy
   → enemy.take_damage()
   → if dead: _kill_enemy(enemy) → add score, explosion effect, maybe drop power-up
8. display.render_frame(game)
   → bullet and enemy positions are drawn
```

This path goes through `main.py → game.py → player.py → tank.py → bullet.py → game.py → display.py`. You only read the relevant functions, yet you connected the whole line of "input → logic → render".

**Exercise 2: When enemies are "frozen", who stops them?**

```
1. Player collects a clock power-up
   → game._collect(powerup) → self.freeze_time = FREEZE_TIME
2. Next frame enemy.update(dt, self) first line
   → if game.freeze_time > 0: return
   → returns immediately: no move, no replan, no fire
3. Start of _update_playing
   → self.freeze_time -= dt   (freeze time decreases each frame)
4. display has no idea about "freeze" at all
   → enemies just look like they're "standing still"
5. Player collects power-up, HUD shows a message
   → game.set_message("Enemies frozen!", ...) → display._hud_lines reads message
```

Conclusion: **one effect can span multiple modules**: data in `config` (`FREEZE_TIME`), state in `game`, behaviour interception in `enemy`, prompt in `display`. Tracing it once lets you understand how the effect is implemented "non-invasively".

### 8D.1.4 Step 4: Trace a Piece of Data

Pick a piece of data and watch its life. For example `grid` (the map):

| Stage | Where | What happens |
|-------|-------|--------------|
| Birth | `battlefield.create_battlefield` | Generates the 2D list |
| Use | `game.can_tank_enter` | Checks whether a tank can enter |
| Modify | `game._step_bullet` | Bullet changes brick to `FLOOR` |
| Change | `game._destroy_base` | Base becomes `BASE_RUBBLE` |
| Consume | `display._build_board` | Translates to coloured strings |
| Rebirth | `game._start_stage` | Regenerated after clearing a stage |

Another example, `score`: increased in `_kill_enemy`, given bonus in `_check_stage_clear`, triggers extra life in `_update_extra_life`, displayed in `_hud_lines`, saved in `_game_over`.

**A piece of data's life reveals module responsibilities better than a call chain.**

### 8D.1.5 Step 5: Change a Constant to Verify Understanding

Whether you understood it or not, changing something tells you:

- Change `PLAYER_MOVE_INTERVAL` from `0.11` to `0.5`: the player crawls like a snail – proves movement speed is indeed controlled by it;
- Change `TERRAIN_GLYPHS[BRICK]` from `"砖"` to `"#"`: the screen changes instantly – proves rendering depends only on config;
- Change `MAX_ENEMIES_ON_FIELD` to `1`: there's always only one enemy on the field – proves spawning is limited by it.

If changing it does nothing, you looked in the wrong place; if changing it crashes, congratulations – you found a hidden dependency. Both outcomes are more rewarding than "reading it once".

### 8D.1.6 Quiz

1. Can `display.py` directly do `from game import Game`? Why?
2. If `tank.py` were changed to `from game import Game`, what would happen?
3. If `config.py` contained a `count = 0`, and both `game.py` and `enemy.py` did `count += 1`, what would happen?

<details>
<summary>Reference answers</summary>

1. Technically yes, but unnecessary and it violates layering. `display` only needs "an object with those attributes", not the concrete class; not importing `game` also lets `display` be tested and reused independently.
2. It might form a circular import: `game.py` imports `tank.py`, and `tank.py` imports `game.py`. Moreover, `tank.py` would go from "pure data + behaviour" to depending on the whole world, unable to be tested in isolation. The project solves this by "passing `game` in as a parameter".
3. The two modules reference the **same** `config.count` object, and `count += 1` is not atomic (read then write). Single-threaded it barely works, but shared state means any change anywhere can affect everything else – exactly why "`config.py` should hold constants only".

</details>

---

## 8D.2 Extension Exercises

The following 8 exercises cover "change data → change logic → add module → refactor → test"; do them in order. Each gives **files involved** and **key ideas**; you write the code yourself.

### Exercise 1: Adjust the Feel (Difficulty: ★)

Change the game to a "slow heavy artillery" style:

- Player moves slower, bullets slower, but enemies slower too;
- More enemies per stage;
- Screen at 30 FPS.

**Files involved**: `config.py`.
**Uses**: 8B.2.1's constant tables, 8C.19's time parameters.
**Verify**: Run it directly after the change and feel "values are code".

### Exercise 2: Add an Enemy Type (Difficulty: ★)

Add a "heavy enemy": `重`, 3 HP, slow movement (speed 0.5), slow bullet (0.8), 500 points on kill.

**Files involved**: `config.py` (add one entry to `ENEMY_TYPES`).
**Think**: Why does `enemy.py` not need changes? This is the meaning of data-driven design.
**Advanced**: Make heavy enemies appear only after stage 3 – change the weight table in `game._pick_enemy_type`, starting its weight at 0.

### Exercise 3: Add a Power-Up (Difficulty: ★★)

Add a "mine" power-up `雷`: when the player picks it up, a mine is placed at the current position; when an enemy steps on it, it explodes and destroys that enemy.

**Files involved**: `config.py` (`POWERUP_TYPES`, `POWERUP_WEIGHTS`), `game.py` (`_collect` branch, new mine list, check after enemy movement), `display.py` (draw the mine).
**Uses**: 8C.14's `random.choices`, 8C.13's copy-iteration.
**Hint**: First implement a simplified version "on pickup, immediately explode and destroy the nearest enemy", then consider placement and triggering.

### Exercise 4: Add a Terrain Type "Ice" (Difficulty: ★★)

Ice `冰`: tanks can pass, but on ice the move interval is halved (slipperier, faster), bullets fly over normally.

**Files involved**: `config.py` (new constants, `TERRAIN_GLYPHS/COLORS`), `battlefield.py` (generate ice), `tank.py` (read `game.grid[self.row][self.col]` to check if on ice, return effective move interval).
**Uses**: 8C.3's `@property` (you can add an `effective_move_interval` to `Tank`).
**Think**: Checking "on ice" needs map info, and `Tank` doesn't import `game` – so either pass `game` into `try_move` (it already has this parameter), or compute it in `game` and tell the tank. Which do you choose? Why?

### Exercise 5: Add a "Base Reinforcement" Power-Up (Difficulty: ★★★)

The classic tank game's "shovel": on pickup, the brick walls around the base become steel walls for 15 seconds, then revert to brick (if the steel walls haven't been destroyed).

**Files involved**: `config.py`, `game.py` (new `shovel_time` timer; `_collect` branch; per-frame timer check; a "rebuild shield" helper).
**Uses**: 8B.2.2's `_build_base` (extract that logic for reuse), 8C.6's `try/finally` idea (timer end must restore).
**Hint**: Note that "revert to brick" must handle "steel wall already destroyed by a max-firepower player" – you can't conjure walls out of nothing.

### Exercise 6: Split `game.py` (Difficulty: ★★★)

Split `game.py` into three files:

- `collision.py`: collision checks for `can_tank_enter`, `_step_bullet`;
- `spawning.py`: `_pick_enemy_type`, `_update_spawning`, `_enemy_total`;
- `game.py`: only the state machine and coordination.

**Files involved**: `game.py`, plus new `collision.py`, `spawning.py`.
**Uses**: 8A.2.9's dependency inversion (new modules shouldn't do `from game import Game`; instead take needed objects as parameters).
**Think**: What does the `import` relationship become after splitting? Any cycles? Which functions become harder to call? – This is the boundary of "over-splitting".

### Exercise 7: Write Tests for Pure Functions (Difficulty: ★★)

The next section systematically covers **unit testing** and **integration testing**. As a warm-up, write unit tests for these three functions:

1. `battlefield.create_battlefield(stage)`: border is all steel; base position is `BASE`; enemy spawn points are `FLOOR`; map dimensions correct.
2. `battlefield.in_bounds(row, col)`: four cases inside/outside the boundary.
3. `input_handler.parse_keys(buffer, now, last_data)`: arrow sequence `\x1b[A` parses to `"up"`; half sequences stay in the buffer; `"q"` parses to `"quit"`.

**Files involved**: new `tests/` or `test_xxx.py`.
**Uses**: 8B.2.9's "pure functions are easy to test", 8C.6's assertion failure (`assert` is essentially a statement "if not satisfied, raise an exception").

### Exercise 8: Swap the Graphics to Emoji / ASCII (Difficulty: ★)

- Emoji version: `砖→🧱`, `钢→⬜`, `我→🚀`, `敌→👾`…
- Pure ASCII version: `砖→#`, `钢→%`, `我→P`, `敌→E`.

**Files involved**: `config.py`'s `TERRAIN_GLYPHS` / `ENEMY_TYPES` / `PLAYER_CHAR`, possibly `CELL_WIDTH`.
**Uses**: 8B.2.8's width computation.
**Note**: Emoji are usually 2 columns in the terminal too, but some platforms render them as 1 or 3 columns, causing misalignment. Test on a small scale first, confirm terminal behaviour, then change everything. If switching to pure ASCII (all 1 column wide), you can change `CELL_WIDTH` from 3 to 1 for a tighter screen – but `pad()`, HUD, and banners all need to be revisited.

---

## 8D.3 What You've Learned in This Section

1. **Five problems of a single file**: slow to find code, dangerous to change, collaboration conflicts, hard to reuse, hard to test; splitting solves them, at the cost of "needing to plan boundaries in advance".
2. **Import mechanisms**: `import` / `from ... import ...` / `as`, multi-line imports, `__name__`, packages and directories, absolute/relative imports, modules loaded only once.
3. **Circular imports and their solutions**: push shared constants down, defer imports inside functions, dependency inversion (pass objects as parameters). TankBattle's dependency direction is "upper layers know lower layers, lower layers don't know upper layers".
4. **Architecture and layering**: constants layer → base data layer → entities layer → world rules layer → periphery layer → entry; responsibilities and dependency graph of each layer.
5. **Structure of a real-time game**: state machine, fixed timestep updates, three time scales of input/logic/render, per-tile bullet resolution, unified corpse cleanup.
6. **A batch of new syntax and standard library**: classes and inheritance, `@property`, complete exception usage, `with` context managers, advanced type annotations, conditional expressions, chained comparisons, format specifiers, `any/all/sum`, iterating copies, `random/time/pathlib/shutil/unicodedata`.
7. **Advanced terminal programming**: ANSI positioning and line clearing, alternate screen, double-width character alignment, non-blocking input, platform detection, signals and graceful exit.
8. **Two engineering patterns**: fixed timestep loop (speed independent of frame rate), frame differential rendering (draw only what changed).
9. **Methods for reading code**: read entry → draw dependency graph → trace call chain → trace data → change constants to verify.

The next section (8E, optional, not required) will cover how to write tests for this multi-file project: first use **unit tests** to verify functions inside each module (like `parse_keys`, `create_battlefield`, `display_width`), then use **integration tests** to verify collaboration between modules (like a whole game from spawning to hitting, from losing HP to clearing a stage) – by then you'll see that these functions were designed as pure functions precisely to make testing easy.
