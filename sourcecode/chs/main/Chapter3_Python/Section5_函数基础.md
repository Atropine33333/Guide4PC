# 第5节：函数

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 函数 | 组织好的、可重复使用的代码块，完成一个特定功能 |
| 定义 | 用 `def` 创建函数的过程 |
| 调用 | 执行一个已经定义的函数 |
| 参数 | 调用函数时传给函数的值 |
| 返回值 | 函数执行完后返回给调用者的结果 |
| 位置参数 | 按位置顺序传入的参数 |
| 关键字参数 | 用 `key=value` 形式传入的参数 |
| 默认参数 | 定义时给参数设定的默认值，调用时可省略 |
| 局部变量 | 在函数内部定义的变量，只在函数内生效 |
| 全局变量 | 在函数外部定义的变量，整个文件都能访问 |
| 作用域 | 变量可以被访问的范围 |

---

## 5.1 为什么需要函数

你已经写过代码。到目前为止，所有代码都是线性的——从第一行跑到最后一行，中间加上一些分支和循环。

当然，那个21点的程序不算，那个我自认为是一个比较规范的单文件程序，各个内容都用函数包裹起来了，即便是不太需要复用的内容为了规范也包起来了。

那么当代码超过几十行，问题就来了：

- 同样的逻辑要在多个地方出现，只能复制粘贴
- 复制粘贴的代码改一处，别处忘了改，产生 bug
- 代码越长，读起来越费劲，不知道哪里是哪里
- 想测试某一块功能，必须跑整个程序

**函数解决的就是这些问题**。

函数是一段**有名字的代码块**。它有几个特点：

- 可以**重复使用**：定义一次，调用多次
- 可以**接收输入**：通过参数传入数据
- 可以**返回结果**：通过 `return` 把数据传回给调用者
- 可以**独立测试**：单独运行一个函数检查它是否正确

---

## 5.2 最简单的函数

先看一个极其简单的例子：

```python
def say_hello() -> None:
    """Print a greeting message."""
    print("Hello, world!")
```

拆开来看：

- `def`：关键字，表示“我要定义一个函数了”
- `say_hello`：函数名，你起的名字
- `()`：括号里放参数，现在为空
- `-> None`：返回类型提示，表示这个函数不返回有意义的值
- `:`：冒号，表示函数体要开始了
- 缩进的代码块：函数体，函数被调用时执行这里的代码

定义完之后，要使用它：

```python
say_hello()   # 调用函数
```

输出：

```
Hello, world!
```

这个函数是最简单的一种——没有参数，没有返回值，只做一件事（打印）。

**什么时候用这种函数**：当某段打印逻辑需要在多处出现时。比如程序启动时打印一个 Logo，结束时打印一个感谢信息，把打印代码放在函数里，每处调用一句就行。

---

## 5.3 带参数的函数

大多数函数需要输入数据。

```python
def greet(name: str) -> None:
    """Print a personalized greeting."""
    print(f"Hello, {name}!")
```

> 再次提醒，`: str` 这个东西是类型注解，可以不写，写上会让你更好理解这个代码。

调用时传入一个参数：

```python
greet("MKJ")
greet("YZY")
```

输出：

```
Hello, MKJ!
Hello, YZY!
```

**参数的本质**：参数就是变量，只不过它的值由调用者传入。定义函数时 `name: str` 只是接收一个名字，调用时 `"MKJ"` 传给 `name`。

多个参数用逗号分隔：

```python
def greet_with_title(name: str, title: str) -> None:
    """Print a greeting with a title."""
    print(f"Good morning, {title} {name}!")

greet_with_title("MKJ", "Dr.")
greet_with_title("YZY", "Mr.")
```

输出：

```
Good morning, Dr. MKJ!
Good morning, Mr. YZY!
```

参数的顺序很重要。调用时 `"MKJ"` 对应 `name`，`"Dr."` 对应 `title`。这叫**位置参数**。

---

## 5.4 带返回值的函数

打印出来对后续逻辑没用。你需要把计算结果交还给调用者。

```python
def add(a: int, b: int) -> int:
    """Return the sum of two numbers."""
    result = a + b
    return result
```

调用：

```python
x = add(3, 5)
print(x)   # 8
```

`return` 做了两件事：
1. 把值传回给调用者
2. 立即结束函数执行（`return` 后面的代码不会执行）

可以一次返回多个值：

```python
def get_min_max(numbers: list[int]) -> tuple[int, int]:
    """Return the minimum and maximum values from a list."""
    return min(numbers), max(numbers)

minimum, maximum = get_min_max([3, 1, 7, 4, 9])
print(minimum, maximum)   # 1 9
```

这里返回了两个值，在调用处用元组解包接住。

---

## 5.5 什么时候用 `return`，什么时候用 `print`

```python
# 版本1：用 print
def add_print(a, b):
    print(a + b)

# 版本2：用 return
def add_return(a, b):
    return a + b
```

区别在这里：

```python
# 版本1：打印了结果，但程序没法用这个值
x = add_print(3, 5)   # 打印 8，然后 x 的值是 None

# 版本2：没打印任何东西，但程序拿到了结果
x = add_return(3, 5)   # 什么也不打印，x = 8
y = x + 2              # y = 10，还能继续用
```

规则很简单：

- 如果这个值**后续还要用**（做运算、存文件、当别的函数的参数）→ 用 `return`
- 如果这个值**只需要给人看一眼**（调试、输出报告）→ 用 `print`

---

## 5.6 默认参数

有时候同一个参数在大部分情况下是同一个值，只有少数情况需要改。这时候可以给参数设定一个默认值。

```python
def attack(enemy: str, damage: int, critical_hit: bool = False) -> None:
    print(f"Attacking {enemy}!")
    if critical_hit:
        print(f"CRITICAL HIT! {damage * 2} damage!")
    else:
        print(f"{damage} damage.")

# 调用时可以不传第三个参数
attack("Goblin", 30)
# Attacking Goblin!
# 30 damage.

# 需要暴击时显式传入
attack("Dragon", 50, critical_hit=True)
# Attacking Dragon!
# CRITICAL HIT! 100 damage!
```

有默认值的参数放在最后面。

```python
# 正确
def attack(enemy: str, damage: int, critical_hit: bool = False) -> None:
    ...

# 错误 —— 无默认值的参数不能放在有默认值的后面
def attack(enemy: str, critical_hit: bool = False, damage: int) -> None:
    ...
```

---

## 5.7 关键字参数

之前调用函数时，传入参数的顺序必须和定义时的顺序一致：

```python
def greet(name: str, title: str) -> None:
    print(f"Hello, {title} {name}!")

greet("MKJ", "Dr.")   # OK
greet("Dr.", "MKJ")   # 逻辑错误，但语法正确
```

如果参数很多，很容易传错顺序。**关键字参数**通过指定参数名来传入，不依赖顺序：

```python
greet(name="MKJ", title="Dr.")   # OK
greet(title="Dr.", name="MKJ")   # 也 OK，顺序无关
```

位置参数和关键字参数可以混用，但**位置参数必须在关键字参数之前**：

```python
def display_profile(name: str, age: int, city: str, job: str) -> None:
    print(f"{name}, {age}, {city}, {job}")

# 前两个用位置，后两个用关键字
display_profile("MKJ", 22, job="Student", city="Beijing")
```

---

## 5.8 作用域：变量在哪儿生效

函数内部的变量和函数外部的变量生活在不同的世界里。

```python
x = 10   # 全局变量

def test() -> None:
    y = 5  # 局部变量，只在这个函数里有效
    print(x)   # 可以读取全局变量
    print(y)

test()
print(x)   # 10，全局变量还在
print(y)   # NameError: y is not defined —— 访问不到函数内部的 y
```

**全局变量**（定义在函数外面的）在函数里可以读取，但不能直接修改：

```python
x = 10

def test() -> None:
    x = 20   # 这是在函数里创建了一个新的局部变量 x，和全局的 x 没关系
    print(x) # 20

test()
print(x)   # 10 —— 全局 x 没变
```

如果确实需要在函数内部修改全局变量，用 `global` 关键字（但不推荐，尽量别这么做）：

```python
x = 10

def test() -> None:
    global x
    x = 20

test()
print(x)   # 20 —— 这次真的改了
```

**为什么函数内部的变量独立**：

- 每个函数像一个小房间，房间里的东西（局部变量）只有房间里面的人能用
- 全局变量像是房子大厅里的东西，所有房间都能看到
- 你在房间里往桌子上放东西（局部变量），不会改变大厅里的东西

这样设计的好处是：函数之间不会互相干扰。你写一个函数时，不用担心它不小心改掉了别的函数使用的变量。

---

## 5.9 综合例子：RPG 战斗系统

把前面所有东西串起来。一个模拟 RPG 战斗的小游戏。

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

这个程序中有这些函数：
- `roll_dice()`：无参数有返回值，带默认参数
- `calculate_damage()`：多参数有返回值，演示公式计算
- `player_turn()`：有参数有返回值，返回多个值（元组）
- `enemy_turn()`：有参数有返回值，与 player_turn 对称
- `combat()`：大函数，调用小函数，主循环，返回布尔值
- `main()`：顶层控制，管理数据，循环调用 combat

各个函数各司其职，每个函数做的事情不超过一件事。这就是“函数”的核心思想：**把大问题拆成小问题，每个小问题交给一个函数解决**。

### 5.9.1 代码分析：RPG 战斗系统的函数拆解

上一节的 RPG 战斗系统比 21 点多了好几层函数调用。现在仔细分析每个函数的作用、数据流、以及新出现的语法。

#### 5.9.1.1 roll_dice() —— 最简单的工具函数

```python
def roll_dice(sides: int = 20) -> int:
    """Roll a dice with the given number of sides."""
    return random.randint(1, sides)
```

**数据流**：输入 `int`（面数，可选）→ 输出 `int`（随机数）

**职责**：封装随机数生成逻辑。随机数在战斗系统里用于命中判定和暴击判定，抽离成独立函数后：
- 调用方不需要关心 `random.randint()` 的用法
- 如果要修改骰子算法（比如换成加权随机），只需改这一个地方
- 方便测试——后续可以用固定返回值替代真实随机（此处未实现）

**新语法**：之前已经见过默认参数，这里再次出现。`sides: int = 20` 表示调用 `roll_dice()` 时如果不传参数，默认掷 20 面骰。

#### 5.9.1.2 calculate_damage() —— 纯计算函数

```python
def calculate_damage(attack: int, defense: int, critical: bool = False) -> int:
    base_damage = attack * (100 / (100 + defense))
    damage = int(base_damage)
    if critical:
        damage = int(damage * 1.5)
    return max(damage, 1)
```

**数据流**：输入 `attack`, `defense`, `critical` → 输出 `int`（伤害值）

**职责**：只做数学计算，不打印任何内容。这是典型的**纯函数**（pure function）——同样的输入永远产生同样的输出，没有副作用（不修改外部状态，不打印）。

**设计理念**：战斗系统的伤害公式可能会频繁调整。把它放在一个独立的函数里，调平衡时只需要改公式，调用方完全不受影响。这样的代码更容易维护。

**新语法**：`max(damage, 1)` 是 Python 内置函数，返回两个参数中较大的一个。这里确保伤害值至少为 1，避免攻击力太低时打出 0 伤害。

#### 5.9.1.3 player_turn() —— 返回多个值的函数

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

**数据流**：输入 `enemy_name`, `enemy_hp`, `enemy_defense` → 输出 `tuple[int, bool]`（伤害值，是否暴击）

**职责**：封装一个完整的玩家回合——掷骰判定是否命中、是否暴击、计算伤害、打印战斗信息。

**新语法**：这里出现了**返回多个值**。`return damage, is_critical` 实际上返回的是一个元组 `(damage, is_critical)`。调用方用 `damage, _ = player_turn(...)` 接住。

**一个疑问**：参数里有 `enemy_hp`，但函数内部并没有用到它。那么为什么传进来？

因为当前设计下，玩家回合只需要敌方的防御力来计算伤害，不需要知道敌方当前血量。打印时也不显示敌方血量变化——只在 `combat()` 里显示总数。`enemy_hp` 传入但未使用，属于“预留参数”，可能后续扩展用。实际编码中应该避免这种冗余，更干净的写法是删除它。但这里保留是为了展示：**函数接收的参数不一定全都用上，参数列表应该只包含需要的值**。

嘛其实我就是嘴硬，我设计时候有这个参数，但是后来觉得没必要，懒得去掉了。

你可以修改成这样

```python
def player_turn(enemy_name: str, enemy_defense: int) -> tuple[int, bool]:
    # 不再需要 enemy_hp
```

类似的，在 `combat` 中的参数也可以修改为
```python
def combat(player_hp: int, player_defense: int, enemy_name: str, enemy_defense: int) -> bool:
    # 省略前面部分
    damage, _ = player_turn(enemy_name, enemy_defense)
```

不妨尝试阅读原代码，找到冗余的部分并尝试去除。这是一个。

#### 5.9.1.4 enemy_turn() —— 对称设计

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

**数据流**：输入 `player_hp`, `player_defense` → 输出 `int`（伤害值）

**职责**：和 `player_turn()` 对称，封装敌人回合。敌人永远命中（不需要掷命中骰），但同样有 5% 概率暴击（掷出 20）。

**设计理念**：玩家和敌人的战斗逻辑被分别封装在两个函数里。它们的接口不同（玩家需要传入敌方防御，敌人需要传入玩家防御），但返回值的类型一致——都是 `int` 表示造成的伤害。这使得 `combat()` 可以用同样的方式处理双方的伤害结算。

#### 5.9.1.5 combat() —— 主战斗循环

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

**数据流**：输入双方所有状态 → 输出 `bool`（True=玩家获胜，False=玩家失败）

**职责**：编排战斗流程。不关心具体的伤害计算逻辑（交给 `calculate_damage()`），不关心回合内部的细节（交给 `player_turn()` 和 `enemy_turn()`）。它只负责：
1. 打印战斗开始信息
2. 轮次循环控制
3. 更新血量
4. 判定胜负并返回

**这一节的函数调用链**：

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

`main()` 调用 `combat()`，`combat()` 调用 `player_turn()` 和 `enemy_turn()`，这两个又调用更底层的 `roll_dice()` 和 `calculate_damage()`。每一层函数只和下一层打交道，不越级。

**为什么这样设计**：主循环 `combat()` 不需要知道“骰子是怎么掷的”或“伤害公式是什么”。它只需要知道“玩家回合造成多少伤害”和“敌人回合造成多少伤害”。这种分层让每层代码都保持简单，修改底层函数不会影响上层逻辑。

#### 5.9.1.6 main() —— 顶层控制

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

**数据流**：无输入 → 无输出（`-> None`）

**职责**：游戏的总控制台。管理玩家状态、敌人列表、闯关流程、分数统计。

**新语法**：

**`for...else`**：`for` 循环后面的 `else` 子句，在循环**正常结束**（没有被 `break` 中断）时执行。在这里：

```python
for enemy in enemies:
    won = combat(...)
    if not won:
        break  # 玩家输了，中断循环
else:
    # 循环正常结束，没有被 break —— 说明所有敌人都打完了
    print("You are the champion!")
```

如果玩家输给某个敌人，`break` 触发，跳过 `else`。如果打完全部敌人没有触发 `break`，执行 `else`。这是 Python 特有的写法。

**`input("Press Enter to fight...")`**：这里调用了 `input()` 但没有把返回值赋给任何变量。作用仅仅是暂停程序，等待玩家按 Enter 继续。输入的内容被丢弃了。

#### 5.9.1.7 这一节出现的函数相关语法汇总

| 语法 | 示例 | 是否已讲 |
|------|------|----------|
| 函数定义 | `def roll_dice():` | 已讲 |
| 位置参数 | `roll_dice(20)` | 已讲 |
| 默认参数 | `sides: int = 20` | 已讲 |
| 关键字参数 | `calculate_damage(critical=True)` | 已讲 |
| 返回值 | `return damage` | 已讲 |
| 多返回值 | `return damage, is_critical` | 已讲 |
| 类型注解 | `-> tuple[int, bool]` | 已讲 |
| 局部变量 | `round_num`、`hit_roll` | 已讲 |
| 全局变量 | 未使用 `global` | 已讲 |
| `max()` 内置函数 | `max(damage, 1)` | 新出现 |
| `for...else` | `for enemy in enemies: ... else: ...` | 新出现 |
| `input()` 做暂停 | `input("Press Enter...")` | 新出现 |

---

#### 5.9.1.8 这一节的函数设计原则总结

1. **单一职责**：每个函数只做一件事。`roll_dice()` 只掷骰子，`calculate_damage()` 只算伤害，`player_turn()` 只处理玩家回合。

2. **数据流清晰**：函数的输入（参数）和输出（返回值）明确。`combat()` 通过参数接收所有状态，通过返回值告诉调用者胜负。

3. **避免副作用**：`calculate_damage()` 是纯函数，不修改任何外部状态，只返回计算结果。这使它可以被独立测试。

4. **分层调用**：`main()` 调 `combat()`，`combat()` 调 `player_turn()` 和 `enemy_turn()`，底层调 `roll_dice()` 和 `calculate_damage()`。每层只和相邻层打交道。

5. **返回值传达状态**：`combat()` 返回 `bool`，`player_turn()` 返回 `(damage, is_critical)`。调用方通过检查返回值来决定下一步做什么。

6. **重复逻辑抽离**：玩家和敌人都用同一个 `calculate_damage()` 计算伤害，同一个 `roll_dice()` 掷骰子。避免在两个函数里写两遍同样的公式。

---

## 5.10 本节你做了什么

1. 理解了函数是什么以及为什么需要它
2. 学会了用 `def` 定义函数
3. 学会了给函数传参数（位置参数、关键字参数）
4. 学会了用 `return` 返回值
5. 理解了 `print` 和 `return` 的区别
6. 学会了设置默认参数
7. 理解了局部变量和全局变量的区别及其作用域
8. 写了一个完整的 RPG 战斗系统，涵盖你所学的一切
