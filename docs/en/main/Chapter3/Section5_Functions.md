# Section 5: Functions

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Function | An organised, reusable block of code that performs a specific task |
| Definition | The process of creating a function using `def` |
| Call | Executing a function that has already been defined |
| Parameter | A value passed to a function when it is called |
| Return value | The result that a function sends back to the caller after execution |
| Positional argument | An argument passed in order based on the function's parameter positions |
| Keyword argument | An argument passed using `key=value` syntax |
| Default parameter | A parameter assigned a default value in the definition – can be omitted when calling |
| Local variable | A variable defined inside a function – only valid within that function |
| Global variable | A variable defined outside any function – accessible throughout the file |
| Scope | The region of a program where a variable can be accessed |

---

## 5.1 Why We Need Functions

You have already written code. So far, all of it has been linear – running from the first line to the last, with some branches and loops along the way.

Of course, the Blackjack program from earlier is an exception – I consider that a reasonably well‑structured single‑file program, with all content wrapped in functions, even when reuse wasn't strictly necessary – just for the sake of good practice.

But once your code exceeds a few dozen lines, problems start to emerge:

- The same logic appears in multiple places – you end up copying and pasting
- When you fix a bug in one copy, you forget to fix it elsewhere, creating inconsistencies
- The longer the code, the harder it is to read – you lose track of what goes where
- Testing a single piece of functionality requires running the entire program

**Functions solve all of these problems.**

A function is a **named block of code**. It has several characteristics:

- **Reusable**: define once, call many times
- **Accepts input**: via parameters
- **Returns output**: via `return`, passing data back to the caller
- **Independently testable**: you can run a single function to verify its correctness

---

## 5.2 The Simplest Function

Let's start with an extremely simple example:

```python
def say_hello() -> None:
    """Print a greeting message."""
    print("Hello, world!")
```

Breaking it down:

- `def`: keyword meaning "I'm about to define a function"
- `say_hello`: the function name you choose
- `()`: parentheses hold parameters – empty here
- `-> None`: return type hint – indicates this function returns no meaningful value
- `:`: colon – the function body is about to start
- The indented block: the function body – this code runs when the function is called

After defining it, you use it:

```python
say_hello()   # calling the function
```

Output:

```
Hello, world!
```

This is the simplest kind of function – no parameters, no return value, it just does one thing (prints).

**When to use this kind of function**: when a particular printing logic needs to appear in multiple places. For example, printing a logo at program startup and a thank‑you message at the end – put the printing code inside a function and call it from each location with a single line.

---

## 5.3 Functions with Parameters

Most functions need input data.

```python
def greet(name: str) -> None:
    """Print a personalized greeting."""
    print(f"Hello, {name}!")
```

> Again, `: str` is a type hint – it's optional, but including it makes the code more understandable.

Call it with one argument:

```python
greet("MKJ")
greet("YZY")
```

Output:

```
Hello, MKJ!
Hello, YZY!
```

**The essence of parameters**: a parameter is just a variable whose value is supplied by the caller. When you define `name: str`, you're receiving a name; when you call with `"MKJ"`, that value is assigned to `name`.

Multiple parameters are separated by commas:

```python
def greet_with_title(name: str, title: str) -> None:
    """Print a greeting with a title."""
    print(f"Good morning, {title} {name}!")

greet_with_title("MKJ", "Dr.")
greet_with_title("YZY", "Mr.")
```

Output:

```
Good morning, Dr. MKJ!
Good morning, Mr. YZY!
```

The order of parameters matters. In the call, `"MKJ"` corresponds to `name`, and `"Dr."` corresponds to `title`. These are **positional arguments**.

---

## 5.4 Functions with Return Values

Printing is fine for display, but it doesn't let you use the result in further logic. You need to hand the result back to the caller.

```python
def add(a: int, b: int) -> int:
    """Return the sum of two numbers."""
    result = a + b
    return result
```

Calling it:

```python
x = add(3, 5)
print(x)   # 8
```

`return` does two things:
1. Sends a value back to the caller
2. Immediately ends the function execution (any code after `return` is skipped)

You can return multiple values at once:

```python
def get_min_max(numbers: list[int]) -> tuple[int, int]:
    """Return the minimum and maximum values from a list."""
    return min(numbers), max(numbers)

minimum, maximum = get_min_max([3, 1, 7, 4, 9])
print(minimum, maximum)   # 1 9
```

Here, two values are returned and unpacked at the call site.

---

## 5.5 When to Use `return` vs `print`

```python
# Version 1: using print
def add_print(a, b):
    print(a + b)

# Version 2: using return
def add_return(a, b):
    return a + b
```

The difference:

```python
# Version 1: prints the result, but the program can't use the value
x = add_print(3, 5)   # prints 8, then x is None

# Version 2: prints nothing, but the program gets the result
x = add_return(3, 5)   # prints nothing, x = 8
y = x + 2              # y = 10 – can still be used
```

The rule is simple:

- If the value **will be used later** (in calculations, saved to a file, passed to another function) → use `return`
- If the value is **only meant to be seen by a human** (debugging, reporting) → use `print`

---

## 5.6 Default Parameters

Sometimes a parameter has the same value in most cases, and only occasionally needs to change. You can assign it a default.

```python
def attack(enemy: str, damage: int, critical_hit: bool = False) -> None:
    print(f"Attacking {enemy}!")
    if critical_hit:
        print(f"CRITICAL HIT! {damage * 2} damage!")
    else:
        print(f"{damage} damage.")

# Call without the third parameter
attack("Goblin", 30)
# Attacking Goblin!
# 30 damage.

# Explicitly pass critical_hit when needed
attack("Dragon", 50, critical_hit=True)
# Attacking Dragon!
# CRITICAL HIT! 100 damage!
```

Parameters with defaults must come after those without defaults.

```python
# Correct
def attack(enemy: str, damage: int, critical_hit: bool = False) -> None:
    ...

# Wrong – non‑default parameter cannot follow a default
def attack(enemy: str, critical_hit: bool = False, damage: int) -> None:
    ...
```

---

## 5.7 Keyword Arguments

So far, the order of arguments in a call must match the order of parameters in the definition:

```python
def greet(name: str, title: str) -> None:
    print(f"Hello, {title} {name}!")

greet("MKJ", "Dr.")   # OK
greet("Dr.", "MKJ")   # Logical error, but syntactically correct
```

With many parameters, it's easy to mix up the order. **Keyword arguments** let you specify parameter names, so order no longer matters:

```python
greet(name="MKJ", title="Dr.")   # OK
greet(title="Dr.", name="MKJ")   # Also OK – order irrelevant
```

Positional and keyword arguments can be mixed, but **positional arguments must come before keyword arguments**:

```python
def display_profile(name: str, age: int, city: str, job: str) -> None:
    print(f"{name}, {age}, {city}, {job}")

# First two positional, last two keyword
display_profile("MKJ", 22, job="Student", city="Beijing")
```

---

## 5.8 Scope: Where Variables Are Valid

Variables inside a function live in a different world from variables outside.

```python
x = 10   # global variable

def test() -> None:
    y = 5  # local variable – only valid inside this function
    print(x)   # can read global variables
    print(y)

test()
print(x)   # 10 – global variable still exists
print(y)   # NameError: y is not defined – cannot access y outside the function
```

**Global variables** (defined outside any function) can be read inside a function, but they cannot be directly modified:

```python
x = 10

def test() -> None:
    x = 20   # This creates a new local variable x – it has no effect on the global x
    print(x) # 20

test()
print(x)   # 10 – global x is unchanged
```

If you really need to modify a global variable inside a function, use the `global` keyword (not recommended – try to avoid this):

```python
x = 10

def test() -> None:
    global x
    x = 20

test()
print(x)   # 20 – this time it actually changed
```

**Why functions have independent variables**:

- Each function is like a small room – its contents (local variables) can only be used by people inside that room
- Global variables are like items in the lobby – visible from every room
- When you place something on a table in your room (a local variable), the lobby remains unchanged

This design ensures that functions don't accidentally interfere with each other. When you write a function, you don't need to worry about it inadvertently modifying variables used by other functions.

---

## 5.9 Comprehensive Example: An RPG Combat System

Let's put everything together. A small turn‑based combat simulation.

```python
"""RPG Combat Simulator.

A simple turn-based combat simulation demonstrating Python functions, parameters, return values, and scope.
"""

import random


def roll_dice(sides: int = 20) -> int:
    """Roll a dice with the given number of sides.
    
    Args:
        sides: Number of sides on the dice. Defaults to 20.
    
    Returns:
        int: Random number between 1 and sides.
    """
    return random.randint(1, sides)


def calculate_damage(attack: int, defense: int, critical: bool = False) -> int:
    """Calculate damage from an attack.
    
    Damage formula: attack * (100 / (100 + defense))
    Critical hits deal 1.5x damage.
    
    Args:
        attack: Attacker's attack stat.
        defense: Defender's defense stat.
        critical: Whether the attack is a critical hit.
    
    Returns:
        int: Calculated damage (at least 1).
    """
    base_damage = attack * (100 / (100 + defense))
    damage = int(base_damage)
    
    if critical:
        damage = int(damage * 1.5)
    
    return max(damage, 1)  # Always deal at least 1 damage


def player_turn(enemy_name: str, enemy_hp: int, enemy_defense: int) -> tuple[int, bool]:
    """Process a player's attack turn.
    
    Args:
        enemy_name: Name of the enemy being attacked.
        enemy_hp: Enemy's current health.
        enemy_defense: Enemy's defense stat.
    
    Returns:
        tuple[int, bool]: (damage_dealt, is_critical)
    """
    print(f"\nYou attack {enemy_name}!")
    
    # Roll to hit: need 8 or higher on a d20
    hit_roll = roll_dice()
    if hit_roll < 8:
        print(f"Your attack missed! (Rolled {hit_roll})")
        return 0, False
    
    # Check for critical hit: natural 20
    is_critical = hit_roll == 20
    attack_stat = 30  # Player's base attack
    
    damage = calculate_damage(attack_stat, enemy_defense, is_critical)
    
    if is_critical:
        print(f"CRITICAL HIT! Rolled 20!")
    else:
        print(f"Hit! Rolled {hit_roll}")
    
    print(f"Dealt {damage} damage to {enemy_name}!")
    return damage, is_critical


def enemy_turn(player_hp: int, player_defense: int) -> int:
    """Process an enemy's attack turn.
    
    Args:
        player_hp: Player's current health.
        player_defense: Player's defense stat.
    
    Returns:
        int: Damage dealt to player.
    """
    print(f"\nEnemy attacks you!")
    
    # Enemy always hits, but can critical on 20
    hit_roll = roll_dice()
    is_critical = hit_roll == 20
    
    enemy_attack = 15  # Enemy's base attack
    damage = calculate_damage(enemy_attack, player_defense, is_critical)
    
    if is_critical:
        print(f"Enemy critical hit!")
    else:
        print(f"Enemy hits you!")
    
    print(f"You take {damage} damage!")
    return damage


def combat(player_hp: int, player_defense: int, enemy_name: str, enemy_hp: int, enemy_defense: int) -> bool:
    """Run a complete combat encounter.
    
    Args:
        player_hp: Player's starting health.
        player_defense: Player's defense stat.
        enemy_name: Name of the enemy.
        enemy_hp: Enemy's starting health.
        enemy_defense: Enemy's defense stat.
    
    Returns:
        bool: True if player wins, False if player loses.
    """
    print("\n" + "=" * 40)
    print(f"COMBAT: Player vs {enemy_name}")
    print(f"Player HP: {player_hp} | {enemy_name} HP: {enemy_hp}")
    print("=" * 40)
    
    round_num = 1
    while player_hp > 0 and enemy_hp > 0:
        print(f"\n--- Round {round_num} ---")
        print(f"You: {player_hp} HP | {enemy_name}: {enemy_hp} HP")
        
        # Player goes first
        damage, _ = player_turn(enemy_name, enemy_hp, enemy_defense)
        enemy_hp -= damage
        
        # Check if enemy is defeated
        if enemy_hp <= 0:
            print(f"\n🎉 Victory! {enemy_name} is defeated!")
            return True
        
        # Enemy counterattacks
        damage = enemy_turn(player_hp, player_defense)
        player_hp -= damage
        
        # Check if player is defeated
        if player_hp <= 0:
            print(f"\nDefeat! You have been slain by {enemy_name}!")
            return False
        
        round_num += 1
    
    return True


def main() -> None:
    """Main game loop."""
    print("=" * 50)
    print("RPG COMBAT SIMULATOR")
    print("=" * 50)
    
    # Player stats
    player_hp = 100
    player_defense = 15
    
    # Enemy stats
    enemies = [
        {"name": "Goblin", "hp": 40, "defense": 5},
        {"name": "Skeleton", "hp": 55, "defense": 8},
        {"name": "Orc", "hp": 75, "defense": 12},
        {"name": "Dragon", "hp": 120, "defense": 20},
    ]
    
    score = 0
    for enemy in enemies:
        print(f"\nA wild {enemy['name']} appears!")
        input("Press Enter to fight...")
        
        won = combat(player_hp, player_defense, enemy['name'], enemy['hp'], enemy['defense'])
        
        if won:
            score += 1
            # Heal a little after winning
            player_hp = min(100, player_hp + 30)
            print(f"You heal 30 HP. Current HP: {player_hp}")
        else:
            print(f"Game over! You defeated {score} enemies.")
            break
    else:
        # This else runs if the for loop completes without a break
        print(f"\nYou are the champion! Defeated all {len(enemies)} enemies!")


if __name__ == "__main__":
    main()
```

This program contains the following functions:
- `roll_dice()`: no parameters, returns a value, with a default parameter
- `calculate_damage()`: multiple parameters, returns a value – demonstrates formula computation
- `player_turn()`: accepts parameters and returns multiple values (as a tuple)
- `enemy_turn()`: accepts parameters, returns a value – symmetric with `player_turn`
- `combat()`: a larger function that calls the smaller ones – contains the main loop, returns a boolean
- `main()`: top‑level control – manages data and calls `combat()` in a loop

Each function has a single, clear responsibility. This is the core idea of functions: **break a large problem into small pieces, and let each piece be handled by a function**.

### 5.9.1 Code Analysis: Function Breakdown of the RPG Combat System

The RPG combat system from the previous section has several more layers of function calls than Blackjack. Let's analyse each function's role, data flow, and any new syntax that appears.

#### 5.9.1.1 roll_dice() – The Simplest Utility Function

```python
def roll_dice(sides: int = 20) -> int:
    """Roll a dice with the given number of sides."""
    return random.randint(1, sides)
```

**Data flow**: input `int` (number of sides, optional) → output `int` (random number)

**Responsibility**: encapsulates random‑number generation. In a combat system, randomness is used for hit rolls and critical hits. Extracting this into a separate function gives:
- Callers don't need to know how to use `random.randint()`
- If the dice algorithm changes (e.g., weighted randomness), only this one place is updated
- Easier to test – later you can substitute deterministic values (not implemented here)

**New syntax**: we've seen default parameters before – here it appears again. `sides: int = 20` means `roll_dice()` defaults to a 20‑sided die if called without arguments.

#### 5.9.1.2 calculate_damage() – A Pure Computation Function

```python
def calculate_damage(attack: int, defense: int, critical: bool = False) -> int:
    base_damage = attack * (100 / (100 + defense))
    damage = int(base_damage)
    if critical:
        damage = int(damage * 1.5)
    return max(damage, 1)
```

**Data flow**: input `attack`, `defense`, `critical` → output `int` (damage)

**Responsibility**: only performs mathematical calculations, no printing at all. This is a typical **pure function** – the same input always produces the same output, with no side effects (does not modify external state or print).

**Design philosophy**: the combat system's damage formula may be adjusted frequently. Placing it in an independent function means that when you tweak the formula, callers remain completely unaffected. This makes the code easier to maintain.

**New syntax**: `max(damage, 1)` is a built‑in Python function that returns the larger of two arguments. Here it ensures damage is at least 1, avoiding zero‑damage hits when attack is too low.

#### 5.9.1.3 player_turn() – A Function That Returns Multiple Values

```python
def player_turn(enemy_name: str, enemy_hp: int, enemy_defense: int) -> tuple[int, bool]:
    print(f"\nYou attack {enemy_name}!")
    hit_roll = roll_dice()
    if hit_roll < 8:
        print(f"Your attack missed! (Rolled {hit_roll})")
        return 0, False
    is_critical = hit_roll == 20
    attack_stat = 30
    damage = calculate_damage(attack_stat, enemy_defense, is_critical)
    if is_critical:
        print(f"CRITICAL HIT! Rolled 20!")
    else:
        print(f"Hit! Rolled {hit_roll}")
    print(f"Dealt {damage} damage to {enemy_name}!")
    return damage, is_critical
```

**Data flow**: input `enemy_name`, `enemy_hp`, `enemy_defense` → output `tuple[int, bool]` (damage dealt, whether it was critical)

**Responsibility**: encapsulates an entire player turn – rolling to hit, checking critical hits, calculating damage, and printing combat messages.

**New syntax**: this is the first example of **returning multiple values**. `return damage, is_critical` actually returns a tuple `(damage, is_critical)`. The caller uses `damage, _ = player_turn(...)` to unpack.

**One question**: `enemy_hp` is passed in but never used inside the function. Why pass it?

In the current design, the player turn only needs enemy defense to compute damage – it doesn't need the enemy's current health. The printing also doesn't display enemy HP changes (that is handled in `combat()`). Passing `enemy_hp` here is redundant – a "leftover parameter" that might have been intended for later extensions. In clean code, you should avoid this; the parameter list should include only what the function actually needs.

Truth be told, I'm just making excuses – I originally designed it with this parameter, later found it unnecessary, and was too lazy to remove it.

You can modify it to:

```python
def player_turn(enemy_name: str, enemy_defense: int) -> tuple[int, bool]:
    # enemy_hp is no longer needed
```

Similarly, in `combat`, you can adjust the call:

```python
def combat(player_hp: int, player_defense: int, enemy_name: str, enemy_defense: int) -> bool:
    # ... earlier parts omitted ...
    damage, _ = player_turn(enemy_name, enemy_defense)
```

Try to read through the original code, find other redundant bits, and remove them. This is one.

#### 5.9.1.4 enemy_turn() – Symmetric Design

```python
def enemy_turn(player_hp: int, player_defense: int) -> int:
    print(f"\nEnemy attacks you!")
    hit_roll = roll_dice()
    is_critical = hit_roll == 20
    enemy_attack = 15
    damage = calculate_damage(enemy_attack, player_defense, is_critical)
    if is_critical:
        print(f"Enemy critical hit!")
    else:
        print(f"Enemy hits you!")
    print(f"You take {damage} damage!")
    return damage
```

**Data flow**: input `player_hp`, `player_defense` → output `int` (damage)

**Responsibility**: symmetric to `player_turn()` – handles the enemy's turn. The enemy always hits (no hit‑roll check) but still has a 5% chance of a critical hit (rolling a 20).

**Design philosophy**: the combat logic for player and enemy is split into two separate functions. Their interfaces differ (player needs enemy defense, enemy needs player defense), but their return types are consistent – both return `int` damage. This allows `combat()` to handle damage in a uniform way.

#### 5.9.1.5 combat() – The Main Combat Loop

```python
def combat(player_hp: int, player_defense: int, enemy_name: str, enemy_hp: int, enemy_defense: int) -> bool:
    print("\n" + "=" * 40)
    print(f"COMBAT: Player vs {enemy_name}")
    print(f"Player HP: {player_hp} | {enemy_name} HP: {enemy_hp}")
    print("=" * 40)
    
    round_num = 1
    while player_hp > 0 and enemy_hp > 0:
        print(f"\n--- Round {round_num} ---")
        print(f"You: {player_hp} HP | {enemy_name}: {enemy_hp} HP")
        
        damage, _ = player_turn(enemy_name, enemy_hp, enemy_defense)
        enemy_hp -= damage
        if enemy_hp <= 0:
            print(f"\nVictory! {enemy_name} is defeated!")
            return True
        
        damage = enemy_turn(player_hp, player_defense)
        player_hp -= damage
        if player_hp <= 0:
            print(f"\nDefeat! You have been slain by {enemy_name}!")
            return False
        
        round_num += 1
    
    return True
```

**Data flow**: input all stats from both sides → output `bool` (True = player wins, False = player loses)

**Responsibility**: orchestrates the flow of combat. It does not care about the specifics of damage calculation (delegated to `calculate_damage()`), nor the internal details of a turn (delegated to `player_turn()` and `enemy_turn()`). Its role is to:
1. Print combat start information
2. Control the round loop
3. Update HP
4. Determine victory/defeat and return the result

**The call chain for this section**:

```
main()
  └── combat()
        ├── player_turn()
        │     ├── roll_dice()
        │     └── calculate_damage()
        └── enemy_turn()
              ├── roll_dice()
              └── calculate_damage()
```

`main()` calls `combat()`, `combat()` calls `player_turn()` and `enemy_turn()`, and these call the lower‑level `roll_dice()` and `calculate_damage()`. Each layer only interacts with the layer immediately below it – no skipping.

**Why design it this way**: the main loop in `combat()` doesn't need to know "how dice are rolled" or "what the damage formula is". It only needs to know how much damage the player's turn deals and how much the enemy's turn deals. This layering keeps each level simple – changing a low‑level function doesn't affect the higher‑level logic.

#### 5.9.1.6 main() – Top‑Level Control

```python
def main() -> None:
    print("=" * 50)
    print("RPG COMBAT SIMULATOR")
    print("=" * 50)
    
    player_hp = 100
    player_defense = 15
    
    enemies = [
        {"name": "Goblin", "hp": 40, "defense": 5},
        {"name": "Skeleton", "hp": 55, "defense": 8},
        {"name": "Orc", "hp": 75, "defense": 12},
        {"name": "Dragon", "hp": 120, "defense": 20},
    ]
    
    score = 0
    for enemy in enemies:
        print(f"\nA wild {enemy['name']} appears!")
        input("Press Enter to fight...")
        
        won = combat(player_hp, player_defense, enemy['name'], enemy['hp'], enemy['defense'])
        
        if won:
            score += 1
            player_hp = min(100, player_hp + 30)
            print(f"You heal 30 HP. Current HP: {player_hp}")
        else:
            print(f"Game over! You defeated {score} enemies.")
            break
    else:
        print(f"\nYou are the champion! Defeated all {len(enemies)} enemies!")
```

**Data flow**: no input → no output (`-> None`)

**Responsibility**: the game's main console. Manages player state, enemy list, progression, and score tracking.

**New syntax**:

**`for...else`**: the `else` clause after a `for` loop executes when the loop **completes normally** (without being interrupted by a `break`). Here:

```python
for enemy in enemies:
    won = combat(...)
    if not won:
        break  # player lost – break the loop
else:
    # Loop completed without break – all enemies defeated
    print("You are the champion!")
```

If the player loses to an enemy, `break` is triggered and the `else` block is skipped. If all enemies are defeated without a `break`, the `else` runs. This is a Python‑specific construct.

**`input("Press Enter to fight...")`**: here `input()` is called without assigning its return value. The purpose is only to pause the program and wait for the user to press Enter. The input is discarded.

#### 5.9.1.7 Syntax Summary for Function‑Related Concepts in This Section

| Syntax | Example | Covered? |
|--------|---------|----------|
| Function definition | `def roll_dice():` | Covered |
| Positional arguments | `roll_dice(20)` | Covered |
| Default parameters | `sides: int = 20` | Covered |
| Keyword arguments | `calculate_damage(critical=True)` | Covered |
| Return value | `return damage` | Covered |
| Multiple return values | `return damage, is_critical` | Covered |
| Type annotations | `-> tuple[int, bool]` | Covered |
| Local variables | `round_num`, `hit_roll` | Covered |
| Global variables | (no `global` used) | Covered |
| `max()` built‑in | `max(damage, 1)` | New |
| `for...else` | `for enemy in enemies: ... else: ...` | New |
| `input()` as pause | `input("Press Enter...")` | New |

---

#### 5.9.1.8 Design Principles Demonstrated in This Section

1. **Single Responsibility**: each function does exactly one thing. `roll_dice()` only rolls dice, `calculate_damage()` only computes damage, `player_turn()` only handles the player's turn.

2. **Clear data flow**: inputs (parameters) and outputs (return values) are explicit. `combat()` receives all state via parameters and returns the outcome via a boolean.

3. **Avoiding side effects**: `calculate_damage()` is a pure function – it does not modify any external state, only returns a result. This makes it independently testable.

4. **Layered calls**: `main()` calls `combat()`, `combat()` calls `player_turn()` and `enemy_turn()`, and the lower layers call `roll_dice()` and `calculate_damage()`. Each layer only interacts with its immediate neighbours.

5. **Return values convey state**: `combat()` returns `bool`, `player_turn()` returns `(damage, is_critical)`. The caller decides what to do next based on these returned values.

6. **Extract repeated logic**: both player and enemy use the same `calculate_damage()` and `roll_dice()`. This avoids duplicating the same formula in two separate functions.

---

## 5.10 What You've Accomplished in This Section

1. Understood what functions are and why they are needed
2. Learned to define functions using `def`
3. Learned to pass arguments to functions (positional and keyword)
4. Learned to return values using `return`
5. Understood the difference between `print` and `return`
6. Learned to set default parameters
7. Understood local vs global variables and their scopes
8. Wrote a complete RPG combat system that uses everything you've learned