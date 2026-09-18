# Section 8A: Python Multi‑File Projects and Import Mechanisms

In the previous seven sections, your programs were all single‑file: a few dozen to a few hundred lines in one `.py` file, reading data with `input()`, computing, and printing with `print()`. Starting from this section, we'll change our approach: **instead of writing a toy from scratch, we'll read a real multi‑file project** – the book's sample project **TankBattle** (a terminal real‑time tank battle game). It consists of 11 Python modules, roughly 1600 lines of code, and runs at 60 FPS in the terminal, complete with a base, enemy AI, power‑ups, and levels. The full code is in `TankBattle.zip` in the download area.

> **Goals of Part 8 (8A–8E)**
>
> 1. **8A**: Learn Python's modules, imports, layering, and dependency management (this section);
> 2. **8B**: Dissect TankBattle – why each module exists, and what the dependency relationships and data flows look like;
> 3. **8C**: Fill in knowledge not covered in the previous seven sections but heavily used in the project (classes and inheritance, `@property`, exceptions and `with`, advanced type annotations, common standard library modules, terminal control…);
> 4. **8D**: Learn to systematically read an unfamiliar project and extend it;
> 5. **8E (optional, not required)**: Write unit and integration tests for the project.
>
> **About the code volume**: the project is about 1600 lines. 8A–8E will not reproduce all of it. Each section gives only **key fragments**, with the focus on analysis. I recommend opening the corresponding code files alongside your reading.
>
> This part is text‑heavy. I suggest reading it over several days.

---

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Module | A `.py` file containing code that can be imported by other files |
| Package | A folder containing `__init__.py`, which can hold multiple modules |
| Namespace | The "scope" of variable and function names, avoiding naming conflicts between different modules |
| Entry point | The file where the program starts executing – in this project, `main.py` |
| Dependency | When one module needs functionality from another module |
| Layering | Dividing code into different levels by responsibility – upper layers depend on lower layers, not vice versa |
| Separation of concerns | Each module is responsible for exactly one thing, without mixing unrelated logic |
| Interface | The functions/classes/attributes a module exposes – others use it only through this interface |
| Refactoring | Adjusting code structure without changing the program's external behaviour |
| Circular import | A imports B, and B imports A – Python raises an error or produces an incomplete module |
| Dependency inversion | Instead of importing at the top of a module when you need something, "let the caller pass the object in" |
| Context manager | An object supporting the `with` statement – responsible for "setup on entry, cleanup on exit" |
| Duck typing | Don't check what class an object is – just check whether it has the methods/attributes you need |
| Data‑driven | Describe rules with data (dictionaries, tables) instead of writing piles of `if/elif` |
| Fixed timestep | Logic updates use a fixed time slice (e.g., 1/120 second), independent of frame rate |
| Frame diffing | Only redraw the parts that changed during rendering, reducing terminal output |

---

## 8A.1 Why Split Code into Multiple Files

### 8A.1.1 How a Single File Spirals Out of Control

Suppose we put all 1600 lines of TankBattle into `main.py`. Let's get a feel for what happens:

**Problem 1: Finding code gets slower**

You want to make "bullet flight speed" a bit faster, so you open the file – at the top are constants, colours, terrain generation, and map randomisation; in the middle are the tank class and AI; later come bullets, power‑ups, and scoring; the rendering is at the very end. You scroll through several screens before finding that `0.04` in the middle of a function body.

**Problem 2: Change one place, break ten others**

All functions share the same namespace. You write a `row` inside the "bullet" function, and another `row` inside "map generation"; they live in their own scopes and won't interfere – but **module‑level variables and function definitions overwrite each other**: if you later define another `update()`, the earlier `update()` is gone forever. More commonly, you change the behaviour of `update()` and another place that calls it immediately breaks.

**Problem 3: Multi‑person collaboration conflicts**

Two people editing a 1600‑line file simultaneously will see a mess of merge conflicts in Git. Even if they edit unrelated sections, they interfere simply because "they're in the same file".

**Problem 4: Hard to reuse**

You wrote a really handy "terminal double‑width character alignment" function and want to use it in another project. But it's mixed in with the game main loop and colour tables, so you have to copy the whole file or manually dig that small piece out.

**Problem 5: Hard to test**

You want to verify "after pressing a key, is the player's state correct?", but input handling, game state, and rendering are tangled together – running it means starting the entire game. To test "do bullets pass through walls?", you first have to build a map.

### 8A.1.2 What Splitting Gives You

Actual numbers from TankBattle:

| Module | Lines | Responsibility |
|--------|-------|----------------|
| `game.py` | 450 | Game state machine, collisions, spawning, power‑ups, scoring |
| `display.py` | 213 | Drawing game data to the terminal |
| `battlefield.py` | 189 | Map generation, checking whether a tile is passable |
| `config.py` | 148 | All constants: sizes, speeds, colours, enemy attributes |
| `input_handler.py` | 126 | Cross‑platform non‑blocking keyboard input |
| `enemy.py` | 114 | Enemy AI |
| `main.py` | 103 | Entry point: main loop, cleanup |
| `player.py` | 94 | Player tank |
| `tank.py` | 78 | Tank base class |
| `bullet.py` | 24 | Bullets |
| `powerup.py` | 18 | Power‑ups |

The benefits of splitting can be verified concretely:

- **Change speed**: touch only 4 numbers in `config.py` (`PLAYER_MOVE_INTERVAL`, etc.), without changing a single line elsewhere;
- **Change appearance**: touch only the `TERRAIN_GLYPHS` and `TERRAIN_COLORS` dictionaries in `config.py`;
- **Change input method**: touch only `input_handler.py` – the game logic has no idea how keys are read;
- **Make enemies smarter**: change only `enemy.py` – map generation and rendering are unaffected;
- **Test rules independently**: `battlefield.py` and `bullet.py` can be imported and run without launching the terminal.

### 8A.1.3 An Analogy: From Junk Room to Toolbox

A single file is like a junk room: everything is piled together, finding a screwdriver means rummaging for ages, and pulling something out might knock over a box next to it.

Multiple files are like a toolbox: screwdrivers in one drawer, wrenches in another – each drawer holds only one kind of tool. When you need something, you open the corresponding drawer without disturbing anything else.

The cost is: you need to plan "what goes in each drawer" ahead of time. That is **architecture** – thinking through how to divide modules before writing code.

### 8A.1.4 Our Sample: The Evolution of TankBattle

This project didn't start out looking like this. Its evolution history perfectly illustrates the value of "splitting":

1. **First version (turn‑based)**: `main.py` had a `while` loop, moving one tile per keypress, bullets flying one frame at a time, using blocking `get_key()`. It was already split into 9 files, but the gameplay was poor.
2. **Second version (real‑time, the subject of 8B's analysis)**: changed to a 60 FPS real‑time game, adding a base, levels, power‑ups, bullet collisions, and a high score. The changes were:
   - **Added** `game.py` (game state machine and rules) and `powerup.py` (power‑ups);
   - **Rewrote** `display.py` (from "clear screen and redraw every frame" to "differential refresh") and `input_handler.py` (from blocking key reads to non‑blocking polling); `config.py` (constants were tabulated);
   - **Minor changes** to `player.py`, `enemy.py`, `bullet.py` (adapting to real‑time timing parameters);
   - `battlefield.py`'s "generate map" interface barely changed.

Note that last point: **upgrading the gameplay did not require a rewrite**, because the map, display, and input each lived in their own drawer. That is the most practical payoff of splitting.

---

## 8A.2 Python Modules and Import Mechanisms

### 8A.2.1 Importing an Entire Module

Suppose two files are in the same directory:

**math_utils.py**:

```python
"""Math utility functions."""

def add(a: int, b: int) -> int:
    """Return the sum of two numbers."""
    return a + b

PI = 3.14159
```

**main.py**:

```python
import math_utils

print(math_utils.add(3, 5))   # 8
print(math_utils.PI)          # 3.14159
```

`import math_utils` loads the entire file, and you use the `math_utils.` prefix to access its contents.

### 8A.2.2 Importing Names from a Module

```python
from math_utils import add, PI

print(add(3, 5))
print(PI)
```

No prefix needed, but the **risk of naming conflicts increases**: if the current file already has an `add`, the imported one overwrites it.

The project's choice: **each file imports only the names it actually needs**, and writes them clearly. For example, `player.py`:

```python
from config import (
    DELTAS, FIRE_HOLD_TIMEOUT, HOLD_GRACE_MAX, HOLD_TIMEOUT_MIN,
    PLAYER_BULLETS_BY_POWER, PLAYER_BULLET_SPEED_BY_POWER, PLAYER_CHAR,
    PLAYER_COLOR, PLAYER_MOVE_INTERVAL, PLAYER_SHOOT_COOLDOWN,
    REPEAT_MAX_GAP, SPAWN_SHIELD_TIME, TAP_HOLD_TIMEOUT,
)
from tank import Tank
```

Just from this import list you can tell: `player.py` depends on `config` and `tank`, not on `game`, not on `display`.

### 8A.2.3 Aliasing Imports

```python
import math_utils as mu
from math_utils import add as plus
```

Aliases are commonly used to shorten long module names or resolve naming conflicts. This project uses no aliases because the module names are all short.

### 8A.2.4 Multi‑line Imports: A Common Engineering Style

When a module needs dozens of constants, a single‑line import becomes very long. Use parentheses to wrap, with a trailing comma at the end of each line:

```python
from config import (
    FRAME_TIME, INPUT_POLL_SLEEP, LOGIC_STEP, MIN_COLS, MIN_LINES,
    STATE_GAME_OVER,
)
```

The benefits:

- Adding a constant means adding one line – Git diffs stay clean;
- The order is stable, making review easier;
- The trailing comma means "add another line" doesn't require editing the previous one.

### 8A.2.5 `__name__` and Entry Points

We covered this in Section 4.11: each file's `__name__` depends on how it is executed.

- Running `python main.py` directly → inside `main.py`, `__name__ == "__main__"`
- Another file doing `import main` → inside `main.py`, `__name__ == "main"`

The end of TankBattle's `main.py`:

```python
if __name__ == "__main__":
    main()
```

This means: "**Only start the game when this file is run directly**". Later, if you write tests, `import main` won't run the game; if you build a packaging tool, it won't produce side effects on import.

### 8A.2.6 Packages and Directories

When modules become numerous enough to need grouping, organise them in folders and put an `__init__.py` inside:

```
project/
├── main.py
└── utils/
    ├── __init__.py
    ├── math_utils.py
    └── string_utils.py
```

Importing:

```python
from utils.math_utils import add
```

TankBattle does not use packages because:

- There are only 11 modules, all at the same level;
- The dependencies are simple (constants → data → entities → coordination);
- Flat layout makes import statements shortest and most intuitive.

**When to create packages**: when modules exceed 20, when clear sub‑domains emerge (e.g., `ai/`, `render/`, `net/`), or when several large modules need to be grouped separately.

### 8A.2.7 Absolute vs Relative Imports

```python
# Absolute import: starting from the project root
from utils.math_utils import add

# Relative import: . current directory, .. parent directory – only inside packages
from .math_utils import add
from ..config import DEBUG
```

Beginners should prefer **absolute imports**: the path is clear, and you won't get confused about "what does this `.` actually refer to". This project uses absolute imports exclusively.

### 8A.2.8 A Module Is Only Loaded Once

This is very important: **no matter how many files do `import config`, Python executes `config.py` only once**, and everyone gets the same module object afterwards.

Therefore:

```python
# config.py
HIGH_SCORE_FILE = Path(__file__).resolve().parent / "highscore.txt"
```

This `Path(...)` is computed only on the first import, and all modules share the same path object – exactly what we want.

Conversely, **do not write side‑effectful code at the top level of a module**. If `config.py` had a top‑level `print("config loaded")`, it would print only once across the entire program – but at an unexpected moment (the first import). TankBattle's `config.py` top level contains only constants and dictionaries, no side effects; `main.py`'s side effect (starting the game) is protected by `if __name__ == "__main__"`.

### 8A.2.9 Circular Imports: The Most Common Pitfall in Multi‑File Projects

**What is a circular import**: A imports B, and B imports A.

```python
# a.py
import b

def hello():
    return "hello from a"

# b.py
import a          # ← circular

def world():
    return a.hello() + " and b"
```

Running `python a.py` directly:

```
ImportError: cannot import name 'hello' from partially initialized module 'a'
```

Reason: when `a` reaches `import b`, it starts loading `b`; `b`'s first line does `import a` again, but `a` has only executed halfway (`hello()` isn't defined yet), so you get a "half‑finished" module.

**Three solutions**:

1. **Push shared things down into a lower‑level module**. In this project, all constants and colours live in `config.py`, which depends on no business module – anyone can import it.
2. **Defer the import inside a function**. The old `player.py` did this:
   ```python
   def shoot(self) -> "Bullet":
       from bullet import Bullet      # placed inside the function, imported on call
       ...
   ```
3. **Dependency inversion: let the caller pass the object in**. This is the project's core technique – pay attention to `tank.py`:

```python
class Tank:
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

`tank.py` does **not** do `from game import Game`. It needs an object that "can determine whether it can enter", so it simply accepts it as a parameter `game` – as long as the passed object has a `can_tank_enter()` method (duck typing). This way:

- The dependency direction is always `game → tank`, never reversed;
- `tank.py` can be imported and tested independently (pass a fake object and it runs);
- Bullets, enemies, and the player all follow the same rule.

**The dependency direction rule in this project**:

```
config  ←  all modules can depend on it
battlefield / bullet / powerup / tank  ←  don't know game exists
player / enemy  ←  only depend on tank + config
game  ←  knows all entities; entities don't know game
display / input_handler  ←  completely unaware of what happens in game
                            (display only reads attributes of the object passed in)
main  ←  the only module that knows everyone
```

**A small question**: `display.py` needs to draw `game.player`, `game.enemies`, `game.grid`… Why doesn't it do `from game import Game`?

Because `render_frame(game)` simply takes `game` as a parameter. There is no line `import game` in `display.py` – it only requires the passed object to have those attributes. This is **duck typing**: "if it walks like a duck and quacks like a duck, treat it as a duck". The benefit is that when testing rendering, you can create a fake object with only `state`, `player`, and `grid` attributes, without launching the entire game.

### 8A.2.10 Standard Library, Third‑Party Libraries, and Local Modules

Three sources of `import`:

| Source | Examples | What this project uses |
|--------|----------|------------------------|
| Python standard library | `random`, `time`, `pathlib`, `select` | `random`, `time`, `pathlib`, `shutil`, `unicodedata`, `os`, `sys`, `select`, `signal` |
| Third‑party libraries | `numpy`, `requests`, `pygame` | **None at all** (zero dependencies – runs with just Python installed) |
| Local modules | Your own `.py` files | `config`, `battlefield`, `tank`, `game`… |

"Zero third‑party dependencies" is a valuable feature: switch to another computer, and as long as Python is installed, it runs – no `pip install` required.

---

By now, you've mastered the rules of modules and imports. The next section (8B) zooms in: let's see how TankBattle is assembled using these rules.