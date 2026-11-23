# Section 8E: Unit Testing and Integration Testing

> **This section is not required.** You can skip it entirely: as long as the program runs and you can modify it, the goal of this stage is already met. Testing is a tool you'll find useful once you start to *fear breaking old features*. If you don't understand it now, just knowing it exists is fine; come back when you need it.

At the end of 8D, we left a question: after writing and understanding the code, how do you make sure your next change hasn't broken something that used to work?

So far, your verification method has been **manual testing**: change a line, open the game, press some keys, see if it works. That's fine when the program is small. But TankBattle has 11 modules and 1600 lines; manual testing gets more tiring and less reliable over time.

This section covers two things:

- **Unit testing**: checking a single function or class in isolation – e.g., "`in_bounds(-1, 0)` must return `False`";
- **Integration testing**: assembling multiple modules and checking them together – e.g., "player fires, enemy should die, score should increase by 100".

We'll use the three functions from Exercise 7 in 8D, plus full TankBattle scenarios. Everything is within the standard library – no installations required.

---

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Test | A piece of code that automatically checks a program's behaviour |
| Test case | A specific check scenario, e.g., "a negative coordinate should be out of bounds" |
| Assertion | A statement that says "this should be true here"; it raises an error if not |
| Unit test | Tests a single function/class, avoiding other modules as much as possible |
| Integration test | Tests several modules assembled together |
| Regression | Something that used to work but broke after a change |
| Test double | A fake object used to replace a real dependency (called "fake" in this project) |
| Headless run | Running logic in memory without a terminal or keypresses |
| Smoke test | A coarse test that only checks "it runs and doesn't crash" |
| Fuzz test | Random input thrown at the program to check it stays in a legal state |
| Coverage | The proportion of code executed by tests relative to total code |
| TDD | Test-Driven Development: write tests first, then implementation (for awareness only) |

---

## 8E.1 Why You Can Skip It, and Why It's Still Worth a Look

### 8E.1.1 How Tiring Manual Testing Gets

Suppose you want to change the player's movement speed from `0.11` to `0.13`. The manual testing process is roughly:

1. Launch the game, wait for the 1.8-second "ready" to finish;
2. Hold a direction key and feel the speed;
3. Fire a shot, confirm bullets still hit enemies;
4. Quit, change back to `0.11` and feel it again;
5. The next day you change enemy AI, and you have to redo all of the above.

Change one number, verify for five minutes; change ten things, verify for an evening. And people get lazy: you'll probably only test the part you just changed, and you won't run through the base, power-ups, pause, and stage-clearing every time – **that's how old features quietly break**.

### 8E.1.2 Two Real Stories

During the development of TankBattle, two bugs were especially typical:

**Story 1: The program crashes when a bullet hits the base**

The code in `game.py` that checks "did a bullet hit the base" uses a constant `BASE`, which must be imported from `config.py`. During one cleanup of import statements, `BASE` was accidentally deleted. This bug is completely invisible during normal play – until an enemy bullet pierces through layers of brick walls and hits the eagle, at which point the program throws a `NameError`.

**Story 2: The hidden code path for power-up drops**

When dropping a power-up, the code needs to pick a walkable tile. The code at the time wrote `p.position`, but the `PowerUp` class only has `row` and `col` – no `position` attribute. The trigger condition for this bug was even more demanding: you needed to **kill 4 enemies first** before a power-up could drop.

The common thread between these two bugs: **manual testing takes minutes or even several rounds to hit them, while tests expose them in milliseconds**. Story 1 corresponds to the integration test in 8E.5.4 of this section; Story 2 corresponds to 8E.5.6.

> Tests are not for "proving the program has no bugs" – they're for "letting bugs run into you".

### 8E.1.3 When It's Genuinely Fine Not to Write Tests

Tests also have a cost: test code must be maintained alongside the program; when interfaces change, tests must change too. For the following situations, not writing tests is reasonable:

- A one-off small script you'll throw away after running;
- A classroom demo where only the few minutes of demonstration matter;
- A prototype where the gameplay is still being explored and interfaces change three times a day;
- You're still learning syntax and writing tests would hurt your confidence.

The criterion is simple: **if you'll still be modifying this program many times, or handing it to someone else to use, testing starts paying off**. TankBattle happens to fall into this category – 11 modules, constantly gaining features – so it's worth writing tests for it.

---

## 8E.2 The Most Basic Test: assert

8C.6 mentioned in passing: `assert` is a statement that "raises an exception if not satisfied". It's the smallest testing tool:

```python
from battlefield import in_bounds

assert in_bounds(0, 0)          # top-left of map, must be True
assert not in_bounds(-1, 0)     # row -1, must be False
assert not in_bounds(0, 26)     # column out of bounds, must be False

print("all passed")
```

If all assertions hold, the program prints "all passed"; if one doesn't:

```
Traceback (most recent call last):
  File "demo.py", line 3, in <module>
    assert not in_bounds(-1, 0)
           ^^^^^^^^^^^^^^^^^^^
AssertionError
```

Two things to note:

- `assert` only works in "debug mode" – running with `python -O` ignores all of them, so **don't use assert for parameter validation** (don't use it to check user input);
- It's very convenient for writing tests, but raw assert has three shortcomings: the first failure stops everything, there's no pass/fail statistics, and you can't run a single check in isolation.

The next section's `unittest` solves all these problems.

---

## 8E.3 unittest: The Standard Library's Built-in Test Framework

### 8E.3.1 Your First Test File

```python
"""Unit tests for battlefield.in_bounds."""

import unittest

from battlefield import in_bounds


class TestInBounds(unittest.TestCase):
    """in_bounds() must accept the whole map and reject everything else."""

    def test_inside_is_true(self):
        self.assertTrue(in_bounds(0, 0))

    def test_outside_is_false(self):
        self.assertFalse(in_bounds(-1, 0))


if __name__ == "__main__":
    unittest.main()
```

Save this as `test_in_bounds.py` and run it in the TankBattle directory:

```bash
py -m unittest test_in_bounds -v
```

(On Linux / macOS / WSL, write `python3 -m unittest test_in_bounds -v`.)

Output:

```
test_inside_is_true (test_in_bounds.TestInBounds.test_inside_is_true) ... ok
test_outside_is_false (test_in_bounds.TestInBounds.test_outside_is_false) ... ok

----------------------------------------------------------------------
Ran 2 tests in 0.000s

OK
```

Three rules to remember:

1. Test classes must **inherit from `unittest.TestCase`**;
2. Test method names must **start with `test_`**, otherwise they are ignored;
3. Assertions use `self.assertXxx(...)` methods, not bare `assert` – on failure they print clearer messages and are easier for tools to tally.

### 8E.3.2 Common Assertion Methods

| Method | Meaning | Example |
|--------|---------|---------|
| `assertEqual(a, b)` | a equals b | `self.assertEqual(score, 100)` |
| `assertNotEqual(a, b)` | a not equal to b | |
| `assertTrue(x)` / `assertFalse(x)` | x is true / false | `self.assertTrue(player.alive)` |
| `assertIn(x, seq)` / `assertNotIn` | x in / not in sequence | `self.assertIn("up", keys)` |
| `assertIsNone(x)` / `assertIsNotNone(x)` | x is / is not None | |
| `assertAlmostEqual(a, b)` | floats approximately equal | `self.assertAlmostEqual(x, 0.11, places=2)` |
| `assertGreater(a, b)` / `assertLess` | size comparison | |
| `assertRaises(ExceptionType)` | "an exception should be raised here" | See 8E.4.3 |

### 8E.3.3 Running Tests

| Goal | Command (in the TankBattle directory) |
|------|---------------------------------------|
| Auto-discover and run all tests in current directory | `py -m unittest -v` |
| Run only one file | `py -m unittest test_battlefield -v` |
| Run only one class | `py -m unittest test_battlefield.TestInBounds -v` |
| Tests in a `tests/` subdirectory | Create an empty `tests/__init__.py` first, then `py -m unittest discover -s tests -v` |

**Why must you run from the project root?** Because the test file contains `from battlefield import in_bounds`. Python adds the "current directory" to the search path when looking up modules; running from another directory means it can't find `battlefield.py`.

### 8E.3.4 What a Failure Looks Like

Deliberately write a wrong assertion and run `py -m unittest tests.test_demo_fail -v`:

```
test_example (tests.test_demo_fail.TestDemo.test_example) ... FAIL

======================================================================
FAIL: test_example (tests.test_demo_fail.TestDemo.test_example)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "tests/test_demo_fail.py", line 8, in test_example
    self.assertEqual(2 + 2, 5)
    ~~~~~~~~~~~~~~~~^^^^^^^^^^
AssertionError: 4 != 5

----------------------------------------------------------------------
Ran 1 test in 0.003s

FAILED (failures=1)
```

How to read a failure message:

1. Find the line after `FAIL:` – it tells you **which test** failed;
2. Look at the last line of the traceback – it tells you **what the expected vs actual difference is** (here `4 != 5`);
3. Use `-v` to see each test's name; without `-v`, default mode shows a row of dots: `. = pass`, `F = fail`.

### 8E.3.5 Where to Put Them and How to Name Them

| Recommendation | Explanation |
|----------------|-------------|
| Filename `test_*.py` | So `unittest` can auto-discover |
| One test file per source file | `battlefield.py` → `test_battlefield.py` |
| Class name `TestXxx` | `TestInBounds`, `TestGeneration` |
| Method name `test_behaviour_expectation` | `test_blocked_move_only_turns` |
| One test verifies one thing | On failure you immediately know what broke |

Two directory layouts – pick either:

```
TankBattle/
├── battlefield.py
├── test_battlefield.py        # Option 1: tests alongside source, simplest
├── test_game.py
└── ...

TankBattle/
├── battlefield.py
├── tests/                     # Option 2: centralised, needs an extra __init__.py
│   ├── __init__.py
│   ├── test_battlefield.py
│   └── test_game.py
└── ...
```

---

## 8E.4 Unit Testing in Practice

### 8E.4.1 Pure Functions Are Easiest to Test: test_battlefield.py

The functions in `battlefield.py` are almost all pure functions: same map dimensions and coordinates always give the same result. These are the least troublesome to test.

Create `test_battlefield.py`:

```python
"""Unit tests for battlefield.py."""

import unittest

from battlefield import (
    base_position, create_battlefield, enemy_spawn_points, in_bounds,
    is_tank_passable, player_spawn,
)
from config import (
    BASE, BRICK, FIELD_HEIGHT, FIELD_WIDTH, FLOOR, FOREST, STEEL, WATER,
)


class TestInBounds(unittest.TestCase):
    """in_bounds() must accept the whole map and reject everything else."""

    def test_corners_and_edges(self):
        self.assertTrue(in_bounds(0, 0))
        self.assertTrue(in_bounds(FIELD_HEIGHT - 1, FIELD_WIDTH - 1))
        self.assertFalse(in_bounds(-1, 0))
        self.assertFalse(in_bounds(FIELD_HEIGHT, 0))
        self.assertFalse(in_bounds(0, -1))
        self.assertFalse(in_bounds(0, FIELD_WIDTH))


class TestTiles(unittest.TestCase):
    """Tanks may drive on floor and forest only."""

    def test_tank_passable_tiles(self):
        self.assertTrue(is_tank_passable(FLOOR))
        self.assertTrue(is_tank_passable(FOREST))
        self.assertFalse(is_tank_passable(BRICK))
        self.assertFalse(is_tank_passable(STEEL))
        self.assertFalse(is_tank_passable(WATER))


class TestGeneration(unittest.TestCase):
    """The generated map must always satisfy a few invariants."""

    def test_size_and_steel_border(self):
        grid = create_battlefield(1)
        self.assertEqual(len(grid), FIELD_HEIGHT)
        for row in grid:
            self.assertEqual(len(row), FIELD_WIDTH)
        for col in range(FIELD_WIDTH):
            self.assertEqual(grid[0][col], STEEL)
            self.assertEqual(grid[FIELD_HEIGHT - 1][col], STEEL)
        for row in range(FIELD_HEIGHT):
            self.assertEqual(grid[row][0], STEEL)
            self.assertEqual(grid[row][FIELD_WIDTH - 1], STEEL)

    def test_base_and_spawns_are_clear(self):
        for stage in (1, 3, 6):
            with self.subTest(stage=stage):
                grid = create_battlefield(stage)
                base_row, base_col = base_position()
                self.assertEqual(grid[base_row][base_col], BASE)
                for row, col in enemy_spawn_points():
                    self.assertEqual(grid[row][col], FLOOR)
                spawn_row, spawn_col = player_spawn()
                self.assertEqual(grid[spawn_row][spawn_col], FLOOR)

    def test_most_of_the_map_is_walkable(self):
        grid = create_battlefield(2)
        floor = sum(row.count(FLOOR) for row in grid)
        self.assertGreater(floor, FIELD_WIDTH * FIELD_HEIGHT // 2)


if __name__ == "__main__":
    unittest.main()
```

**Techniques worth learning from this test**

- **Boundary value testing**: `in_bounds` is tested at six positions – top-left, bottom-right, and out-of-bounds cases on all four edges. Boundaries are where mistakes are most likely.
- **`with self.subTest(stage=stage)`**: run multiple parameter sets within one test. If one set fails, the report shows which set, rather than a generic "this test failed".
- **Repeated `self.assertEqual`**: verbose to write, but on failure it prints "which line, what value, how far from expected" – far better than `assert`.

### 8E.4.2 Functions with Randomness: Test "Invariants"

`create_battlefield` generates a different map each round, so you can't write:

```python
# Wrong: the next map is different, so the test would be flaky
self.assertEqual(grid[3][4], BRICK)
```

What you must test is **things that must hold no matter what random map comes out** (the technical term is "invariants"):

- The map's dimensions are always correct;
- The outermost ring is always steel wall;
- The base is always at the `base_position()` cell;
- The three enemy spawn points are always open ground.

That's exactly what `test_base_and_spawns_are_clear` and `test_size_and_steel_border` above do. **Don't guess what randomness produced – test what it must preserve.**

### 8E.4.3 Isolating Dependencies with Fake Objects: test_player.py

`Tank.try_move(direction, game)` needs a `game` object to determine whether the target tile is walkable. In tests, you don't need to actually construct a `Game` – per the dependency inversion from 8A.2.9, `Tank` only requires the passed object to have a `can_tank_enter()` method. We write a minimal fake object:

Create `test_player.py`:

```python
"""Unit tests for Tank and Player, using a fake game object."""

import unittest

from player import Player
from tank import Tank


class FakeGame:
    """Stand-in for Game: Tank.try_move() only needs can_tank_enter()."""

    def __init__(self, blocked=()):
        self.blocked = set(blocked)

    def can_tank_enter(self, tank, row, col):
        return (row, col) not in self.blocked


class TestTank(unittest.TestCase):
    """Movement and damage rules of the base class."""

    def setUp(self):
        self.tank = Tank(5, 5, "T", "white", hp=2,
                         move_interval=0.1, faction="test")

    def test_move_changes_position_and_direction(self):
        self.assertTrue(self.tank.try_move("right", FakeGame()))
        self.assertEqual(self.tank.position, (5, 6))
        self.assertEqual(self.tank.direction, "right")

    def test_blocked_move_only_turns(self):
        game = FakeGame(blocked={(5, 6)})
        self.assertFalse(self.tank.try_move("right", game))
        self.assertEqual(self.tank.position, (5, 5))
        self.assertEqual(self.tank.direction, "right")

    def test_invalid_direction_raises(self):
        with self.assertRaises(KeyError):
            self.tank.try_move("diagonal", FakeGame())

    def test_damage_stops_at_zero(self):
        self.tank.take_damage(10)
        self.assertEqual(self.tank.hp, 0)
        self.assertFalse(self.tank.alive)


class TestPlayerStats(unittest.TestCase):
    """Fire power is computed from the star level."""

    def test_power_level_drives_bullet_stats(self):
        player = Player(1, 1)
        self.assertEqual(player.max_bullets, 1)
        self.assertFalse(player.breaks_steel)
        player.power = 2
        self.assertEqual(player.max_bullets, 2)
        player.power = 3
        self.assertTrue(player.breaks_steel)
        self.assertGreater(player.bullet_speed, 1.0)


if __name__ == "__main__":
    unittest.main()
```

**Techniques worth learning from this test**

- **`FakeGame` is the "test double" from 8C.6**: it doesn't implement the game's full functionality, only the one method that the code under test requires. This is the payoff from 8A's "pass `game` in as a parameter" – dependency inversion makes code more testable.
- **`setUp()` runs before each test method**. Here it creates a fresh tank so tests don't affect each other. Note: `setUp` is re-executed each time, so each test is independent.
- **`assertRaises`** tests "this **should** error":
  ```python
  with self.assertRaises(KeyError):
      self.tank.try_move("diagonal", FakeGame())
  ```
  A direction outside the four valid ones causes `DELTAS["diagonal"]` to raise `KeyError` – this is deliberately designed to "fail loudly", and the test pins that behaviour down.
- **Testing property linkage**: `Player`'s firepower properties are `@property` (8C.3); after changing `power`, `max_bullets` and `breaks_steel` should immediately follow. These "computed values" are especially worth protecting with tests.

### 8E.4.4 Protocol Parsing: test_input_handler.py

`parse_keys` was deliberately written as a pure function in 8B.2.9: input "buffer + two timestamps", output "action list + remaining buffer", touching no terminal.

Create `test_input_handler.py`:

```python
"""Unit tests for input_handler.parse_keys (no terminal needed)."""

import unittest

from input_handler import parse_keys


class TestParseKeys(unittest.TestCase):
    """parse_keys() is a pure function: text in, action names out."""

    def test_plain_keys_are_mapped(self):
        keys, rest = parse_keys("wjq", 0.0, 0.0)
        self.assertEqual(keys, ["up", "fire", "quit"])
        self.assertEqual(rest, "")

    def test_arrow_key_sequences(self):
        keys, rest = parse_keys("\x1b[A\x1b[B\x1b[C\x1b[D", 0.0, 0.0)
        self.assertEqual(keys, ["up", "down", "right", "left"])
        self.assertEqual(rest, "")

    def test_incomplete_escape_stays_in_buffer(self):
        keys, rest = parse_keys("\x1b[", 5.0, 5.0)
        self.assertEqual(keys, [])
        self.assertEqual(rest, "\x1b[")
        keys, rest = parse_keys(rest + "A", 5.0, 5.0)
        self.assertEqual(keys, ["up"])
        self.assertEqual(rest, "")

    def test_lone_escape_needs_a_grace_period(self):
        keys, rest = parse_keys("\x1b", 10.0, 9.99)
        self.assertEqual(keys, [])
        self.assertEqual(rest, "\x1b")
        keys, rest = parse_keys("\x1b", 10.2, 10.0)
        self.assertEqual(keys, ["pause"])
        self.assertEqual(rest, "")

    def test_ctrl_c_is_quit(self):
        keys, _ = parse_keys("\x03", 0.0, 0.0)
        self.assertEqual(keys, ["quit"])


if __name__ == "__main__":
    unittest.main()
```

**Why these tests are especially valuable**

Arrow key escape sequences may arrive split into two parts, a lone ESC and the ESC at the start of an arrow key can't be distinguished, and Ctrl+C in raw mode is just an ordinary byte – these situations are **nearly impossible to test manually** (you'd need to press exactly the right key at exactly the right moment), but writing a pure-function test is just a few lines of strings:

```python
keys, rest = parse_keys("\x1b[", 5.0, 5.0)      # half a sequence
self.assertEqual(keys, [])
self.assertEqual(rest, "\x1b[")                 # should stay in the buffer
```

### 8E.4.5 Three Disciplines of Unit Testing

1. **Each test is independent**: doesn't depend on another test running first, doesn't depend on residue from a previous run (this is also why `setUp` exists).
2. **No dependence on real time or randomness**: need time? pass `dt` manually. Need randomness? `random.seed(8)` to fix the seed.
3. **No dependence on terminal, network, or files**: need screen output? redirect it first. This project doesn't even need a terminal, because logic and display are separated (8B.1.5's layering pays off again here).

---

## 8E.5 Integration Testing: Assembling Modules

### 8E.5.1 Unit Tests vs Integration Tests

| | Unit test | Integration test |
|--|-----------|------------------|
| Subject | One function / one class | Multiple modules collaborating |
| Dependencies | Isolated with fake objects | Real classes |
| Speed | Very fast | Slightly slower (still fast in this project) |
| Failure localisation | Easy, small scope | Needs investigation, large scope |
| Examples | `in_bounds`, `parse_keys` | "fire → enemy dies → score increases" |
| Quantity ratio | Many | Few |

Rule of thumb: **write enough unit tests first, then add a few key integration tests**. All integration tests would be slow and hard to localise; all unit tests would leave nobody verifying "does it still work when assembled".

### 8E.5.2 Headless Starting a Game

The key to integration tests: `Game` can run without a terminal.

```python
from game import Game
from config import STATE_PLAYING

game = Game()
game.state = STATE_PLAYING
game.spawn_queue = ["basic"]      # control what enemy spawns next
game.freeze_time = 10.0           # freeze enemy AI to make the scene reproducible
```

- `Game()` generates the map, creates the player – all in memory;
- Directly modifying `state`, `spawn_queue`, `freeze_time` is "cheating on internal state in tests". This is allowed in tests: your goal is to **construct a scenario**, not to simulate a player clicking through menus;
- `game.update(1 / 120)` advances the logic one step, exactly like the main loop does – just without rendering or sleeping.

### 8E.5.3 Combat Scenario: Fire a Shot, Enemy Dies, Score Increases

Create `test_game.py`, start with a base class and the first scenario:

```python
"""Integration tests: several modules working together, no terminal."""

import random
import re
import unittest

from battlefield import base_position
from bullet import Bullet
from config import (
    BASE_RUBBLE, CELL_WIDTH, FIELD_HEIGHT, FIELD_WIDTH, FLOOR, MIN_COLS,
    STATE_GAME_OVER, STATE_PLAYING, STATE_READY, STATE_STAGE_CLEAR,
)
from display import display_width, render_frame
from enemy import Enemy
from game import Game
from powerup import PowerUp

ANSI = re.compile(r"\033\[[0-9;?]*[A-Za-z]")


class GameTestCase(unittest.TestCase):
    """Base class that builds a deterministic, already-playing game."""

    def make_game(self, queue):
        game = Game()
        game.state = STATE_PLAYING
        game.spawn_queue = list(queue)
        game.freeze_time = 10.0        # keep enemies still and quiet
        return game


class TestCombat(GameTestCase):
    """Rules that need player, bullet, enemy and game together."""

    def test_player_bullet_kills_enemy_and_scores(self):
        game = self.make_game(["basic"])
        for col in range(3, 7):        # clear the firing line
            game.grid[5][col] = FLOOR
        game.enemies = [Enemy(5, 5, "basic")]
        game.player.row, game.player.col = 5, 3
        game.player.direction = "right"
        game.spawn_bullet(game.player)
        game.update(0.05)
        self.assertEqual(game.enemies, [])
        self.assertEqual(game.score, 100)
```

**This test covers 5 modules**: `Enemy`, `Player`/`Tank`, `Bullet`, `Game`, and `battlefield` (via `base_position`). That's the value of integration testing: it verifies "does it work when assembled".

**Techniques worth learning**

- `for col in range(3, 7): game.grid[5][col] = FLOOR` – **clean the test environment to a deterministic state**. The map is randomly generated; without clearing the firing line, the bullet might hit a brick first and the test would be flaky (these "intermittent failures" are the most patience-draining).
- `game.update(0.05)` advances one step. `0.05` is larger than the bullet's movement interval (`0.04`), so the bullet definitely advances one step – **time in tests can be precisely controlled**.
- Assertions are direct: the enemy list is empty, the score is 100. If the program changes the kill score from 100 to 200, this test goes red immediately, telling you "behaviour changed".

### 8E.5.4 Base Destroyed: Game Over

Add to `TestCombat`:

```python
    def test_enemy_bullet_destroys_the_base(self):
        game = self.make_game([])
        base_row, base_col = base_position()
        enemy = Enemy(1, 1, "basic")
        game.enemies = [enemy]
        game.player.row, game.player.col = 1, 3
        bullet = Bullet(base_row, base_col - 1, "right", enemy)
        game.bullets = [bullet]
        game._step_bullet(bullet)
        self.assertEqual(game.state, STATE_GAME_OVER)
        self.assertEqual(game.grid[base_row][base_col], BASE_RUBBLE)
```

This test is the defence line for "Story 1" in 8E.1.2: **if `game.py` forgets to import `BASE`, a bullet hitting the base raises `NameError`, and this test catches it immediately** – no need to actually play to that moment.

Also note `game._step_bullet(bullet)` uses an underscore-prefixed internal method. Tests are occasionally allowed to "go around the back" and call internal methods directly, in exchange for more precise scenario control; it's a trade-off, not a mistake.

### 8E.5.5 Stage Clear: Empty the Enemies → Next Stage

```python
    def test_clearing_the_field_starts_the_next_stage(self):
        game = self.make_game([])
        game.enemies = []
        game.update(1 / 120)
        self.assertEqual(game.state, STATE_STAGE_CLEAR)
        game.update(3.0)
        self.assertEqual(game.stage, 2)
        self.assertEqual(game.state, STATE_READY)
```

It verifies the state machine transitions: `PLAYING → STAGE_CLEAR → (wait 2.2 s) → READY`. The state machine is the most complex piece of logic in 8B.2.7, and it's worth pinning down with an integration test.

### 8E.5.6 Power-up Drop: 4 Consecutive Kills

```python
    def test_powerup_drops_after_four_kills(self):
        game = self.make_game(["basic"])
        for _ in range(4):
            game._kill_enemy(Enemy(8, 8, "basic"))
        self.assertTrue(game.powerups)
```

This test corresponds to "Story 2": the power-up drop code path only triggers after 4 kills. With it, bugs like `PowerUp.position` (a non-existent attribute) are exposed immediately, rather than crashing when you reach stage 3.

### 8E.5.7 Smoke Test: Random Input for a Few Thousand Frames

The preceding tests are "pointed checks". There's another category that **checks nothing specific, only that the program doesn't crash and the world stays legal**:

```python
class TestSmoke(GameTestCase):
    """Fuzz the whole game and check the world stays legal."""

    def test_random_input_for_a_few_thousand_frames(self):
        random.seed(8)
        game = Game()
        actions = ["up", "down", "left", "right", "fire"]
        for _ in range(3000):
            if random.random() < 0.3:
                game.handle_input(random.choice(actions))
            game.update(1 / 60)
            for tank in list(game.tanks()):
                self.assertTrue(0 <= tank.row < FIELD_HEIGHT)
                self.assertTrue(0 <= tank.col < FIELD_WIDTH)
            for bullet in game.bullets:
                self.assertTrue(0 <= bullet.row < FIELD_HEIGHT)
                self.assertTrue(0 <= bullet.col < FIELD_WIDTH)
```

Key points:

- `random.seed(8)` fixes the random seed. This makes the test **reproducible**: the sequence of "random mashing" that failed today can be reproduced tomorrow;
- 3000 frames is about 50 seconds of game time, running in under a second on a computer;
- It checks "invariants": all tanks' and bullets' coordinates must be inside the map. This kind of test is also called a **fuzz test**, a great tool for finding "edge case crashes".

### 8E.5.8 Rendering Needs Testing Too: Every Line's Width Must Be Correct

The rendering module can also be tested headlessly, because `render_frame` is just "data in, strings out":

```python
class TestRendering(GameTestCase):
    """Rendering is data in, strings out; no real terminal required."""

    def test_every_board_line_is_exactly_one_screen_wide(self):
        game = self.make_game([])
        game.update(1 / 120)
        lines = render_frame(game)
        for line in lines[:FIELD_HEIGHT]:
            text = ANSI.sub("", line)
            self.assertEqual(display_width(text), FIELD_WIDTH * CELL_WIDTH)
        for line in lines:
            self.assertLessEqual(display_width(ANSI.sub("", line)), MIN_COLS)

    def test_unknown_powerup_kind_is_a_programming_error(self):
        game = self.make_game([])
        game.powerups.append(PowerUp("unknown", 1, 1))
        with self.assertRaises(KeyError):
            render_frame(game)
```

**Principle**: each line returned by `render_frame` mixes in ANSI colour codes; first strip them with a regex, then use `display_width()` to count the actual column count. Chinese characters take two columns, ASCII takes one – if any line's width is wrong, the terminal screen will misalign, and this test can catch it without opening a terminal.

The second test uses `assertRaises` to pin down "unknown power-up type must fail loudly". `render_frame` raises `KeyError` at `POWERUP_TYPES[kind]`, and that's exactly what we want: **errors should be caught early and be obvious**, not silently draw a blank cell.

### 8E.5.9 Notes on Integration Testing

- **Control the environment**: random maps, random enemies, real time are all instability factors; tests must pin them down (clear the firing line, fix the queue, fix `dt`, set the seed).
- **One test, one scenario**: don't stuff "shooting + stage clear + power-up" into one method; on failure you won't know which part broke.
- **Allow a little side effect**: `test_enemy_bullet_destroys_the_base` triggers game over and writes `highscore.txt` once. This is normal, no special handling needed.
- **Unit first, integration second**: unit tests cover details, integration tests cover collaboration. All 22 tests in this project take 0.02 seconds to run – you can absolutely run them after every change.

Run the whole suite:

```bash
py -m unittest -v
```

Reference output (22 tests for this project):

```
......................

----------------------------------------------------------------------
Ran 22 tests in 0.023s

OK
```

---

## 8E.6 Getting Tests into Your Development Flow

### 8E.6.1 Three Habits

1. **Run the tests before changing code**: all green means the starting point is healthy; run again after the change, still green means you didn't break anything.
2. **Every bug fix comes with a new test**: so it can't happen again. That's how the two real stories above became defence lines.
3. **For new features, first think "how will I verify it"**: you don't need strict TDD, but thinking clearly "how do I know it's correct" before writing the implementation saves a lot of rework.

### 8E.6.2 unittest and pytest

`pytest` is a third-party testing framework with more concise syntax, popular in the community:

```python
# pytest style: no TestCase inheritance, use assert directly
from battlefield import in_bounds

def test_outside():
    assert not in_bounds(-1, 0)
```

| | unittest | pytest |
|--|----------|--------|
| Origin | Standard library – comes with Python | Requires `pip install pytest` |
| Style | Inherit `TestCase`, use `self.assertXxx` | Plain functions and `assert` |
| Features | Sufficient | Many plugins, convenient parameterisation |
| Suits | Teaching, zero-dependency projects (like TankBattle) | Large projects |

TankBattle pursues "zero third-party dependencies", so the standard library's `unittest` is enough.

### 8E.6.3 Coverage and CI (Just Know the Concepts)

- **Coverage**: the proportion of code executed by tests out of total code. Tools can tell you "which code was never reached by tests". Higher coverage isn't necessarily better – 100% coverage can still miss business errors.
- **CI (Continuous Integration)**: every time code is pushed to a server, automatically install the environment and run the tests. TankBattle doesn't have this yet; that's for when you put the project on a remote repository.

### 8E.6.4 More Tests Isn't Always Better

- Tests are **code to be maintained**: when interfaces change, tests must change too.
- Don't test implementation details (e.g., "inside `_step_bullet`, is brick checked before steel?"); test **external behaviour** ("bullets should destroy brick walls").
- If a test frequently gives false alarms, either fix it or delete it – **a red alert nobody looks at is no alert at all**.

---

## 8E.7 Hands-on Exercises (Optional)

1. **Add tests for display width**: what should `display_width("钢")`, `display_width("abc")`, `display_width("我^")`, and the empty string each return?
2. **Write boundary tests for damage**: with `hp=1`, should taking 1 point and 100 points give the same result? Will `hp` go negative?
3. **Write a test for enemy AI**: in the list returned by `Enemy._choose_path(game)`, do all four directions appear exactly once? Convert to a `set` and assert its length.
4. **Add drama to the parser**: input two arrow keys at once `"\x1b[A\x1b[C"` should parse to `["up", "right"]`.
5. **Lengthen the smoke test**: change 3000 frames to 30000 and see how long it takes; then try a few different `random.seed` values to see if you can produce a crash.
6. **Experience "regression"**: deliberately change `config.FIELD_WIDTH` to `27`, run the tests, see which go red; then change it back. You'll get a direct feel for "how tests raise alarms".
7. **(Advanced) Write a lifecycle test for power-ups**: power-ups should disappear after 12 seconds. Write a test that advances continuously for 12.5 seconds and asserts `game.powerups` is empty.

---

## 8E.8 What You've Learned in This Section (Optional Content)

Even though this section is optional, if you've read it through you've already encountered these concepts:

1. **The value of testing**: let bugs run into you, rather than waiting for players to run into them;
2. **`assert` and `unittest`**: from "raise an exception if not satisfied" to the standard test framework – `TestCase`, `test_` naming, `self.assertXxx`, `setUp`, `subTest`, `assertRaises`;
3. **Unit testing**: pure functions, testing invariants of random functions, isolating dependencies with fake objects;
4. **Integration testing**: headless game running, controlling the test environment, pinpoint scenario tests, smoke/fuzz testing;
5. **Rendering tests**: strip ANSI colour codes and check each line's display width;
6. **Testing discipline**: independent, deterministic, no dependence on real time or terminal; unit first then integration; tests are code and must be maintained.

**Not learning it doesn't hurt**: the content in 8A–8D is already enough for you to read, modify, and extend this project. The day you find yourself struggling because you broke a feature, come back to this section – you'll find it much easier than it is now.