# Section 3: Control Flow

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Conditional | Decides which block of code to execute based on whether a condition is true |
| Branch | Different execution paths formed by `if`, `elif`, and `else` |
| Code block | Several lines of code with the same indentation, treated as a unit |
| Loop | Repeating the same piece of code |
| Iteration | Processing elements of a sequence one by one |
| Traversal | Same as iteration – visiting each element of a container in order |
| Nesting | Putting loops or conditionals inside other loops or conditionals |
| Break | `break` – terminates the loop early |
| Skip | `continue` – skips the rest of the current iteration and moves to the next |
| Boolean expression | An expression that evaluates to `True` or `False` |

---

## 3.1 What Is Control Flow

Control flow is **the order in which code is executed**.

Without any control structures, code runs line by line – finish line 1, then line 2, then line 3. That only works for linear scripts and cannot do anything complex.

You need three abilities to make a program "come alive":

1. **Branching**: take different paths depending on conditions (`if` / `elif` / `else`)
2. **Loops**: repeat a block of code (`for` / `while`)
3. **Jumps**: exit early or skip iterations inside loops (`break` / `continue`)

These three control structures are at the core of every programming language, and Python is no exception.

---

## 3.2 Branching: if / elif / else

The `if` statement decides whether to execute a block of code based on a condition. Basic syntax:

```python
if condition:
    # do something when condition is True
```

For multiple branches, use `elif` (short for `else if`) and `else`:

```python
if condition1:
    # runs when condition1 is True
elif condition2:
    # runs when condition1 is False and condition2 is True
else:
    # runs when all conditions above are False
```

A real example:

```python
def classify_number(x: int) -> str:
    """Classify a number as positive, negative, or zero."""
    if x > 0:
        return "positive"
    elif x < 0:
        return "negative"
    else:
        return "zero"
```

You can have multiple `elif` clauses – they are checked from top to bottom. As soon as one condition is `True`, its block executes and the entire `if-elif-else` structure is skipped. There can be at most one `else`, and it must come last.

---

## 3.3 Loops: for

The `for` loop is used to **iterate** over a sequence (list, tuple, string, range, etc.). It takes each element from the sequence one by one and executes the loop body for each.

```python
fruits = ["apple", "banana", "cherry"]
for fruit in fruits:
    print(fruit)
```

`range()` is the most common companion of `for`. It generates a sequence of integers:

```python
# range(stop): 0, 1, 2, ..., stop-1
for i in range(5):
    print(i)   # 0 1 2 3 4

# range(start, stop): start, start+1, ..., stop-1
for i in range(2, 6):
    print(i)   # 2 3 4 5

# range(start, stop, step): increment by step
for i in range(0, 10, 2):
    print(i)   # 0 2 4 6 8
```

When iterating over a list, if you need both the index and the element, use `enumerate()`:

```python
fruits = ["apple", "banana", "cherry"]
for idx, fruit in enumerate(fruits):
    print(f"{idx}: {fruit}")
```

Output:

```
0: apple
1: banana
2: cherry
```

> **Introducing enumerate(): get index and value simultaneously**
>
> When iterating over a list, you often need to know both the **current element's value** and its **position (index)**. `enumerate()` is designed for exactly that.
>
> `enumerate()` returns an iterator that produces a tuple of **(index, value)** for each element. You can unpack it directly with `i, fruit` in the loop header.
>
> **Specifying a starting index**:
>
> The default start is `0`. To start from `1`, add the `start` parameter:
>
> ```python
> for i, fruit in enumerate(fruits, start=1):
>     print(f"{i}: {fruit}")
> ```
>
> Output:
>
> ```
> 1: apple
> 2: banana
> 3: cherry
> ```
>
> **Common use cases**:
>
> - Printing data with line numbers
> - Knowing which element you are processing inside a loop
> - Combined with `if` to locate an element
>
> ```python
> # Find the first element starting with 'b' and its position
> for i, fruit in enumerate(fruits):
>     if fruit.startswith("b"):
>         print(f"Found '{fruit}' at index {i}")
>         break
> ```

Iterating over a dictionary:

```python
student = {"name": "MKJ", "age": 18, "major": "Phys"}
for key in student:           # Iterate over keys
    print(key)
for key, value in student.items():  # Iterate over both key and value
    print(f"{key} = {value}")
```

---

## 3.4 Loops: while

The `while` loop executes the loop body **while the condition is true**. The difference from `for` is:

- `for` knows how many iterations (the length of the sequence)
- `while` does not – you must control when the loop ends yourself

```python
count = 0
while count < 5:
    print(count)
    count += 1   # must update manually, otherwise infinite loop
```

If you forget to update the variable in the condition, the loop never ends:

```python
count = 0
while count < 5:
    print(count)   # infinite loop, count is always 0
```

`while` is suitable when you don't know the number of iterations in advance, only the condition to stop – for example, waiting for the user to enter a specific command.

---

## 3.5 Loop Control: break and continue

`break`: immediately ends the entire loop and jumps to the code after the loop.

```python
for i in range(10):
    if i == 5:
        break
    print(i)   # prints 0 1 2 3 4
```

`continue`: skips the rest of the current iteration and moves directly to the next one.

```python
for i in range(10):
    if i % 2 == 0:
        continue
    print(i)   # prints 1 3 5 7 9 (skips even numbers)
```

Both `break` and `continue` work in both `for` and `while` loops.

---

## 3.6 Hands‑on: Blackjack

### 3.6.1 Game Rules (Simplified)

- Uses a standard deck with ranks 2‑10, J, Q, K, A
- J, Q, K count as 10, A counts as 1 or 11 (whichever is better for the player)
- Player and dealer (computer) each get two cards; the dealer's first card is face up, the second is hidden
- Player can choose "Hit" (take another card) or "Stand" (stop)
- If the player's total exceeds 21, they bust and lose
- After the player stands, the dealer must hit until they reach 17 or more
- Compare totals – the one closer to 21 without going over wins

### 3.6.2 Complete Code

**Warning**: This code uses some features not yet covered, such as the `.pop()` method, `' '.join(...)` chaining, `hand[1:]` slicing, and the generator expression `sum(value for _, value in hand)`. If you don't understand them, skip reading the code for now, copy it, create a new `BlackJack.py` file in VS Code, paste the code, activate your virtual environment as described in Section 1.7, and run `py BlackJack.py` to play a round or two. Once you feel less intimidated, come back to Section 3.6.5 for a line‑by‑line breakdown – it will feel much easier.

```python
"""Blackjack (21) card game.

A simplified command-line implementation of Blackjack.
Player plays against the dealer (computer).
"""

import random


def create_deck() -> list[tuple[str, int]]:
    """Create a standard deck of 52 cards.

    Returns:
        List of tuples, each containing (rank, value).
        Face cards (J/Q/K) have value 10, Ace has value 11 (adjusted later).
    """
    ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    values = {
        '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
        '7': 7, '8': 8, '9': 9, '10': 10,
        'J': 10, 'Q': 10, 'K': 10, 'A': 11
    }
    deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
    random.shuffle(deck)
    return deck


def calculate_hand(hand: list[tuple[str, int]]) -> int:
    """Calculate the total value of a hand.

    Aces are counted as 11 initially, but reduced to 1 if the total exceeds 21.

    Args:
        hand: List of (rank, value) tuples.

    Returns:
        int: The best total value (<= 21 if possible).
    """
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')
    
    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1
    
    return total


def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    """Format a hand as a readable string.

    Args:
        hand: List of (rank, value) tuples.
        hide_first: If True, hide the first card (dealer's hole card).

    Returns:
        str: Formatted hand representation.
    """
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)


def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    """Determine if dealer should take another card.

    Dealer must hit if total is less than 17.

    Args:
        hand: Dealer's current hand.

    Returns:
        bool: True if dealer should hit.
    """
    return calculate_hand(hand) < 17


def play_blackjack() -> None:
    """Main Blackjack game loop."""
    print("=" * 50)
    print("BLACKJACK (21)")
    print("=" * 50)
    print("Commands: hit, stand, quit\n")
    
    chips = 100
    bet = 0
    
    while True:
        # ---- Betting phase ----
        print(f"\nYou have {chips} chips.")
        while True:
            bet_input = input("Place your bet (or type 'quit' to exit): ").strip().lower()
            if bet_input == 'quit':
                print("Thanks for playing!")
                return
            try:
                bet = int(bet_input)
                if bet <= 0:
                    print("Bet must be positive.")
                    continue
                if bet > chips:
                    print(f"You only have {chips} chips.")
                    continue
                break
            except ValueError:
                print("Invalid input. Enter a number.")
        
        chips -= bet
        
        # ---- Deal initial cards ----
        deck = create_deck()
        player_hand = [deck.pop(), deck.pop()]
        dealer_hand = [deck.pop(), deck.pop()]
        
        print(f"\nDealer shows: {display_hand(dealer_hand, hide_first=True)}")
        print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")
        
        # ---- Player's turn ----
        player_bust = False
        while True:
            player_total = calculate_hand(player_hand)
            
            if player_total > 21:
                player_bust = True
                break
            if player_total == 21:
                print("You got 21!")
                break
            
            action = input("Hit or stand? ").strip().lower()
            if action == 'quit':
                print("Thanks for playing!")
                return
            elif action == 'hit':
                new_card = deck.pop()
                player_hand.append(new_card)
                print(f"You drew: {new_card[0]}")
                print(f"Your hand: {display_hand(player_hand)} (total: {calculate_hand(player_hand)})")
            elif action == 'stand':
                break
            else:
                print("Invalid command. Please enter 'hit' or 'stand'.")
                continue
        
        # ---- Dealer's turn (only if player didn't bust) ----
        dealer_bust = False
        if not player_bust:
            print(f"\nDealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")
            
            while dealer_should_hit(dealer_hand):
                new_card = deck.pop()
                dealer_hand.append(new_card)
                print(f"Dealer drew: {new_card[0]}")
                print(f"Dealer's hand: {display_hand(dealer_hand)} (total: {calculate_hand(dealer_hand)})")
            
            dealer_total = calculate_hand(dealer_hand)
            if dealer_total > 21:
                dealer_bust = True
        
        # ---- Determine winner ----
        player_total = calculate_hand(player_hand)
        dealer_total = calculate_hand(dealer_hand)
        
        print("\n" + "-" * 40)
        
        if player_bust:
            print("You busted! Dealer wins.")
        elif dealer_bust:
            print("Dealer busted! You win!")
            chips += bet * 2
        elif player_total > dealer_total:
            print(f"You win! {player_total} vs {dealer_total}")
            chips += bet * 2
        elif player_total < dealer_total:
            print(f"Dealer wins! {player_total} vs {dealer_total}")
        else:
            print(f"Push! {player_total} vs {dealer_total}")
            chips += bet
        
        print(f"Your chips: {chips}")
        
        if chips <= 0:
            print("\nYou're out of chips! Game over.")
            break


if __name__ == "__main__":
    play_blackjack()
```

### 3.6.3 Import Explanation

The code imports only one module at the top:

```python
import random
```

`random` is a Python standard library module that provides `shuffle()` to shuffle the deck. Note that we do **not** import `List` or `Tuple` – the code uses Python 3.9+'s built‑in generic syntax `list[tuple[str, int]]`, which does not require any extra imports.

**About `list[tuple[str, int]]`**:

This is a type annotation syntax introduced in Python 3.9, meaning "a list where each element is a tuple containing a string and an integer". It is essentially a generic form of `list` – it tells you what kind of elements the list holds.

The standard library `typing` also provides `List` and `Tuple` (with capital letters), but they have been superseded by the built‑in `list` and `tuple` since Python 3.9. You can use the built‑ins directly without additional imports.

### 3.6.4 Code Analysis: Where Is the Control Flow?

This game uses every control structure covered in this section. Let's map them:

**Branching (if / elif / else)**

- `calculate_hand()` contains a conditional in `while total > 21 and ace_count > 0`
- `dealer_should_hit()` returns a boolean – essentially `return calculate_hand(hand) < 17`
- Player input handling: `if action == 'quit'` → `elif action == 'hit'` → `elif action == 'stand'` → `else`
- Winner determination: `if player_bust` → `elif dealer_bust` → `elif player_total > dealer_total` → `elif player_total < dealer_total` → `else` (push)
- Chips check: `if chips <= 0` → `break`

**Loops**

| Loop | Location | Purpose |
|------|----------|---------|
| `while True` | Main loop | Each round restarts until the player quits or runs out of chips |
| `while True` | Betting phase | Repeatedly ask for a bet until valid input is entered |
| `while True` | Player turn | Repeatedly ask for Hit/Stand until the player stands, busts, or reaches 21 |
| `while dealer_should_hit` | Dealer turn | Auto‑draw until the total reaches 17 or more |

**Jump controls**

| Jump | Location | Purpose |
|------|----------|---------|
| `break` | Betting phase | Exit after a valid bet |
| `return` | Betting phase | When `quit` is entered, exit the entire program |
| `break` | Player turn | When reaching 21, stop asking and go to dealer turn |
| `break` | Player turn | When the player stands |
| `continue` | Player turn | Invalid command – ask again |
| `break` | Main loop | When chips reach 0, game over |

**Nesting**

- Main loop contains a betting loop, which contains input validation logic
- Main loop contains the player turn loop, which contains branching
- Winner determination contains nested conditional branches

### 3.6.5 Function‑by‑Function Breakdown

We already analysed control flow; now let's look at the code from the function level and see how data flows between them and what each function is responsible for.

#### 3.6.5.1 create_deck() – Build a Deck

```python
def create_deck() -> list[tuple[str, int]]:
    ranks = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    values = {
        '2': 2, '3': 3, '4': 4, '5': 5, '6': 6,
        '7': 7, '8': 8, '9': 9, '10': 10,
        'J': 10, 'Q': 10, 'K': 10, 'A': 11
    }
    deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
    random.shuffle(deck)
    return deck
```

**Data flow**: no input → outputs a `list[tuple[str, int]]`

**Line‑by‑line breakdown**:

| Line | What it does | Result type |
|------|--------------|-------------|
| `ranks = [...]` | Defines the 13 ranks | `list[str]` |
| `values = {...}` | Maps rank to point value | `dict[str, int]` |
| `deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]` | List comprehension – generates 4 copies of each rank (suits), total 52 cards | `list[tuple[str, int]]` |
| `random.shuffle(deck)` | Shuffles the deck in place | `None` (modifies the object) |
| `return deck` | Returns the deck | `list[tuple[str, int]]` |

**Breaking down the list comprehension**:

```python
# Outer loop: go through each rank
# Inner loop: repeat each rank 4 times (for the four suits)
# Each element is a tuple (rank, values[rank])
deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
```

Equivalent to:

```python
deck = []
for rank in ranks:
    for _ in range(4):
        deck.append((rank, values[rank]))
```

`_` is a conventional variable name in Python meaning "I don't need this value, I'm just using it as a placeholder". Here the inner loop just repeats 4 times – we don't need the loop variable.

`.append` is a built‑in method of `list` – it adds an element to the end of the list.

**Why use `tuple` here**: each card is described by two fields – rank (`str`) and value (`int`). Bundling them together in a tuple is clearer than using two separate lists. Throughout the code, every card is always passed around as `(rank, value)`.

**`return` vs `print`**:

- `return`: hands a value back to the caller. When you write `deck = create_deck()`, `return` passes the deck to the `deck` variable.
- `print`: displays the value on the screen for the user. Calling `print(deck)` just outputs text – the program cannot use that data further.

If a function is only responsible for "generating data" and not "displaying data", use `return`. If it's responsible for "showing results", use `print`. In this program, `create_deck()` outputs nothing – it generates data and returns it to the caller.

---

#### 3.6.5.2 calculate_hand() – Compute Hand Total

```python
def calculate_hand(hand: list[tuple[str, int]]) -> int:
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')
    
    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1
    
    return total
```

**Data flow**: input `list[tuple[str, int]]` (hand) → output `int` (total)

**Line‑by‑line breakdown**:

| Line | What it does |
|------|--------------|
| `sum(value for _, value in hand)` | Sums the values of all cards; `_` ignores the rank, we only want the value |
| `sum(1 for rank, _ in hand if rank == 'A')` | Counts how many Aces are in the hand |
| `while total > 21 and ace_count > 0` | If over 21 and there is an Ace, change one Ace from 11 to 1 (subtract 10) |
| `return total` | Returns the final total |

**Break down `sum(value for _, value in hand)`**:

This uses a **generator expression** – similar to a list comprehension but it does not create a full list; it generates values one by one for `sum()` to accumulate.

```python
# Expanded equivalent:
total = 0
for _, value in hand:
    total += value
```

`sum()` takes an iterable and adds up all the values. The generator `value for _, value in hand` yields the `value` of each card in hand.

**Why use `_`**: we use `_` to indicate "I don't need this value". In `for _, value in hand`, each card is a `(rank, value)` tuple – during unpacking, `rank` goes to `_`, meaning we ignore it. This is idiomatic Python.

**Break down `while total > 21 and ace_count > 0`**:

This is a classic use of `while`. Ace handling requires repeated checks – each time we reduce one Ace from 11 to 1, we see if the total falls below 22.

For example, with `['A', 'A', '9']`, initial total = `11 + 11 + 9 = 31`, ace_count = 2:
- First loop: `31 > 21 and 2 > 0` → `total = 21`, `ace_count = 1`
- Second loop: `21 > 21` is False, exit
- Return 21 (one Ace as 11, one as 1: 11 + 1 + 9 = 21)

---

#### 3.6.5.3 display_hand() – Format for Display

```python
def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)
```

**Data flow**: input `list[tuple[str, int]]` and `bool` → output `str`

**Line‑by‑line breakdown**:

| Line | What it does |
|------|--------------|
| `if hide_first` | If hiding the first card (dealer's hole card), display only the rest |
| `hand[1:]` | Slice: take from the second card onward (index 1 to end) |
| `' '.join(...)` | Joins multiple strings with a space between them |
| `f'{rank}'` | Converts rank to string (it already is, but this is safe) |

**Break down `join()`**:

`join()` is a string method – it concatenates an iterable of strings with the separator:

```python
>>> ' '.join(['A', '10', '5'])
'A 10 5'
```

**Default value of `hide_first`**:

`hide_first: bool = False` makes this parameter optional. You can call `display_hand(hand)` without passing `hide_first` – it will default to `False`. This is called a **default parameter** in function definitions; we'll cover it later.

---

#### 3.6.5.4 dealer_should_hit() – Should Dealer Hit?

```python
def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    return calculate_hand(hand) < 17
```

**Data flow**: input `list[tuple[str, int]]` → output `bool`

**Why a separate function?** This is good practice. The rule for the dealer (hit if total < 17) appears only once in the code. If it only appears once, you might think it doesn't need a separate function.

But encapsulating logic in a function gives two benefits:

1. **Readability**: reading `while dealer_should_hit(dealer_hand):` is easier to understand than `while calculate_hand(dealer_hand) < 17:` – the former reads like an English sentence "should the dealer hit?", while the latter forces you to parse the condition.
2. **Maintainability**: if the rule changes later (e.g., to < 16 or < 18), you only need to change this one function, not search through the main loop.

This is not directly about control flow, but writing good code is not just about using correct syntax – it also involves organising code to make logic clearer.

---

#### 3.6.5.5 play_blackjack() – Main Game Loop

This is the largest function and contains all the control flow structures. From a high level, its structure looks like:

```python
def play_blackjack() -> None:
    # Initialisation
    print(...)
    chips = 100
    
    while True:  # Main loop: each round
        # ---- Phase 1: Betting ----
        while True:  # Betting loop: ask until valid
            # ... input, validation, break ...
        
        # ---- Phase 2: Deal cards ----
        deck = create_deck()
        player_hand = [deck.pop(), deck.pop()]
        dealer_hand = [deck.pop(), deck.pop()]
        
        # ---- Phase 3: Player's turn ----
        while True:  # Player loop: hit until stand or bust
            # ... show hand, input, branching ...
            if action == 'hit':
                # draw a card
            elif action == 'stand':
                break
        
        # ---- Phase 4: Dealer's turn ----
        if not player_bust:
            while dealer_should_hit(dealer_hand):
                # dealer auto‑draws
        
        # ---- Phase 5: Determine winner ----
        if player_bust:
            # ...
        elif dealer_bust:
            # ...
        elif player_total > dealer_total:
            # ...
        # ...
        
        # ---- Phase 6: Check chips ----
        if chips <= 0:
            break
```

**How data flows between functions**:

```
create_deck()
    ↓ returns deck (list[tuple])
    
deck.pop() × 4
    ↓ cards taken to form player_hand / dealer_hand (list[tuple])
    
calculate_hand(player_hand)
    ↓ returns player_total (int)
    
display_hand(player_hand)
    ↓ returns formatted string (str)
    
dealer_should_hit(dealer_hand)
    ↓ returns True/False (bool) to control the while loop
```

**What does `deck.pop()` do?**

`pop()` is a list method – it removes and returns the last element of the list. Here `deck.pop()` is like "draw the top card from the deck".

```python
>>> cards = ['A', 'K', '7']
>>> cards.pop()
'7'
>>> cards
['A', 'K']
```

In Blackjack, `player_hand = [deck.pop(), deck.pop()]` draws two cards to form the player's initial hand. The first `pop()` takes the last card, the second takes the second‑last (the order doesn't matter because the deck is shuffled).

---

#### 3.6.5.6 if __name__ == "__main__"

```python
if __name__ == "__main__":
    play_blackjack()
```

This is a very common Python idiom.

- `__name__` is an automatic variable Python sets for each module
- When you run `python blackjack.py` directly, `__name__` is set to `"__main__"` – the condition is true, so `play_blackjack()` runs
- When you `import blackjack` in another file, `__name__` becomes `"blackjack"` (the module name) – the condition is false, so `play_blackjack()` does not run automatically

This allows the file to be used both as a standalone program **and** as a module that can be imported by other programs.

---

#### 3.6.5.7 Syntax in This Program Not Yet Covered

Looking back at the entire code, the following syntax/features have not been formally introduced yet:

| Syntax / Concept | Where it appears |
|------------------|------------------|
| Default parameters | `hide_first: bool = False` |
| List comprehension | `[(rank, values[rank]) for rank in ranks for _ in range(4)]` |
| Generator expression | `sum(value for _, value in hand)` |
| Tuple unpacking | `for _, value in hand` |
| `join()` method | `' '.join(...)` |
| `pop()` method | `deck.pop()` |
| Slicing | `hand[1:]` |
| `_` as placeholder | Ignoring values in loops |
| `if __name__ == "__main__"` | Program entry point |
| `None` type | Return type annotation |

Most of these will be covered systematically in the next chapter, "Data Structures". For now, the goal is: **when you see these constructs, you can guess what they mean – you don't need to write them yourself yet**.

---

#### 3.6.5.8 Summary: How to Write a Program Like This from Scratch

If you can't write this program yet, that's perfectly normal – it involves functions, data structures, loops, branches, I/O, and knowledge from multiple areas. It's not something you can produce immediately after learning a few syntax points.

Writing code is a process of **assembly**.

1. First, think clearly about the game rules → list the features you need (deal cards, calculate scores, display hands, decide winners)
2. Write each feature as a separate function → functions communicate via parameters and return values
3. Use the main loop to chain the functions together → forming a complete workflow

Don't rush to write a large program in one go. The goal of this lesson is to understand the role of control flow in a program. More details about functions and data structures (list, tuple, dict) will be covered in later sections.

---

## 3.7 What You've Accomplished in This Section

1. Learned to use `if` / `elif` / `else` for branching
2. Learned to use `for` to iterate over sequences (lists, dictionaries, ranges)
3. Learned to use `while` for conditional loops
4. Learned to use `break` to exit loops early and `continue` to skip the current iteration
5. Distinguished `for` (known iteration count) from `while` (unknown iteration count) by use case
6. Wrote your first complete interactive game, incorporating all control structures from this section
7. Learned about Python 3.9+'s built‑in generic syntax `list[tuple[str, int]]`, with no extra imports needed