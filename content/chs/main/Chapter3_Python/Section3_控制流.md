# 第3节：控制流

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 条件判断 | 根据条件是否为真来决定执行哪一段代码 |
| 分支 | `if`、`elif`、`else` 构成的不同执行路径 |
| 代码块 | 缩进相同的若干行代码，被视为一个整体 |
| 循环 | 重复执行同一段代码 |
| 迭代 | 逐个取出序列中的元素进行处理 |
| 遍历 | 和迭代同义，指按顺序访问容器中的每个元素 |
| 嵌套 | 在循环或分支内部再写循环或分支 |
| 中断 | `break`，提前终止循环 |
| 跳过 | `continue`，跳过当前迭代的剩余部分，进入下一次 |
| 布尔表达式 | 结果为 `True` 或 `False` 的表达式 |

---

## 3.1 控制流是什么

控制流就是**代码执行的顺序**。

在没有任何控制结构的情况下，代码是逐行执行的——第一行跑完跑第二行，第二行跑完跑第三行。这种方式只能写线性脚本，做不了复杂的事情。

你需要三种能力来让程序“活”起来：

1. **分支**：根据条件走不同的路（`if` / `elif` / `else`）
2. **循环**：重复执行某段代码（`for` / `while`）
3. **跳转**：在循环内部提前跳出或跳过（`break` / `continue`）

三种控制结构是任何编程语言的核心，Python 也不例外。

---

## 3.2 分支：if / elif / else

`if` 语句根据条件决定是否执行某段代码。基本语法：

```python
if condition:
    # do something when condition is True
```

如果有多个分支，用 `elif`（`else if` 的缩写）和 `else`：

```python
if condition1:
    # runs when condition1 is True
elif condition2:
    # runs when condition1 is False and condition2 is True
else:
    # runs when all conditions above are False
```

实际例子：

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

`elif` 可以有多个，按顺序从上到下检查，遇到第一个为 `True` 的就执行对应的代码块，执行完后直接跳出整个 `if-elif-else` 结构，不再检查后面的条件。`else` 最多只能有一个，放在最后面。

---

## 3.3 循环：for

`for` 循环用于**遍历**一个序列（列表、元组、字符串、range 等）。它逐个取出序列中的元素，每取一个执行一次循环体。

```python
fruits = ["apple", "banana", "cherry"]
for fruit in fruits:
    print(fruit)
```

`range()` 是 `for` 循环最常用的搭档。它生成一个整数序列：

```python
# range(stop): 0, 1, 2, ..., stop-1
for i in range(5):
    print(i)   # 0 1 2 3 4

# range(start, stop): start, start+1, ..., stop-1
for i in range(2, 6):
    print(i)   # 2 3 4 5

# range(start, stop, step): 按步长递增
for i in range(0, 10, 2):
    print(i)   # 0 2 4 6 8
```

遍历列表时，如果既需要位置又需要元素，用 `enumerate()`：

```python
fruits = ["apple", "banana", "cherry"]
for idx, fruit in enumerate(fruits):
    print(f"{idx}: {fruit}")
```

输出：

```
0: apple
1: banana
2: cherry
```

> 介绍一下enumerate()：同时获取索引和值
> 
> 在 `for` 循环中遍历列表时，经常需要同时知道**当前元素的值**和**它的位置（索引）**。`enumerate()` 就是为此而生的。
> 
> `enumerate()` 返回一个迭代器，每次产生一个包含**索引**和**值**的元组。用 `i, fruit` 这样的解包语法可以直接拆开使用。
> 
> **指定起始索引**：
> 
> 默认从 `0` 开始。如果想从 `1` 开始，加 `start` 参数：
> 
> ```python
> for i, fruit in enumerate(fruits, start=1):
>     print(f"{i}: {fruit}")
> ```
> 
> 输出：
> 
> ```
> 1: apple
> 2: banana
> 3: cherry
> ```
> 
> **常见场景**：
> 
> - 打印带行号的数据
> - 在循环中需要知道当前处理到第几个元素
> - 查找某个元素的位置时，和 `if` 配合使用
> 
> ```python
> # 找第一个以 'b' 开头的元素及其位置
> for i, fruit in enumerate(fruits):
>     if fruit.startswith("b"):
>         print(f"Found '{fruit}' at index {i}")
>         break
> ```


遍历字典时：

```python
student = {"name": "MKJ", "age": 18, "major": "Phys"}
for key in student:           # Iterate over key
    print(key)
for key, value in student.items():  # Iterate over both key and value
    print(f"{key} = {value}")
```

---

## 3.4 循环：while

`while` 循环在**条件为真**时持续执行循环体。它和 `for` 的区别在于：

- `for` 知道要迭代多少次（序列有多长）
- `while` 不知道，需要你自己控制循环何时结束

```python
count = 0
while count < 5:
    print(count)
    count += 1   # must update manually, otherwise infinite loop
```

如果忘记更新条件中的变量，循环永远不会结束：

```python
count = 0
while count < 5:
    print(count)   # infinite loop, count is always 0
```

`while` 适合“不知道要循环多少次，只知道什么时候该停”的场景，比如等待用户输入特定的命令。

---

## 3.5 循环控制：break 和 continue

`break`：立即结束整个循环，跳到循环后面的代码。

```python
for i in range(10):
    if i == 5:
        break
    print(i)   # prints 0 1 2 3 4
```

`continue`：跳过当前这一次迭代的剩余部分，直接进入下一次迭代。

```python
for i in range(10):
    if i % 2 == 0:
        continue
    print(i)   # prints 1 3 5 7 9 (skips even numbers)
```

`break` 和 `continue` 在 `for` 和 `while` 中都可以用。

---

## 3.6 综合实战：21点（Blackjack）

### 3.6.1 游戏规则（简化版）

- 使用一副扑克牌，只取点数 2-10、J、Q、K、A
- J、Q、K 算 10 点，A 算 1 点或 11 点（取对玩家有利的值）
- 玩家和庄家（电脑）各发两张牌，庄家第一张牌公开，第二张隐藏
- 玩家可以选择“要牌”（Hit）或“停牌”（Stand）
- 如果玩家点数超过 21，直接爆牌输掉
- 玩家停牌后，庄家自动要牌直到点数达到 17 或以上
- 比较双方点数，更接近 21 且不超过 21 的获胜

### 3.6.2 完整代码

**!预警**这份代码为了可以运行起来使用了一些未曾讲过的用法，比如 `.pop()` 方法调用， `' '.join(...)` 链式调用， `hand[1:]` 切片， `sum(value for _, value in hand)` 生成器表达式之类的。看不懂请先跳过阅读代码部分，复制代码，在 VS Code 中新建 BlackJack.py 文件，粘贴代码后，按照第一节1.7的指引激活 venv ，输入 py BlackJack.py 玩上一两把21点，觉得压力小一些了，再来对照3.6.5拆解这份代码，你会感觉容易一些。

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

### 3.6.3 import 说明

代码顶部只用了一行 import：

```python
import random
```

`random` 是 Python 标准库，提供了 `shuffle()` 函数来洗牌。注意这里**没有 import `List` 或 `Tuple`**，因为代码里直接用了 Python 3.9+ 的内置泛型写法 `list[tuple[str, int]]`，不需要额外导入任何类型。

**关于 `list[tuple[str, int]]`**：

`list[tuple[str, int]]` 是 Python 3.9 引入的类型注解语法，表示“一个列表，里面的每个元素都是一个元组，元组里第一个元素是字符串，第二个是整数”。它实际上是 `list` 类型的泛型形式——说明“这个列表里装的是什么类型的东西”。

标准库 `typing` 中也有 `List` 和 `Tuple`（首字母大写），但在 Python 3.9+ 中已被内置的 `list` 和 `tuple` 取代。直接用内置写法即可，不需要额外 import。

### 3.6.4 代码分析：控制流在哪里

这个游戏把本节讲的所有控制流结构都用上了。对照着看：

**分支结构（if / elif / else）**

- `calculate_hand()` 中的 `while total > 21 and ace_count > 0` 里隐含了条件判断
- `dealer_should_hit()` 返回一个布尔值，实际上就是 `return calculate_hand(hand) < 17`
- 玩家输入处理：`if action == 'quit'` → `elif action == 'hit'` → `elif action == 'stand'` → `else`
- 胜负判定：`if player_bust` → `elif dealer_bust` → `elif player_total > dealer_total` → `elif player_total < dealer_total` → `else`（平局）
- 筹码检查：`if chips <= 0` → `break`

**循环结构**

| 循环 | 位置 | 作用 |
|------|------|------|
| `while True` | 主循环 | 每局游戏重新开始，直到玩家退出或筹码归零 |
| `while True` | 下注阶段 | 反复询问下注金额，直到输入有效 |
| `while True` | 玩家回合 | 反复询问 Hit/Stand，直到玩家停牌、爆牌或达成 21 |
| `while dealer_should_hit` | 庄家回合 | 自动要牌直到点数达到 17 或以上 |

**跳转控制**

| 跳转 | 位置 | 作用 |
|------|------|------|
| `break` | 下注阶段 | 输入有效下注后退出循环 |
| `return` | 下注阶段 | 输入 `quit` 时直接返回，结束整个程序 |
| `break` | 玩家回合 | 点数达到 21 时不再询问，直接进入庄家回合 |
| `break` | 玩家回合 | 玩家停牌时退出循环 |
| `continue` | 玩家回合 | 输入无效命令时重新询问 |
| `break` | 主循环 | 筹码归零时结束游戏 |

**嵌套结构**

- 主循环里面套了下注循环，下注循环套了输入校验逻辑
- 主循环里面套了玩家回合循环，玩家回合循环套了分支判断
- 胜负判定中嵌套了多个分支条件

### 3.6.5 函数逐行拆解：代码是如何拼起来的

前面分析了控制流，现在从函数层面拆解这个程序。来看数据是如何在函数之间流动、每个函数承担了哪些职责。

#### 3.6.5.1 create_deck() — 生成一副牌

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

**数据流**：无输入 → 输出一副 `list[tuple[str, int]]`

**每行拆解**：

| 行 | 做了什么 | 返回/结果类型 |
|----|----------|---------------|
| `ranks = [...]` | 定义 13 个牌面 | `list[str]` |
| `values = {...}` | 定义牌面到点数的映射 | `dict[str, int]` |
| `deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]` | 列表推导式，每张牌生成 4 张（四种花色），组成 52 张牌 | `list[tuple[str, int]]` |
| `random.shuffle(deck)` | 洗牌，原地打乱 | `None`（修改原对象） |
| `return deck` | 返回牌堆 | `list[tuple[str, int]]` |

**拆解那一行列表推导式**：

```python
# 外层循环：遍历 13 个牌面
# 内层循环：每个牌面重复 4 次（代表四种花色）
# 每个元素都是一个元组 (rank, values[rank])
deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
```

等价于：

```python
deck = []
for rank in ranks:
    for _ in range(4):
        deck.append((rank, values[rank]))
```

`_` 是 Python 中约定俗成的变量名，表示“这个值我不需要，只是占个位置”。这里内层循环只是重复 4 次，不需要用到循环变量。

关于 `.append` ，这是 `list` 的一个内置方法，意思是在列表的末尾添加一个元素。

**`tuple` 在这里的作用**：一张牌用两个字段来描述：牌面（`str`）和点数（`int`）。这两种数据组合在一起，用元组（`tuple`）打包，比用两个独立的列表更清晰。在后续代码中，每张牌始终以 `(rank, value)` 的形式传递，不会被拆散。

**关于 `return` 和 `print` 的区别**：

- `return`：把值**交还给调用者**。调用 `deck = create_deck()` 时，`return` 把牌堆传给 `deck` 变量。
- `print`：把值**显示在屏幕上给人看**。调用 `print(deck)` 只是在终端输出内容，程序无法继续使用这些数据。

如果一个函数只负责“生成数据”而不负责“显示数据”，用 `return`。如果一个函数负责“展示结果”，用 `print`。21点程序中，`create_deck()` 不输出任何内容，它只负责生成数据并返回给调用者。

---

#### 3.6.5.2 calculate_hand() — 计算手牌点数

```python
def calculate_hand(hand: list[tuple[str, int]]) -> int:
    total = sum(value for _, value in hand)
    ace_count = sum(1 for rank, _ in hand if rank == 'A')
    
    while total > 21 and ace_count > 0:
        total -= 10
        ace_count -= 1
    
    return total
```

**数据流**：输入 `list[tuple[str, int]]`（手牌）→ 输出 `int`（总点数）

**逐行拆解**：

| 行 | 做了什么 |
|----|----------|
| `sum(value for _, value in hand)` | 遍历手牌，累加所有牌的点数。`_` 表示忽略牌面，只要点数。 |
| `sum(1 for rank, _ in hand if rank == 'A')` | 数一数手牌中有几张 A |
| `while total > 21 and ace_count > 0` | 如果超过 21 点且有 A，把 A 从 11 降到 1（减 10） |
| `return total` | 返回最终点数 |

**拆解这一行**：

`sum(value for _, value in hand)` 这里用了**生成器表达式**，写法类似列表推导式但不产生完整列表，逐个生成值给 `sum()` 累加。

```python
# 展开后等价于：
total = 0
for _, value in hand:
    total += value
```

`sum()` 的参数是一个迭代器，`sum` 会遍历它并把所有值加起来。这里 `value for _, value in hand` 是一个生成器表达式，每次取出一张牌的 value。

**为什么用 `_`**：这里用 `_` 表示“我不需要这个值”。在 `for _, value in hand` 中，每张牌是 `(rank, value)` 元组，解包时 rank 用 `_` 占位，表示忽略它。这是 Python 的惯用写法。

**拆解 `while total > 21 and ace_count > 0`**：

这段是控制流中 `while` 循环的典型应用。Ace 的处理逻辑需要反复检查，每次把一张 A 从 11 降到 1，看总点数是否降到 21 以下。

比如手牌是 `['A', 'A', '9']`，初始总数为 `11 + 11 + 9 = 31`，ace_count = 2：
- 第一次循环：`31 > 21 and 2 > 0` → `total = 21`，`ace_count = 1`
- 第二次循环：`21 > 21` 为 False，退出
- 返回 21（两张 A 中一张算 11，一张算 1：11 + 1 + 9 = 21）

#### 3.6.5.3 display_hand() — 格式化显示

```python
def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)
```

**数据流**：输入 `list[tuple[str, int]]` 和 `bool` → 输出 `str`

**逐行拆解**：

| 行 | 做了什么 |
|----|----------|
| `if hide_first` | 如果隐藏第一张（庄家策略），只显示后面的牌 |
| `hand[1:]` | 切片：从第二张开始取（索引 1 到末尾） |
| `' '.join(...)` | 用空格把多个字符串拼接成一个字符串 |
| `f'{rank}'` | 牌面转成字符串（其实已经是字符串，这里是保险写法） |

**拆解 `join()`**：

`join()` 是字符串的方法，作用是用指定的分隔符把一个可迭代对象中的字符串连接起来：

```python
>>> ' '.join(['A', '10', '5'])
'A 10 5'
```

**`hide_first` 参数的默认值**：

`hide_first: bool = False` 表示这个参数是可选的。调用时可以只传 `hand`，不传 `hide_first`，这时默认值是 `False`。这个技巧在函数定义中叫**默认参数**，后面会专门讲。

---

#### 3.6.5.4 dealer_should_hit() — 判断庄家是否要牌

```python
def dealer_should_hit(hand: list[tuple[str, int]]) -> bool:
    return calculate_hand(hand) < 17
```

**数据流**：输入 `list[tuple[str, int]]` → 输出 `bool`

**为什么单独写这个函数**：这是一个良好的习惯。庄家是否要牌的规则（点数 < 17 继续要）在代码中只出现了一次，如果只写一次，似乎不需要单独封装。

但把逻辑封装成函数，有两个好处：

1. **可读性**：在主循环中看到 `while dealer_should_hit(dealer_hand):`，比看到 `while calculate_hand(dealer_hand) < 17:` 更容易理解——前者读起来像英文句子“庄家应该要牌吗？”，后者需要仔细读才能理解条件。
2. **可维护性**：如果以后要修改规则（比如改成 < 16 或 < 18），只需要改这一个地方，不需要在主循环里搜索。

这种写法和控制流没有直接关系，但写出好代码不只是用对语法，还包括如何组织代码让逻辑更清晰。

---

#### 3.6.5.5 play_blackjack() — 主游戏循环

这个函数最大，包含了所有控制流结构。从函数层面看它的结构：

```python
def play_blackjack() -> None:
    # 初始化
    print(...)
    chips = 100
    
    while True:  # 主循环：每一局
        # ---- 阶段1：下注 ----
        while True:  # 下注循环：反复询问直到有效
            # ... 输入、校验、break ...
        
        # ---- 阶段2：发牌 ----
        deck = create_deck()
        player_hand = [deck.pop(), deck.pop()]
        dealer_hand = [deck.pop(), deck.pop()]
        
        # ---- 阶段3：玩家回合 ----
        while True:  # 玩家循环：反复要牌直到停牌或爆牌
            # ... 显示手牌、输入、分支 ...
            if action == 'hit':
                # 要一张牌
            elif action == 'stand':
                break
        
        # ---- 阶段4：庄家回合 ----
        if not player_bust:
            while dealer_should_hit(dealer_hand):
                # 庄家自动要牌
        
        # ---- 阶段5：判定胜负 ----
        if player_bust:
            # ...
        elif dealer_bust:
            # ...
        elif player_total > dealer_total:
            # ...
        # ...
        
        # ---- 阶段6：检查筹码 ----
        if chips <= 0:
            break
```

**数据在函数之间如何流动**：

```
create_deck()
    ↓ 返回 deck (list[tuple])
    
deck.pop() × 4
    ↓ 取出牌组成 player_hand / dealer_hand (list[tuple])
    
calculate_hand(player_hand)
    ↓ 返回 player_total (int)
    
display_hand(player_hand)
    ↓ 返回格式化的字符串 (str)
    
dealer_should_hit(dealer_hand)
    ↓ 返回 True/False (bool) 控制 while 循环
```

**`deck.pop()` 是什么意思**：

`pop()` 是列表的方法，移除并返回列表的最后一个元素。这里 `deck.pop()` 相当于“从牌堆顶翻一张牌”。

```python
>>> cards = ['A', 'K', '7']
>>> cards.pop()
'7'
>>> cards
['A', 'K']
```

在 21 点中，`player_hand = [deck.pop(), deck.pop()]` 会从牌堆中取出两张牌，组成玩家的初始手牌。第一次 `pop()` 取最后一张，第二次 `pop()` 取倒数第二张（不保证真实发牌顺序，但随机洗过牌后顺序无意义）。

---

#### 3.6.5.6 if __name__ == "__main__"

```python
if __name__ == "__main__":
    play_blackjack()
```

这是 Python 中一个非常常见的惯用写法。

- `__name__` 是 Python 自动给每个模块设置的变量
- 当你直接运行 `python blackjack.py` 时，`__name__` 的值是 `"__main__"`，条件成立，执行 `play_blackjack()`
- 当你 `import blackjack` 到其他文件时，`__name__` 的值是 `"blackjack"`（模块名），条件不成立，不会自动执行

这行代码的作用是：**让这个文件既可以作为独立程序运行，也可以作为模块被其他程序引用**。

---

#### 3.6.5.7 这个程序里还没讲过的语法

回顾整段代码，以下语法点尚未正式讲解：

| 语法/概念 | 出现在哪里 |
|-----------|------------|
| 默认参数 | `hide_first: bool = False` |
| 列表推导式 | `[(rank, values[rank]) for rank in ranks for _ in range(4)]` |
| 生成器表达式 | `sum(value for _, value in hand)` |
| 元组解包 | `for _, value in hand` |
| `join()` 方法 | `' '.join(...)` |
| `pop()` 方法 | `deck.pop()` |
| 切片 | `hand[1:]` |
| `_` 占位符 | 循环中忽略不需要的值 |
| `if __name__ == "__main__"` | 程序入口 |
| `None` 类型 | 返回值类型 |

其中大部分在下一章“数据结构”中会系统讲解。目前只需要：**见到这些写法能大概猜到意思，不一定要自己会写**。

---

#### 3.6.5.8 总结：如何从零写出这样的程序

如果你目前还写不出这个程序，这很正常，因为它涉及了函数、数据结构、循环、分支、输入输出、多个模块的知识，不是学完几个语法点就能立刻写出来的。

写代码是**组装**的过程。

1. 先想清楚游戏规则 → 列出需要哪些功能（发牌、算分、显示、判定输赢）
2. 每个功能单独写成一个函数 → 函数之间通过参数和返回值传递数据
3. 用主循环把各个函数串起来 → 形成完整的流程

先别急于一次写出大程序。这节课的目标是能理解控制流在程序中的作用。关于数据结构（list、tuple、dict）和函数的更多细节，会在后续章节逐步展开。

---

## 3.7 本节你做了什么

1. 学会了用 `if` / `elif` / `else` 做分支判断
2. 学会了用 `for` 遍历序列（列表、字典、range）
3. 学会了用 `while` 做条件循环
4. 学会了用 `break` 提前退出循环，用 `continue` 跳过当前迭代
5. 区分了 `for`（已知迭代次数）和 `while`（未知迭代次数）的使用场景
6. 写了第一个完整的交互式游戏，包含了本节所有控制流结构
7. 了解了 Python 3.9+ 的内置泛型写法 `list[tuple[str, int]]`，无需额外 import