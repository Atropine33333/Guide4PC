# Section 6: Data Structures

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Data structure | A way of organising and storing data |
| List (`list`) | An ordered, mutable sequence that can hold elements of any type |
| Tuple (`tuple`) | An ordered, immutable sequence – once created, it cannot be changed |
| Dictionary (`dict`) | A collection of key‑value pairs – fast lookup by key |
| Set (`set`) | An unordered collection of unique elements |
| Mutable | An object whose content can be modified after creation |
| Immutable | An object whose content cannot be modified after creation |
| Index | The position of an element in a sequence, starting from 0 |
| Key | The identifier used in a dictionary to look up a value |
| Value | The data associated with a key in a dictionary |
| Shallow copy | Copies only the outermost container; inner elements are shared by reference |
| Deep copy | Recursively copies all nested elements, producing a fully independent object |

---

## 6.1 Why Data Structures Are Needed

You've already used variables like `name = "MKJ"` and `age = 18`. But what if you need to store the names of 50 students in a class? Creating 50 separate variables is obviously impractical.

Data structures solve this problem – they let you store multiple values in a single variable and provide convenient ways to organise, access, and manipulate those values.

Python has four built‑in data structures, each suited for different use cases:

| Data Structure | Ordered? | Mutable? | Duplicates allowed? | Access method |
|----------------|----------|----------|---------------------|---------------|
| List (`list`) | Ordered | Mutable | Yes | By index |
| Tuple (`tuple`) | Ordered | Immutable | Yes | By index |
| Dictionary (`dict`) | Ordered (Python 3.7+) ("ordered" means insertion order is preserved, not that keys are sorted) | Mutable | Keys unique, values may repeat | By key |
| Set (`set`) | Unordered | Mutable | No | No index |

Before writing code, consider the scenario:

- Need to store multiple items in order and may later add/remove/change them → list
- Need to store multiple items in order but never want them changed → tuple
- Need to look up values by name/ID → dictionary
- Need to remove duplicates or perform set operations (union, intersection) → set

---

## 6.2 Lists (`list`)

### 6.2.1 Creating and Accessing

```python
# Empty list
empty = []

# Store elements (lists can mix types)
fruits = ["apple", "banana", "cherry"]      # uniform type
mixed = [1, "hello", 3.14, True]            # mixed types, not recommended

# Access by index (starting at 0)
fruits[0]   # "apple"
fruits[1]   # "banana"
fruits[-1]  # "cherry"  -1 means the last element
```

### 6.2.2 Modifying Lists

```python
fruits = ["apple", "banana", "cherry"]

# Append to the end
fruits.append("date")        # ["apple", "banana", "cherry", "date"]

# Insert at a specific position
fruits.insert(1, "blueberry")  # ["apple", "blueberry", "banana", "cherry", "date"]

# Modify an element at a specific position
fruits[2] = "blackberry"     # ["apple", "blueberry", "blackberry", "cherry", "date"]

# Remove an element at a specific position
removed = fruits.pop(0)      # removes and returns the first element
fruits.remove("cherry")      # removes the first matching element

# Clear the entire list
fruits.clear()               # []
```

### 6.2.3 Common Operations

```python
numbers = [3, 1, 4, 1, 5, 9, 2]

len(numbers)      # 7 – length
numbers.count(1)  # 2 – count of a specific element
numbers.index(9)  # 5 – first occurrence position
numbers.sort()    # sorts in place → [1, 1, 2, 3, 4, 5, 9]
numbers.reverse() # reverses in place → [9, 5, 4, 3, 2, 1, 1]

# Concatenate lists with +
[1, 2] + [3, 4]   # [1, 2, 3, 4]

# Repeat with *
[0] * 5           # [0, 0, 0, 0, 0]

# Check membership
"apple" in fruits    # True / False
"grape" not in fruits
```

### 6.2.4 Example: An Inventory System

```python
# Player's inventory
inventory = ["sword", "shield", "potion", "gold"]

# Pick up a new item
inventory.append("magic ring")
print(inventory)  # ['sword', 'shield', 'potion', 'gold', 'magic ring']

# Use an item (remove from inventory and consume)
potion = inventory.pop(2)  # take the potion at index 2
print(f"Used {potion}! Health restored.")
print(inventory)  # ['sword', 'shield', 'gold', 'magic ring']

# Check if an item is present
if "key" in inventory:
    print("You can open the door.")
else:
    print("You don't have the key.")
```

---

## 6.3 Tuples (`tuple`)

### 6.3.1 Creating and Accessing

```python
# Create
empty = ()
single = (5,)        # note: single‑element tuple needs a trailing comma
coordinates = (10, 20)
colors = ("red", "green", "blue")

# Access (same as lists)
coordinates[0]   # 10
coordinates[1]   # 20
```

### 6.3.2 Immutability

```python
point = (3, 5)
point[0] = 4      # TypeError: 'tuple' object does not support item assignment
```

Once created, you cannot modify, delete, or add elements.

### 6.3.3 When to Use Tuples

**The only difference between tuples and lists is that tuples are immutable and lists are mutable.**

- Coordinates `(x, y)`: if the coordinate changes, create a new coordinate rather than modifying the old one
- RGB colour `(255, 128, 64)`: the colour itself is fixed
- Multiple return values from a function: effectively tuples
- Dictionary keys (lists cannot be keys; tuples can)

```python
# Tuples can be used as dictionary keys
game_moves = {
    (0, 0): "start",
    (1, 3): "treasure",
    (2, 5): "boss"
}
```

The choice is simple:

- If the data needs to be modified → list
- If the data does not need to be modified → tuple (uses less memory, safer code)

---

## 6.4 Dictionaries (`dict`)

### 6.4.1 Creating and Accessing

```python
# Empty dictionary
empty = {}

# Create
player = {
    "name": "MKJ",
    "level": 5,
    "hp": 100,
    "skills": ["fireball", "heal"]
}

# Access by key
player["name"]      # "MKJ"
player["level"]     # 5

# Safe access with get() (returns None or a default if key missing)
player.get("mana")           # None
player.get("mana", 0)        # 0 (default value)
```

### 6.4.2 Modifying Dictionaries

```python
# Add or modify key‑value pairs
player["hp"] = 80              # modify
player["mana"] = 50            # add

# Delete key‑value pairs
del player["skills"]           # delete the entire key‑value pair
removed = player.pop("level")  # delete and return the value (5)
removed = player.pop("xxx", 0) # if key missing, return default

# Get all keys, values, or items
player.keys()      # dict_keys(['name', 'hp', 'mana'])
player.values()    # dict_values(['MKJ', 80, 50])
player.items()     # dict_items([('name', 'MKJ'), ('hp', 80), ('mana', 50)])

# Check if a key exists
if "hp" in player:
    print(f"HP: {player['hp']}")
```

### 6.4.3 Iterating Over Dictionaries

```python
# Iterate over keys
for key in player:
    print(key)

# Iterate over key‑value pairs (most common)
for key, value in player.items():
    print(f"{key}: {value}")

# Iterate over keys and values separately with .keys() and .values()
```

### 6.4.4 Example: Skill Cooldown Management

```python
# skill name → cooldown (in turns)
cooldowns = {
    "fireball": 0,
    "heal": 2,
    "shield": 5,
    "ultimate": 10
}

# Decrease cooldowns by 1 each turn
def tick_cooldowns(cooldowns):
    for skill in list(cooldowns.keys()):  # make a copy to avoid mutation during iteration
        if cooldowns[skill] > 0:
            cooldowns[skill] -= 1
    return cooldowns

# Use a skill (set its cooldown)
def use_skill(skill):
    if cooldowns[skill] == 0:
        print(f"✨ Using {skill}!")
        cooldowns[skill] = cooldowns.get(skill, 0) + 3
    else:
        print(f"⏳ {skill} is on cooldown for {cooldowns[skill]} more turns.")
```

Why `list(cooldowns.keys())`: you cannot change the size of a dictionary (add or remove keys) while iterating over it. However, you can modify existing values. `list(cooldowns.keys())` creates a copy of the key list, so any changes to the dictionary during iteration do not affect the copy.

---

## 6.5 Sets (`set`)

### 6.5.1 Creating and Basic Operations

```python
# Create
empty = set()           # cannot use {}, because that would be an empty dict
numbers = {1, 2, 3, 4, 5}
letters = {"a", "b", "c"}

# Automatic deduplication (key feature of sets)
duplicates = {1, 2, 2, 3, 3, 3}   # {1, 2, 3}

# Add
numbers.add(6)          # {1, 2, 3, 4, 5, 6}

# Remove
numbers.remove(3)       # removes if exists, raises error otherwise
numbers.discard(10)     # removes if exists, does nothing otherwise

# Pop (remove and return an arbitrary element)
numbers.pop()           # because sets are unordered, pop removes a random element
```

### 6.5.2 Set Operations

```python
a = {1, 2, 3, 4}
b = {3, 4, 5, 6}

# Union (all elements)
a | b           # {1, 2, 3, 4, 5, 6}
a.union(b)      # same

# Intersection (common elements)
a & b           # {3, 4}
a.intersection(b)

# Difference (in a but not in b)
a - b           # {1, 2}
a.difference(b)

# Symmetric difference (in a or b but not both)
a ^ b           # {1, 2, 5, 6}
a.symmetric_difference(b)

# Subset / superset checks
{1, 2}.issubset(a)      # True
{1, 2, 3, 4}.issuperset({1, 2})  # True
```

### 6.5.3 Example: Friend Relationship Analysis

```python
MKJ_friends = {"YZY", "WYT", "WYH", "LYZ"}
YZY_friends = {"MKJ", "WYT", "TYZ", "MFY"}

# Mutual friends
mutual = MKJ_friends & YZY_friends
print(f"Mutual friends: {mutual}")  # {'WYT'}

# Total unique friends
all_friends = MKJ_friends | YZY_friends
print(f"Total unique friends: {len(all_friends)}")  # 6

# Friends MKJ has but YZY does not
only_MKJ = MKJ_friends - YZY_friends
print(f"Only MKJ has: {only_MKJ}")  # {'WYH', 'LYZ'}

# Check friendship
def are_friends(person1, person2):
    return person2 in person1_friends
```

---

## 6.6 Mutable vs Immutable

### 6.6.1 Which Types Are Mutable

| Mutable | Immutable |
|---------|-----------|
| List (`list`) | Tuple (`tuple`) |
| Dictionary (`dict`) | String (`str`) |
| Set (`set`) | Integer (`int`) |
| Custom objects | Float (`float`) |
| | Boolean (`bool`) |

### 6.6.2 Why This Matters

When you pass a list to a function:

```python
def add_item(inventory, item):
    inventory.append(item)   # modifies the external list directly
    return inventory

my_inv = ["sword", "shield"]
add_item(my_inv, "potion")
print(my_inv)   # ['sword', 'shield', 'potion'] – it was modified
```

If you don't want the function to modify the list, pass a copy:

```python
def add_item(inventory, item):
    inventory = inventory[:]   # make a copy before operating
    inventory.append(item)
    return inventory
```

Or pass a tuple (immutable, so it cannot be modified):

```python
def add_item(inventory_tuple, item):
    # inventory_tuple.append(item)  # would raise an error
    return inventory_tuple + (item,)  # returns a new tuple
```

Remember this distinction: in Python, assignment for mutable types shares the reference; for immutable types, it assigns a new value to the variable.

---

## 6.7 Shallow and Deep Copy (Important!)

This is one of the most overlooked pitfalls in data structures.

### 6.7.1 Assignment Does Not Copy

```python
a = [1, 2, 3]
b = a           # b and a point to the same object!
b.append(4)
print(a)        # [1, 2, 3, 4] – a was also modified
```

Here `b = a` does not copy the list – it only copies the reference. Both variables point to the same memory.

### 6.7.2 Shallow Copy

```python
# Method 1: slicing
b = a[:]

# Method 2: list() constructor
b = list(a)

# Method 3: copy module
import copy
b = copy.copy(a)
```

A shallow copy copies the outermost container, but inner elements are still shared references.

```python
a = [[1, 2], [3, 4]]
b = a[:]          # shallow copy
b[0].append(5)    # modify the inner list b[0]
print(a)          # [[1, 2, 5], [3, 4]] – a[0] was also modified!
```

Here `b[0]` and `a[0]` point to the same inner list. Shallow copy only copied the outer list.

### 6.7.3 Deep Copy

```python
import copy
a = [[1, 2], [3, 4]]
b = copy.deepcopy(a)   # complete copy
b[0].append(5)
print(a)          # [[1, 2], [3, 4]] – a remained unchanged
```

Deep copy recursively copies all nested elements, creating a fully independent object.

### 6.7.4 When to Watch Out

```python
# Scenario 1: dictionary values are lists
player = {
    "name": "MKJ",
    "inventory": ["sword", "shield"]
}

# If you copy this dictionary without deep copy, the inventory is shared
backup = player.copy()           # shallow copy
backup["inventory"].append("potion")
print(player["inventory"])       # ['sword', 'shield', 'potion'] – modified

# Use deep copy
backup = copy.deepcopy(player)
backup["inventory"].append("ring")
print(player["inventory"])       # ['sword', 'shield', 'potion'] – unchanged
```

### 6.7.5 Example: Game Saving

```python
import copy

# Current game state
save = {
    "player": {
        "name": "MKJ",
        "hp": 85,
        "position": (5, 12)
    },
    "items": ["sword", "shield", "potion"],
    "quests_completed": ["slime", "goblin"]
}

# Quick save (shallow copy pitfall)
quick_save = save.copy()           # shallow copy
quick_save["player"]["hp"] = 100   # modify the quick save
print(save["player"]["hp"])        # 100 – main save was also modified!

# Correct quick save
def save_game(state):
    return copy.deepcopy(state)

def load_game(state):
    return copy.deepcopy(state)

quick_save = save_game(save)
quick_save["player"]["hp"] = 100
print(save["player"]["hp"])        # 85 – main save remains unchanged
```

---

## 6.8 What You've Learned

This section covered Python's four built‑in data structures and the important concepts behind them. Let's review what you've learned:

**Lists (`list`)**
- Ordered, mutable, allow duplicates
- Basic operations: `append()`, `insert()`, `pop()`, `remove()`, `sort()`, `reverse()`
- Access by index, slicing for subsequences
- Use case: need to store ordered data and later add/remove/change items

**Tuples (`tuple`)**
- Ordered, immutable, allow duplicates
- Cannot be modified after creation, but access is the same as lists
- Use case: coordinates, colour values, multiple return values from functions, dictionary keys

**Dictionaries (`dict`)**
- Key‑value storage – keys are unique and immutable, values can be anything
- Fast lookup by key, O(1) average time
- Common operations: `dict[key]`, `.get()`, `.keys()`, `.values()`, `.items()`
- Use case: need to look up data by name/ID

**Sets (`set`)**
- Unordered, mutable, no duplicates
- Automatic deduplication, set operations: union `|`, intersection `&`, difference `-`, symmetric difference `^`
- Use case: removing duplicates, friend relationship analysis, membership tests

**Shallow vs Deep Copy**
- Assignment does not copy – only copies the reference
- Shallow copy copies the outer container; inner elements are still shared
- Deep copy recursively copies all levels, producing a fully independent object
- Use `copy.deepcopy()` and import the `copy` module

**Mutable vs Immutable**
- Mutable types: `list`, `dict`, `set`
- Immutable types: `int`, `float`, `str`, `bool`, `tuple`
- When mutable types are passed as function arguments, modifications inside the function affect the outside

**Selection Guide**

| You need to… | Use |
|--------------|-----|
| Store ordered data that may be modified | List |
| Store ordered data that will not be modified | Tuple |
| Look up values by name/key | Dictionary |
| Deduplicate or perform set operations | Set |
| Fully copy a nested structure | Deep copy |