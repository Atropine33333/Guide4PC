# Section 8C: Knowledge Supplement – Concepts Not Covered in the First 7 Sections

Many constructs in 8B were not covered in the first 7 sections. This section fills them in systematically, with each item tied back to a real usage in the project.

---

## 8C: Knowledge Used in the Project but Not Covered in the First 7 Sections

The previous 7 sections covered: comments/variables/types, control flow, list comprehensions/slicing/unpacking, functions and scope, list/tuple/dict/set, copying, etc. But TankBattle is a real project, and it also uses a batch of "beyond-the-textbook" syntax and standard library features.

The table below gives an overview first, then we'll expand each one. **Each entry provides: syntax → minimal example → real usage in the project → caveats.**

| Concept | Location in project | Status in first 7 sections |
|---------|---------------------|----------------------------|
| Classes, objects, `__init__`, `self`, methods | All `class` definitions (tank/player/enemy/bullet/powerup/game/display/input_handler) | Not covered (only `type()` output `<class 'int'>` appeared in 2.4) |
| Inheritance, `super()`, polymorphism | `Player(Tank)`, `Enemy(Tank)` | Not covered |
| `@property` | `player.py`, `bullet.position`, `game.base_position`, etc. | Not covered |
| Class attributes vs instance attributes | The four defaults at the top of `Tank` | Not covered |
| Private naming convention `_name` | `_step`, `_lines`, `_clock`, `_hud_lines`, etc. | Not covered |
| Exceptions: `try/except/else/finally`, multiple exceptions, `raise` | `game.load_high_score`, `main`, `input_handler` | Only one `try/except ValueError` appeared in the Blackjack program in 3.6 |
| `with` and context managers (`__enter__`/`__exit__`) | `main.py` + `InputHandler` | Used `with open(...)` in 7.9, but not explained |
| Advanced type annotations: `list[list[str]]`, `tuple[int, int]`, `str \| None` | Almost every function signature | Basic annotations covered in 2.6.2; union types not covered |
| Conditional expression `a if cond else b` | `enemy._choose_path`, `display`, multiple places in `game` | Not covered |
| Chained comparison `0 <= r < H` | `battlefield.in_bounds`, `input_handler.parse_keys` | Not covered |
| f-string format specifiers `{score:06d}` | `display._hud_lines` | Not covered |
| `any()` / `all()` / `sum(generator)` | `game.can_tank_enter`, `active_bullets` | Only names appeared in a table in 4.3.3 |
| Modifying a container while iterating | `game._update_powerups` | Not covered (6.7's copy knowledge can bridge) |
| `random.choice / random / uniform / choices(weights)` | `battlefield`, `enemy`, `game` | Only covered `shuffle` (3.6), `randint` (5.8), `seed` (7.9) |
| `time.perf_counter / monotonic / sleep` | `main`, `player`, `input_handler` | Only used `time.time()` in 7.9 |
| `pathlib`'s `read_text / write_text` | `config`, `game` | Used `Path` in 7.9 but didn't explain |
| `shutil.get_terminal_size()` | `main`, `display` | Not covered |
| `unicodedata.east_asian_width()` | `display.display_width` | Not covered |
| ANSI cursor positioning, line clearing, alternate screen | `display.Renderer` | The pre-rewrite Section 8 only covered colours/clear screen/cursor |
| Non-blocking input: `termios`/`tty`/`select`/`msvcrt` | `input_handler` | Pre-rewrite Section 8 had a basic version; the full version here is new |
| `os.name` / `sys.platform` | `input_handler` | Not covered |
| `signal` | `main._install_signal_handlers` | Not covered |
| Fixed timestep loop, frame differential rendering | `main`, `display` | Not covered (engineering patterns, not syntax) |

### 8C.1 Introduction to Classes and Objects

**Why classes are needed**: when a group of data (position, HP, speed) and a group of operations (move, take damage, fire) are always tied together, use a class to package them.

```python
class Counter:
    """A simple counter."""

    def __init__(self, start: int = 0) -> None:
        self.value = start          # instance attribute

    def add(self, amount: int = 1) -> int:
        self.value += amount
        return self.value


c = Counter(10)      # c is an "instance"; __init__ is called automatically
print(c.value)       # 10
print(c.add(5))      # 15
print(c.value)       # 15
```

Three roles to remember:

| Role | In the example | Purpose |
|------|----------------|---------|
| Class | `Counter` | Template defining "what attributes and methods exist" |
| Instance | `c` | A concrete object created from the template, with its own `value` |
| `self` | First parameter of `add` | Represents "the instance calling this method"; Python passes it automatically |

The first parameter of a method definition must be `self`. `c.add(5)` is equivalent to `Counter.add(c, 5)` – that's why you don't write `self` when calling.

**Usage in the project**: `Tank`, `Player`, `Enemy`, `Bullet`, `PowerUp`, `Game`, `Renderer`, `InputHandler`, `Effect` are all classes. Each holds its own state: each tank has its own `row/col/hp`, each bullet has its own `accum`, and the game has global `score/stage`.

**Common pitfall**: don't do OOP for OOP's sake. Many functions in `battlefield.py` and `display.py` are written as plain functions – because they don't need to save state, and functions are simpler.

### 8C.2 Inheritance, `super()`, and Polymorphism

**Inheritance** expresses an "is-a" relationship: `Player` is a kind of `Tank`.

```python
class Tank:
    def __init__(self, row: int, col: int, hp: int) -> None:
        self.row = row
        self.col = col
        self.hp = hp

    def position(self) -> tuple[int, int]:
        return self.row, self.col


class Player(Tank):
    def __init__(self, row: int, col: int) -> None:
        super().__init__(row, col, hp=1)   # let the parent do common init
        self.power = 0                     # subclass-specific attribute

    def shout(self) -> str:
        return "player!"
```

- `super()` refers to "the parent class"; `super().__init__(...)` calls the parent's `__init__`, avoiding copying the `row/col/hp` assignments.
- Subclasses can **add** attributes/methods (`power`), and can **override** parent methods: in this project, `Player.update` and `Enemy.update` are same-named methods with completely different behaviour.
- **Polymorphism**: `game._update_playing` writes:
  ```python
  self.player.update(dt, self)
  for enemy in self.enemies:
      enemy.update(dt, self)
  ```
  This code doesn't care what `update` actually does. Later, if you add a "splitting enemy", as long as it also has `update(dt, game)`, `game.py` needs zero changes.

**Usage in the project**: `Enemy` and `Player` both inherit `Tank`, reusing `try_move`, `take_damage`, `can_fire`, `position`, `mark()`, etc. Subclasses only write "what's different".

### 8C.3 `@property`: Disguising Computation as an Attribute

Some "values" aren't stored; they're computed from other data. For example, the player's bullet speed is determined by "firepower level":

```python
class Demo:
    def __init__(self) -> None:
        self.power = 0

    @property
    def speed(self) -> float:
        return 1.0 + 0.3 * self.power


d = Demo()
print(d.speed)     # 1.0   – note: no parentheses!
d.power = 2
print(d.speed)     # 1.6
```

- A method decorated with `@property` is used like an attribute: write `d.speed`, not `d.speed()`.
- It **recomputes** on each access, so it always stays consistent with `power`.
- By default it's **read-only**: `d.speed = 5` raises `AttributeError: can't set attribute`.

**Usage in the project**: `Player.max_bullets`, `Player.bullet_speed`, `Player.breaks_steel` are all `@property`. Upgrading firepower only changes `self.power`, and these three values update automatically:

```python
    @property
    def max_bullets(self) -> int:
        return PLAYER_BULLETS_BY_POWER[self.power]
```

When a bullet is fired, it reads `tank.bullet_speed`, so the next shot after an upgrade is immediately faster.

**A real pitfall** (encountered during development): if the parent's `__init__` writes `self.max_bullets = 1`, and the subclass defines `max_bullets` as a getter-only `@property`, then creating a subclass instance raises `AttributeError` immediately – because `property` is a "data descriptor", and assignment first looks for its setter, raising an error if not found.

So `Tank`'s defaults are not written in `__init__`, but as **class attributes**:

```python
class Tank:
    # Defaults; subclasses may override them with properties.
    bullet_speed = 1.0
    max_bullets = 1
    shoot_cooldown = 0.3
    breaks_steel = False
```

The subclass's `@property` overrides the parent's class attribute; reads go through the subclass property first, and the parent's `__init__` never "assigns" it. That's what that comment is reminding about.

### 8C.4 Class Attributes vs Instance Attributes

```python
class Dog:
    species = "canis"          # class attribute: shared by all dogs

    def __init__(self, name: str) -> None:
        self.name = name        # instance attribute: one per dog


a = Dog("A")
b = Dog("B")
print(Dog.species, a.species, b.species)   # all canis
a.species = "wolf"        # shadows the class attribute for a alone
print(Dog.species)        # canis (class attribute unchanged)
print(a.species, b.species)   # wolf canis
```

- Read-only class attributes are shared by all instances, saving memory;
- Once you do `a.species = ...`, a new instance attribute is created on `a` itself, **shadowing** the class attribute;
- Criterion: **values that can differ per instance → instance attribute; default constants identical for all instances → class attribute**.

**Usage in the project**: `Tank`'s `bullet_speed/max_bullets/shoot_cooldown/breaks_steel` are "basic tank defaults"; `Enemy.__init__` assigns `bullet_speed` etc. as instance attributes; `Player` overrides them with `@property`.

### 8C.5 Private Naming Convention: `_name`

Python has no `private` keyword, only naming conventions:

| Form | Meaning |
|------|---------|
| `name` | Public, use freely |
| `_name` | "Internal use" – call from inside the class/module, don't depend on it externally |
| `__name` | Name mangling, generally used to avoid subclass collisions; beginners rarely need it |

**Usage in the project**: `game._step_bullet`, `game._update_playing`, `renderer._lines`, `player._clock`, `input_handler._read_unix` are all internal implementations. External modules (`main.py`, `display.py`) call only public interfaces, such as `game.update`, `game.handle_input`, `renderer.draw`.

Conventions aren't enforced. Test code sometimes calls `game._step_bullet(...)` to construct scenarios, but you should be aware: **interfaces starting with an underscore may change without notice**.

### 8C.6 Exception Handling: `try` / `except` / `else` / `finally`

The Blackjack program in 3.6 had one instance:

```python
try:
    bet = int(bet_input)
except ValueError:
    print("Invalid input. Enter a number.")
```

That's the most basic use. The full structure is:

```python
try:
    risky()                 # code that may fail
except ValueError:          # catch a specific exception
    print("the value is wrong")
except (OSError, TypeError) as exc:   # one except catches multiple exceptions
    print(f"another problem: {exc}")
else:
    print("runs only if no exception")     # optional
finally:
    print("always runs")                   # optional, for cleanup
```

Key points:

- **Exceptions are typed**. `ValueError` (bad value), `OSError` (file/system related), `KeyError` (missing dict key), `TypeError` (wrong type), etc.
- **`except Exception as exc`** binds the exception object to a variable so you can print its info.
- **`raise`** actively throws an exception:
  ```python
  if not 0 <= index < len(items):
      raise IndexError("index out of range")
  ```
- **Don't write a bare `except: pass`**. It swallows your program's own bugs and makes debugging a nightmare.
- Principle: **only catch exceptions you genuinely know how to recover from**; otherwise let them propagate.

**Usage in the project**:

1. Reading the high score: first play the file doesn't exist, or the file was hand-edited and corrupted:

   ```python
   try:
       return int(HIGH_SCORE_FILE.read_text(encoding="utf-8").strip())
   except (OSError, ValueError):
       return 0
   ```

   - `OSError` handles "file problems" (doesn't exist, no permission);
   - `ValueError` handles "content problems" (`int("abc")` raises this);
   - Both cases recover with default 0 and the game continues.

2. Writing the high score: the directory may be read-only; a write failure shouldn't crash the game:

   ```python
   try:
       HIGH_SCORE_FILE.write_text(f"{score}\n", encoding="utf-8")
   except OSError:
       pass
   ```

3. `main.py`'s outermost layer catches `KeyboardInterrupt`, letting Ctrl+C go through the normal exit path instead of printing a long traceback.

### 8C.7 `with` and Context Managers

Used in 7.9:

```python
with open(csv_path, 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f)
    ...
```

`with` guarantees "**cleanup always happens on leaving the block**", even if an exception occurs. The file is closed automatically.

How does it work? The object implements two methods:

```python
class Timer:
    def __enter__(self):
        self.start = time.perf_counter()
        return self                    # the `as` variable receives this return value

    def __exit__(self, exc_type, exc, tb):
        self.elapsed = time.perf_counter() - self.start
        print(f"took {self.elapsed:.3f} seconds")
        return False                   # return False: don't swallow, keep propagating


with Timer() as t:
    do_something()
print(t.elapsed)
```

- `__enter__`: called on entering the `with` block;
- `__exit__`: called on leaving (normal exit, `return`, or exception);
- Returning `True` from `__exit__` **swallows** the exception; returning `False`/`None` lets it propagate – beginners should always return `False`.

**Usage in the project**: `InputHandler` is a context manager. On entry it switches the terminal to raw mode; on exit it restores:

```python
with InputHandler() as keys:
    renderer.start()
    try:
        ...
    finally:
        renderer.stop()
```

Without `with`, if the program crashes mid-game, the terminal stays in raw mode – keys don't echo, Enter doesn't newline, and the user has to reopen the terminal. That's the value of "cleanup".

### 8C.8 Advanced Type Annotations

2.6.2 covered basic annotations (`name: str`, `-> str`) and stressed: **annotations are for humans/tools; the interpreter doesn't check them**. The project uses annotations extensively; here are a few new forms:

```python
def grid_size() -> tuple[int, int]:            # tuple type
    ...

def make_grid(w: int, h: int) -> list[list[str]]:   # nested list
    ...

def find_player() -> "Player | None":          # may not find → allow None
    ...

def tanks(self):                                # don't care about concrete return type
    yield self.player
```

| Form | Meaning | Notes |
|------|---------|-------|
| `list[int]` | List of ints | Python 3.9+ |
| `list[list[str]]` | 2D list of strings | Represents a map in the project |
| `tuple[int, int]` | Tuple of two ints | Represents `(row, col)` |
| `str \| None` | String or None | Python 3.10+; on older versions write `Optional[str]` |
| `-> None` | Returns nothing meaningful | The function just "does things" |
| `-> "Bullet"` | Return value is a Bullet | The string form defers resolution, avoiding import cycles |

**Usage in the project**: `battlefield.create_battlefield(stage) -> list[list[str]]` immediately shows it returns a map; `game.can_tank_enter(...) -> bool` immediately shows it's a check; `Game.__init__(self) -> None`.

**Note**: `str \| None` (a union type) is a syntax error before Python 3.10. If your environment is 3.8/3.9, write `Optional[str]` (requires `from typing import Optional`).

### 8C.9 Conditional Expression: `a if cond else b`

This compresses a simple `if/else` "into one line"; the result is an **expression** (usable on the right of `=`, in function arguments, inside strings).

```python
direction = "down" if dr > 0 else "up"
```

Equivalent to:

```python
if dr > 0:
    direction = "down"
else:
    direction = "up"
```

Nested (hard to read, but present in the project):

```python
d = ("down" if dr > 0 else "up") if abs(dr) > abs(dc) \
    else ("right" if dc > 0 else "left")
```

**Usage in the project**:

- `enemy._choose_path`: `horizontal = "right" if dc > 0 else "left"`;
- `display._build_board`: `color = ARMOR_HP_COLORS.get(enemy.hp, color)` alongside a ternary `if ... else`;
- `player.handle_key` doesn't chain, but the caller of `game._destroy_base(by_player)` passes a boolean from `bullet.faction == "player"`.

**Note**: conditional expressions suit "pick one of two" when short. More than two levels of nesting should go back to `if` statements.

### 8C.10 Chained Comparison

Python allows multiple comparisons in a chain, with the same meaning as in mathematics:

```python
0 <= row < FIELD_HEIGHT
```

Equivalent to `0 <= row and row < FIELD_HEIGHT`, but **`row` is evaluated only once**, and it's shorter and less error-prone (you can't mistype `row`).

The project also has character chained comparisons:

```python
while final < len(rest) and not ("@" <= rest[final] <= "~"):
```

This exploits ASCII continuity: `"@"` (64) to `"~"` (126) covers all letters and most symbols. The "terminating character" of an escape sequence falls in this range. **However, this is poor readability** and is only common in binary/protocol parsing; don't use it in everyday business code.

**Usage in the project**: `battlefield.in_bounds`, `display.overlay`'s `if not 0 <= row < FIELD_HEIGHT`.

### 8C.11 f-string Format Specifiers

2.6.2 covered only `f"{x}"`. In fact, braces can contain a **format specifier** after `:`:

```python
score = 1234
print(f"{score:06d}")    # 001234  – pad with zeros to 6 digits
print(f"{score:>8d}")    #     1234 – right-aligned to width 8
print(f"{score:<8d}")    # 1234     – left-aligned
print(f"{score:^8d}")    #   1234   – centred
rate = 0.5
print(f"{rate:.1%}")     # 50.0%   – percentage
print(f"{rate:.3f}")     # 0.500   – 3 decimal places
```

The specifier structure is `[[fill]align][sign][width][,][.precision][type]`; the common ones are above.

**Usage in the project**: the score in the HUD:

```python
f"{game.score:06d}"
```

The score grows from 0 to tens of thousands; using `{game.score}` directly would make the HUD jump left and right as digit count changes; `06d` fixes it to 6 digits, keeping the HUD stable.

### 8C.12 `any()` / `all()` / `sum()` with Generators

They appeared in a table in 4.3.3; here they are formally introduced:

```python
print(any([False, True, False]))    # True  – at least one is true
print(all([True, True, False]))     # False – must all be true
print(sum([1, 2, 3]))               # 6
```

They also accept **generator expressions** (from 4.3) and have **short-circuit** behaviour:

```python
# If any enemy is at (r, c), return True; stop checking further
any(t.alive and t.position == (r, c) for t in self.tanks())

# Count live bullets: each live bullet contributes a 1
sum(1 for b in self.bullets if b.alive and b.owner is tank)
```

**Usage in the project**:

- `game.can_tank_enter`: `any(...)` checks whether the target tile is occupied by another tank;
- `game.active_bullets`: `sum(1 for ...)` counts a tank's bullets on the field (decides whether it can fire again);
- `game._drop_powerup`: `any(...)` filters out tiles that already have a tank/power-up;
- `display.display_width`: `sum(...)` accumulates width character by character.

**Note**: `sum(1 for ...)` uses less memory than building a list and calling `len()`, but the difference is usually negligible; prefer clarity when writing code.

### 8C.13 Modifying a Container While Iterating

A classic trap:

```python
numbers = [1, 2, 3, 4]
for n in numbers:
    if n % 2 == 0:
        numbers.remove(n)
print(numbers)      # [1, 3]?
```

Looks fine, but it can miss deletions. Because `for` advances by index, and `remove` shifts later elements forward, causing a "skip".

**Three safe approaches**:

```python
# Approach 1: iterate over a copy
for n in list(numbers):
    if n % 2 == 0:
        numbers.remove(n)

# Approach 2: slice copy
for n in numbers[:]:
    ...

# Approach 3: rebuild directly (most recommended)
numbers = [n for n in numbers if n % 2 != 0]
```

**Usage in the project**:

- `game._update_powerups`: countdown and possibly `remove` at the same time, so `for powerup in list(self.powerups)`;
- `game._remove_dead`: rebuild directly with a comprehension: `self.enemies = [e for e in self.enemies if e.alive]`;
- `game._hit_player`: marks the bullet dead instead of removing on the spot, leaving it to `_remove_dead`.

**Link to 6.7**: `list(lst)` is a **shallow copy** – a new list, but the element objects are the same. If you're modifying "element attributes" rather than "the list itself" in the loop, shallow copy is perfectly sufficient.

### 8C.14 Common Standard Library Quick Tour

The standard library is Python's built-in function pack; `import` it and use it, no installation required.

**`random`: random numbers**

| Function | Purpose | Example in project |
|----------|---------|--------------------|
| `random.randint(a, b)` | Random integer in closed interval | `battlefield._find_blank_area` picks a random position |
| `random.random()` | Random float `[0, 1)` | `_add_brick_blocks` decides whether to place a brick by probability |
| `random.choice(seq)` | Pick one from a sequence | `game._drop_powerup` picks a tile |
| `random.shuffle(seq)` | Shuffle in place | `enemy._choose_path` shuffles candidate directions |
| `random.uniform(a, b)` | Random float in interval | Jitter of enemy fire interval |
| `random.choices(pop, weights=w, k=n)` | Weighted draw | `game._pick_enemy_type` draws enemy type by stage weight |

`random.choices` is the most common "gacha" function:

```python
names = ["basic", "fast", "armor"]
weights = [8, 3, 1]
kind = random.choices(names, weights=weights, k=1)[0]
```

**`time`: time**

| Function | Purpose | Example in project |
|----------|---------|--------------------|
| `time.perf_counter()` | High-precision, monotonically increasing timer, good for "how long between two calls" | Main loop computes `dt` |
| `time.monotonic()` | Monotonic clock, won't go backwards if system time changes | Input parsing's "ESC expiry" check |
| `time.sleep(s)` | Sleep | Main loop yields CPU |

**Why not `time.time()`**: it returns "wall-clock time" (seconds since 1970), which can jump when the system syncs or the user changes the clock. Using it for `dt` in a game loop could suddenly produce negative or huge values.

**`pathlib`: file paths**

```python
from pathlib import Path

path = Path(__file__).resolve().parent / "highscore.txt"
text = path.read_text(encoding="utf-8")   # read entire content
path.write_text("123\n", encoding="utf-8")  # write content
```

`read_text` / `write_text` are sugar for "open → read/write → close in one line" (internally still `open`). The `open(...)` in 7.9 is lower-level and suits streaming large files line by line; for small files, pathlib is easier.

**`shutil.get_terminal_size()`: terminal size**

```python
import shutil

size = shutil.get_terminal_size((80, 24))   # use defaults if unavailable
print(size.columns, size.lines)
```

`main.py` uses it to check whether the terminal is big enough; `display.Renderer` uses it to detect window resizing (force a full redraw after a resize).

**`unicodedata.east_asian_width(ch)`: character width**

```python
import unicodedata

unicodedata.east_asian_width("砖")   # 'W' (Wide)
unicodedata.east_asian_width("R")    # 'Na' (Narrow)
```

A return value of `"W"` or `"F"` means it occupies 2 columns in the terminal. `display.display_width` uses it to compute a string's actual display width, then decides how many spaces to pad – the key to Chinese terminal programs not "slanting".

### 8C.15 Terminal Advanced: ANSI Positioning, Line Clearing, Alternate Screen

The pre-rewrite Section 8 covered colours, clear screen, cursor visibility. A real-time game needs more control:

| Sequence | Purpose | Usage in project |
|----------|---------|------------------|
| `\033[{row};{col}H` | Move cursor to row `row`, column `col` (1-based) | Differential refresh positions at a line |
| `\033[K` | Clear from cursor to end of line | Wipe leftover chars when new line is shorter |
| `\033[2J` | Clear screen | Initialisation |
| `\033[H` | Cursor to top-left | Initialisation |
| `\033[?25l` / `\033[?25h` | Hide / show cursor | Hidden during the game |
| `\033[?1049h` / `\033[?1049l` | Enter / exit alternate screen | Don't pollute the user's terminal history |

**Newline pitfall in raw mode**: normally the terminal translates `\n` into "carriage return + line feed"; raw mode disables that, so `\n` only goes down, not to column 1. The project writes each line using **absolute positioning** (`\033[{row};1H`), completely sidestepping this; if you use `print()` line by line, write `\r\n`.

**`print` vs `sys.stdout.write`**: `print` automatically adds a newline and flushes the buffer. The renderer needs to assemble multiple chunks into one write (to reduce terminal flicker), so it uses `sys.stdout.write("".join(out))` and manually `flush()`.

**Windows compatibility issue**: consoles before Windows 10 don't recognise ANSI sequences by default; you need to enable "virtual terminal processing":

```python
import ctypes

kernel32 = ctypes.windll.kernel32
handle = kernel32.GetStdHandle(-11)          # standard output
mode = ctypes.c_uint32()
if kernel32.GetConsoleMode(handle, ctypes.byref(mode)):
    kernel32.SetConsoleMode(handle, mode.value | 0x0004)   # 0x0004 = VT mode
```

This code lives in `display.enable_windows_ansi()`, guarded by `os.name != "nt"` so it only runs on Windows. `ctypes` is "platform glue"; not understanding the details is fine – knowing "this is to enable ANSI support" is enough.

### 8C.16 Non-blocking Input and Platform Differences

`input()` is **blocking**: the program stops at that line until the user enters a full line and presses Enter. A real-time game needs "ask at any time: any keypress now? No? Keep running."

**Unix (Linux/macOS/WSL)**:

```python
import os, select, sys, termios, tty

fd = sys.stdin.fileno()               # file descriptor
old = termios.tcgetattr(fd)           # save terminal settings
tty.setraw(fd)                        # switch to raw mode
try:
    ready, _, _ = select.select([sys.stdin], [], [], 0)   # 0 timeout = return immediately
    if ready:                          # read only if data available
        data = os.read(fd, 64)         # read by bytes, won't block
        text = data.decode("utf-8", errors="ignore")
finally:
    termios.tcsetattr(fd, termios.TCSADRAIN, old)   # restore terminal
```

Key points:

- In raw mode the terminal no longer line-buffers or echoes; keys become bytes immediately;
- `select` just "asks whether data is available", without waiting;
- `os.read` reads **bytes**; use `.decode()` to turn into a string;
- `errors="ignore"` means undecodable bytes are skipped, not crashing;
- Terminal settings **must be restored** (`finally` or `with`).

**Windows**:

```python
import msvcrt

while msvcrt.kbhit():                 # is a key pressed?
    ch = msvcrt.getwch()              # read one character (Unicode)
    if ch in ("\x00", "\xe0"):        # first byte of function/arrow key
        code = msvcrt.getwch()        # second byte
        ...
```

**Unified representation of arrow keys**: different platforms give different raw data (Unix gives escape sequences like `\x1b[A`, Windows gives `\xe0` + a code), but `input_handler.py` translates them all into the same action name `"up"`. This is the value of "**unifying data formats at module boundaries**": changing platform touches one file.

### 8C.17 `os.name` and `sys.platform`

```python
import os, sys

os.name         # "nt" (Windows) / "posix" (Linux/macOS)
sys.platform    # "win32" / "linux" / "darwin"
```

`os.name` has only two values, good for a rough "Windows or Unix-like" check; `sys.platform` is finer and can distinguish macOS from Linux. The project uses:

```python
if os.name == "nt":
    import msvcrt
else:
    import termios
    import tty
```

**Note**: platform checks should appear only in the outermost adaptation module (`input_handler.py`, `display.enable_windows_ansi`); don't scatter `if os.name == "nt"` throughout game logic.

### 8C.18 `signal` and Graceful Exit

The `signal` module lets a program respond to operating system signals (like `kill`, terminal close).

```python
import signal

def _handler(signum, frame):
    raise KeyboardInterrupt

signal.signal(signal.SIGINT, _handler)    # Ctrl+C
signal.signal(signal.SIGTERM, _handler)   # kill
```

`main.py` uses it for two things:

1. When the program is `kill`ed or receives Ctrl+C, turn the control flow into `KeyboardInterrupt`;
2. So `try/finally` gets a chance to run `renderer.stop()` and restore terminal settings.

Interestingly: in Unix raw mode, the terminal no longer translates Ctrl+C into a signal (`ISIG` is off), so you receive byte `\x03`, mapped to `"quit"` by `input_handler`. **Both paths exit, because each layer did its own job well.**

### 8C.19 Fixed Timestep Loop: Speed Independent of Computer Speed

First, an incorrect approach:

```python
while True:
    dt = compute_dt()
    enemy.move_one_step()      # move one tile per frame
    render()
```

Problem: a fast computer has high frame rate and enemies move fast; a slow computer makes them slow. Two computers playing the same game have different speeds.

**Fixed timestep** approach: cut logic updates into identical small slices.

```python
LOGIC_STEP = 1 / 120      # each slice 8.3 ms
accumulator = 0.0
last = time.perf_counter()

while running:
    now = time.perf_counter()
    accumulator += min(now - last, 0.25)   # how many seconds this round (capped)
    last = now

    while accumulator >= LOGIC_STEP:       # enough accumulated, update once
        game.update(LOGIC_STEP)
        accumulator -= LOGIC_STEP

    if now >= next_frame:                  # rendering follows its own rhythm
        draw()
        next_frame = now + FRAME_TIME
```

Benefits:

- **Consistent speed**: regardless of computer speed or how many updates per frame, game time is real;
- **Reproducible**: same input, roughly predictable results, easy to debug and test;
- **Logic and render decoupled**: 120 Hz logic, 60 Hz render, independent.

In the project, every action in `game.update(dt)` (move timing, bullet timing, shield countdown) is written around `dt`, so changing `LOGIC_STEP` size doesn't change game speed, only precision.

`min(now - last, 0.25)` is a necessary safeguard: if the system sleeps 10 seconds, the difference of 10 makes the inner `while` run 1200 updates, "fast-forwarding" and getting laggier. Capping at 0.25 s means "drop this chunk of game time on a hitch".

### 8C.20 Frame Differential Rendering

Old rendering every frame:

```python
clear_screen()          # clear entire screen
print 19 lines
```

At 30/60 FPS this flickers, and writes thousands of characters per frame; the terminal can't keep up.

The project's approach: **only redraw changed lines**.

```python
# First frame: cache empty, all lines "changed", write all
# Second frame: only the player's line and HUD changed → write only those
for index, line in enumerate(lines):
    if line != self._lines[index]:
        write(f"\033[{index + 1};1H{line}\033[K")
self._lines = list(lines)
```

The cost is saving "each line of the previous frame"; the benefit is static walls produce zero output. Measured single-frame render cost is about 0.2 ms; 60 FPS is effortless.

**A boundary that must be handled**: after the terminal window resizes, some lines' content may be unchanged but their position/width changed; the cache's old "unchanged" judgement causes screen garbling. So `draw()` first compares terminal size; if different, clears the cache and forces a full redraw.

Differential thinking isn't only for the terminal: front-end Virtual DOM, database incremental backups, Git diff – all are essentially "**only process what changed**".

---

### 8C.21 Supplement Checklist Review

Listing these "new faces" again, mapped to project files, for easy lookup:

| New knowledge | One sentence | Where to look |
|---------------|--------------|---------------|
| Classes/objects/self/`__init__` | Packaging data + behaviour | `tank.py`, `bullet.py` |
| Inheritance/`super()`/polymorphism | Subclass reuses parent, same interface | `player.py`, `enemy.py` |
| `@property` | Computed attributes, read-only fields | `player.py` |
| Class vs instance attributes | Shared default vs own copy | `tank.py` |
| `_name` convention | Internal interface marker | Almost every file |
| Exception handling | Recovery paths on error | `game.py` save, `main.py` |
| `with` / context managers | Setup on enter, cleanup on exit | `input_handler.py` + `main.py` |
| Advanced type annotations | Container generics, union types | All signatures |
| Conditional expression | One-line pick-one-of-two | `enemy.py`, `display.py` |
| Chained comparison | `0 <= x < n` | `battlefield.py`, `input_handler.py` |
| f-string format specifier | Zero-pad/align/precision | `display.py` |
| `any/all/sum` | Aggregate checks | `game.py` |
| Iterating a copy | Safe way to delete while iterating | `game.py` |
| `random.choice/choices` | Gacha/weighted random | `game.py`, `enemy.py` |
| `time.perf_counter` | High-precision timing | `main.py` |
| `pathlib` | Paths and read/write | `config.py`, `game.py` |
| `shutil.get_terminal_size` | Terminal size | `main.py`, `display.py` |
| `unicodedata` | CJK character width | `display.py` |
| ANSI positioning/line clear/alternate screen | Terminal "brush" | `display.py` |
| Non-blocking input | Keyboard reading that doesn't freeze | `input_handler.py` |
| Platform detection | Windows/Unix branching | `input_handler.py` |
| `signal` | Graceful exit | `main.py` |
| Fixed timestep | Logic decoupled from frame rate | `main.py` |
| Frame diffing | Draw only what changed | `display.py` |

---

The syntax is filled in. The final section (8D) covers two things: how to systematically read an unfamiliar project, and 8 exercises to turn TankBattle into your own project.