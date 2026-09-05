# 第4节：第3节遗留语法详解

> 本章使用说明
> 
> 这一节不要求你一次性读完并记住所有内容。它的定位是 “词典” 或 “速查手册”。
> 
> 当你以后在代码中看到 [x for x in list] 时，翻回来查 4.2 节；看到 hand[1:] 时，翻回来查 4.4 节；看到 -> None 时，翻回来查 4.10 节。
> 
> 你的目标是“知道这里有答案”，而不是“今天背下所有答案”。

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 列表推导式 | 用一行代码生成列表的简洁写法 |
| 生成器表达式 | 类似列表推导式，但不生成完整列表，逐个产生值 |
| 切片 | 从序列中取出一部分，`[start:stop:step]` |
| 元组解包 | 把元组中的值分别赋给多个变量 |
| `_` 占位符 | 表示“这个值我不需要”，用来占位置 |
| 默认参数 | 函数定义时给参数设定默认值，调用时可省略 |
| `None` | Python 中的“空值”，表示什么都没有 |
| `if __name__ == "__main__"` | 判断当前文件是直接运行还是被导入 |

---

## 4.1 写在前面：这节解决什么问题

上一节的 21 点程序里，有一些写法你可能没完全看懂：

```python
deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
total = sum(value for _, value in hand)
return ' '.join(f'{rank}' for rank, _ in hand)
player_hand = [deck.pop(), deck.pop()]
display_hand(dealer_hand, hide_first=True)
if __name__ == "__main__":
```

这些写法不属于“控制流”本身，但它们出现在控制流的例子里。如果不讲，你读代码时会卡住。

这节的目的很明确：**把第三节代码里出现的、还没讲过的语法点全部拆开讲一遍**。不求一次性掌握全部细节，但求下次再见到时能看懂。

---

## 4.2 列表推导式

### 4.2.1 它解决了什么问题

假设你有一个列表，想对每个元素做同样的操作，生成一个新列表：

```python
# 把每个数字翻倍
numbers = [1, 2, 3, 4, 5]
doubled = []
for n in numbers:
    doubled.append(n * 2)
# doubled = [2, 4, 6, 8, 10]
```

这种“遍历一个序列 → 对每个元素做操作 → 收集结果”的模式太常见了。Python 提供了一种更简洁的写法：**列表推导式**。

```python
numbers = [1, 2, 3, 4, 5]
doubled = [n * 2 for n in numbers]
# doubled = [2, 4, 6, 8, 10]
```

### 4.2.2 语法拆解

```
[ 表达式 for 变量 in 可迭代对象 ]
```

- `表达式`：对每个元素做什么操作
- `变量`：从可迭代对象中取出的每一个元素
- `可迭代对象`：来源（列表、range、字符串等）

读法：**“对于可迭代对象中的每个元素，计算表达式，收集结果”**。

### 4.2.3 带条件的列表推导式

可以在后面加 `if` 过滤：

```python
# 只取偶数
numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
evens = [n for n in numbers if n % 2 == 0]
# evens = [2, 4, 6, 8, 10]
```

读法：**“对于每个元素，如果满足条件，就收集表达式的结果”**。

### 4.2.4 21点程序中的例子

```python
deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
```

这里有两个 `for`：

- 外层：遍历 `ranks`（13 个牌面）
- 内层：每个牌面重复 4 次（四种花色）

展开后等价于：

```python
deck = []
for rank in ranks:
    for _ in range(4):
        deck.append((rank, values[rank]))
```

`_` 是占位符，表示“内层循环的变量我不需要，只是要重复 4 次”。

## 4.2.5 两个有趣的例子

**例子1：找出所有体重超标的宠物**

假设你有一份宠物档案：

```python
pets = {
    "Jvzhang": 12,      # 12 kg
    "Luna": 5,
    "Charlie": 28,
    "Nala": 4,
    "Cooper": 35,
    "Bella": 6
}
```

猫超过 8kg 就算超重，狗超过 25kg 算超重。想提取所有超重宠物的名字：

```python
overweight = [name for name, weight in pets.items() if (weight > 8 and name in ["Jvzhang", "Luna", "Nala", "Bella"]) or (weight > 25 and name in ["Charlie", "Cooper"])]
```

你看，这种硬编码显然**不优雅**且**难以理解**。换一种方式，把宠物品种也存下来：

```python
pets = {
    "Jvzhang": {"type": "cat", "weight": 12},
    "Luna": {"type": "cat", "weight": 5},
    "Charlie": {"type": "dog", "weight": 28},
    "Nala": {"type": "cat", "weight": 4},
    "Cooper": {"type": "dog", "weight": 35},
    "Bella": {"type": "cat", "weight": 6}
}

def is_overweight(name: str, info: dict) -> bool:
    if info["type"] == "cat":
        return info["weight"] > 8
    else:  # dog
        return info["weight"] > 25

overweight_pets = [name for name, info in pets.items() if is_overweight(name, info)]
# overweight_pets = ['Milo', 'Charlie', 'Cooper']
```

列表推导式在这里的作用是**过滤**：从字典中筛选出满足条件的键。

**例子2：给游戏角色加 Buff**

假设你有一队 RPG 角色的属性数据，想给他们每人加 20% 攻击力：

```python
characters = {
    "法师": {"hp": 320, "attack": 120, "defense": 50},
    "战士": {"hp": 650, "attack": 95, "defense": 180},
    "弓箭手": {"hp": 380, "attack": 140, "defense": 70},
    "刺客": {"hp": 280, "attack": 165, "defense": 40},
}

buffed = {name: {"hp": stats["hp"], "attack": int(stats["attack"] * 1.2), "defense": stats["defense"]} 
          for name, stats in characters.items()}
```

运行后：

```python
buffed = {
    "法师": {"hp": 320, "attack": 144, "defense": 50},
    "战士": {"hp": 650, "attack": 114, "defense": 180},
    "弓箭手": {"hp": 380, "attack": 168, "defense": 70},
    "刺客": {"hp": 280, "attack": 198, "defense": 40},
}
```

这里用了**字典推导式**（用 `{}` 而非 `[]`），对原始字典的每个条目做转换，生成一个新字典。`int(stats["attack"] * 1.2)` 确保攻击力是整数。

---

## 4.3 生成器表达式

### 4.3.1 它和列表推导式的区别

列表推导式用方括号 `[]`，会一次性生成完整列表。如果数据量很大（比如 1000 万个元素），内存会爆。

生成器表达式用圆括号 `()`，不生成完整列表，而是“懒”地逐个产生值，用的时候才计算。

```python
# 列表推导式 —— 立刻生成所有值
squares_list = [x * x for x in range(10)]
# squares_list 是一个完整的列表

# 生成器表达式 —— 按需生成
squares_gen = (x * x for x in range(10))
# squares_gen 是一个生成器对象，还不包含任何值
```

### 4.3.2 21点程序中的例子

```python
total = sum(value for _, value in hand)
```

`sum()` 需要一个可迭代对象。这里传入的是一个生成器表达式，它逐个产生手牌中每张牌的点数，`sum()` 一边产生一边累加，不会创建中间列表。

如果写成列表推导式：

```python
total = sum([value for _, value in hand])
```

也能跑，但会先生成一个完整列表，再传给 `sum()`。手牌最多 21 张，差别不大，但生成器表达式的写法更省内存，也更符合 Python 的习惯。

### 4.3.3 什么时候用哪个

| 场景 | 推荐 |
|------|------|
| 数据量小，或者后续要用到列表（如排序、索引） | 列表推导式 |
| 数据量大，只遍历一次 | 生成器表达式 |
| 传给 `sum()`、`max()`、`min()`、`any()`、`all()` 等函数 | 生成器表达式更省内存 |

如果判断不了，先写列表推导式——大多数情况下够用。遇到性能问题再改不迟。

---

## 4.4 切片（Slicing）

### 4.4.1 它是什么

切片是从序列（列表、字符串、元组）中取出一部分。

语法：`序列[start:stop:step]`

- `start`：起始索引（包含）
- `stop`：结束索引（不包含）
- `step`：步长（可选）

### 4.4.2 基本用法

```python
fruits = ['apple', 'banana', 'cherry', 'date', 'elderberry']

# 取第 2、3、4 个（索引 1 到 3）
fruits[1:4]   # ['banana', 'cherry', 'date']

# 取前 3 个（索引 0 到 2）
fruits[:3]    # ['apple', 'banana', 'cherry']

# 取后 2 个（从索引 3 到末尾）
fruits[3:]    # ['date', 'elderberry']

# 取全部（复制一个新列表）
fruits[:]     # ['apple', 'banana', 'cherry', 'date', 'elderberry']

# 每隔一个取一个
fruits[::2]   # ['apple', 'cherry', 'elderberry']

# 倒序
fruits[::-1]  # ['elderberry', 'date', 'cherry', 'banana', 'apple']
```

### 4.4.3 21点程序中的例子

```python
' '.join(f'{rank}' for rank, _ in hand[1:])
```

`hand[1:]` 表示“从第二张牌开始到最后一张”。庄家的第一张牌隐藏时，只需要显示后面的牌。

展开来看：

```python
hand = [('A', 11), ('K', 10), ('5', 5)]
hand[1:]  # [('K', 10), ('5', 5)]
```

### 4.4.4 有趣的应用

字符串也是序列，同样支持切片：

```python
text = "Hello, World!"
text[7:12]   # 'World'
text[:5]     # 'Hello'
text[::-1]   # '!dlroW ,olleH'
```

提取邮箱的域名：

```python
email = "student@university.edu"
domain = email[email.index('@') + 1:]
# domain = 'university.edu'
```

反转一个句子中的单词：

```python
sentence = "the quick brown fox"
words = sentence.split()  # ['the', 'quick', 'brown', 'fox']
reversed_words = ' '.join(reversed(words))
# reversed_words = 'fox brown quick the'
```

---

## 4.5 `pop()` 方法

### 4.5.1 它是什么

`pop()` 是列表的方法，移除并返回列表的最后一个元素（或指定位置的元素）。

```python
cards = ['A', 'K', '7', '3']
last = cards.pop()   # last = '3', cards = ['A', 'K', '7']
```

可以指定位置：

```python
cards = ['A', 'K', '7', '3']
second = cards.pop(1)  # second = 'K', cards = ['A', '7', '3']
```

### 4.5.2 21点程序中的用法

```python
player_hand = [deck.pop(), deck.pop()]
```

第一次 `deck.pop()` 取最后一张，第二次再取倒数第二张。

这和现实中的发牌相反（现实中是从顶部开始发），但由于已经洗过牌了，从哪个方向取不影响随机性。

### 4.5.3 和 `append()` 配对

列表最常用的两个方法：

- `append(x)`：在末尾添加一个元素
- `pop()`：从末尾移除一个元素

```python
stack = []
stack.append(1)   # [1]
stack.append(2)   # [1, 2]
stack.append(3)   # [1, 2, 3]
stack.pop()       # 返回 3，stack = [1, 2]
stack.pop()       # 返回 2，stack = [1]
```

这种“后进先出”的结构叫栈（Stack）。虽然术语听起来专业，但在 Python 中就是用列表实现的。

---

## 4.6 元组解包

### 4.6.1 它是什么

元组解包是把元组中的值分别赋给多个变量：

```python
# 一个元组
card = ('A', 11)

# 解包
rank, value = card
# rank = 'A', value = 11
```

数量必须匹配：

```python
a, b = (1, 2)     # 正确
a, b = (1, 2, 3)  # ValueError: too many values to unpack
a, b, c = (1, 2)  # ValueError: not enough values to unpack
```

### 4.6.2 在循环中的应用

`for` 循环中遍历元组列表时，直接在循环变量处解包：

```python
hand = [('A', 11), ('K', 10), ('5', 5)]

# 不解包
for card in hand:
    rank = card[0]
    value = card[1]

# 解包
for rank, value in hand:
    print(f"{rank}: {value}")
```

### 4.6.3 21点程序中的例子

```python
sum(1 for rank, _ in hand if rank == 'A')
```

遍历 `hand`，每张牌解包成 `rank` 和 `_`（点数用 `_` 忽略），只统计牌面是 `'A'` 的数量。

等价于：

```python
count = 0
for card in hand:
    if card[0] == 'A':
        count += 1
```

### 4.6.4 一个有趣的例子

交换两个变量的值：

```python
a, b = 5, 10
a, b = b, a
# a = 10, b = 5
```

Python 在解包时先计算右侧的值，再赋值给左侧，所以交换不需要临时变量。

---

## 4.7 `_` 占位符

### 4.7.1 它是什么

`_` 在 Python 中是一个合法的变量名，通常表示“这个值我不需要，只是一个占位符”。

```python
# 循环 3 次，不需要循环变量
for _ in range(3):
    print("Hello")

# 解包时忽略不需要的值
rank, _ = ('A', 11)
```

### 4.7.2 为什么用 `_` 而不是别的名字

用 `_` 是约定俗成。代码阅读者看到 `_`，会自动理解“这个值不重要，被忽略了”。如果随便起一个名字如 `temp` 或 `x`，阅读者会困惑“这个变量后面用到了吗？”。

### 4.7.3 21点程序中的例子

```python
deck = [(rank, values[rank]) for rank in ranks for _ in range(4)]
```

内层循环的 `_` 表示“我不需要知道当前循环次数，只是要重复 4 次”。

---

## 4.8 `join()` 方法

### 4.8.1 它是什么

`join()` 是字符串的方法，用指定的分隔符把一个可迭代对象中的字符串连接成一个字符串。

```python
parts = ['apple', 'banana', 'cherry']
result = ', '.join(parts)
# result = 'apple, banana, cherry'
```

### 4.8.2 常见的误区

新手经常这样写：

```python
# 错误写法
result = ', '.join('apple', 'banana', 'cherry')  # TypeError
```

`join()` 的参数是一个可迭代对象（列表、元组、生成器等），不是多个独立参数。

```python
# 正确写法
', '.join(['apple', 'banana', 'cherry'])
```

### 4.8.3 21点程序中的例子

```python
return ' '.join(f'{rank}' for rank, _ in hand)
```

把手牌中所有牌的牌面用空格连接起来。比如手牌是 `[('A', 11), ('K', 10), ('5', 5)]`，返回 `'A K 5'`。

`f'{rank}'` 生成字符串形式的牌面，`for rank, _ in hand` 遍历所有牌并提取牌面。

### 4.8.4 一个有趣的例子

统计一篇文章中每个单词出现的次数，并用 `join()` 格式化输出：

```python
text = "the quick brown fox jumps over the lazy dog"
words = text.split()
word_counts = {}

for word in words:
    word_counts[word] = word_counts.get(word, 0) + 1

# Format output
report = '\n'.join(f"{word}: {count}" for word, count in word_counts.items())
print(report)
```

输出：

```
the: 2
quick: 1
brown: 1
fox: 1
jumps: 1
over: 1
lazy: 1
dog: 1
```

`'\n'.join(...)` 用换行符连接，每一行是一个单词统计。

---

## 4.9 默认参数

### 4.9.1 它是什么

定义函数时，可以给参数指定默认值。调用时如果不传这个参数，就使用默认值。

```python
def greet(name: str, greeting: str = "Hello") -> str:
    return f"{greeting}, {name}!"

greet("MKJ")              # "Hello, MKJ!"
greet("MKJ", "Hi")        # "Hi, MKJ!"
```

### 4.9.2 21点程序中的例子

```python
def display_hand(hand: list[tuple[str, int]], hide_first: bool = False) -> str:
    if hide_first:
        return f"[HIDDEN] {' '.join(f'{rank}' for rank, _ in hand[1:])}"
    return ' '.join(f'{rank}' for rank, _ in hand)
```

调用时：

```python
# 不传 hide_first → 默认 False → 显示所有牌
display_hand(player_hand)

# 传 hide_first=True → 隐藏第一张 → 显示 "[HIDDEN] K 5"
display_hand(dealer_hand, hide_first=True)
```

### 4.9.3 一个有趣的例子

登录提醒函数，可以自定义提醒文案：

```python
def notify(user: str, message: str = "You have a new message.") -> None:
    print(f"[{user}] {message}")

notify("MKJ")                    # [MKJ] You have a new message.
notify("YZY", "Your order is ready.")  # [YZY] Your order is ready.
```

---

## 4.10 `None` 类型

### 4.10.1 它是什么

`None` 是 Python 中的“空值”，表示“什么都没有”。它的类型是 `NoneType`。

常见用途：
- 函数没有 `return` 语句时，默认返回 `None`
- 作为占位值，表示“还没有值”
- 在类型注解中表示“这个函数没有返回值”（`-> None`）

### 4.10.2 21点程序中的例子

```python
def play_blackjack() -> None:
```

`-> None` 表示这个函数不返回任何有意义的值。它做的事情是打印输出和修改游戏状态，但不返回数据给调用者。

### 4.10.3 检查 `None`

判断一个值是否为 `None`，用 `is` 运算符，不要用 `==`：

```python
if x is None:
    # x 是空值

if x is not None:
    # x 有实际值
```

### 4.10.4 一个有趣的例子

```python
def find_student(students: dict[str, int], name: str) -> int | None:
    """Find a student's score by name. Return None if not found."""
    if name in students:
        return students[name]
    return None

scores = {"MKJ": 85, "YZY": 92}
MKJ_score = find_student(scores, "MKJ")    # 85
charlie_score = find_student(scores, "Charlie")  # None

if charlie_score is None:
    print("Student not found")
```

---

## 4.11 `if __name__ == "__main__"`

### 4.11.1 它是什么

每个 Python 文件都是一个模块。当这个文件被直接运行时，`__name__` 的值是 `"__main__"`。当这个文件被 `import` 到其他文件时，`__name__` 的值是模块名（文件名）。

```python
if __name__ == "__main__":
    # 只有直接运行这个文件时才执行
    print("This is the main program")
```

### 4.11.2 为什么需要它

假设你在写一个工具模块 `calculator.py`：

```python
def add(a, b):
    return a + b

def subtract(a, b):
    return a - b

# 测试代码
print(add(3, 5))
print(subtract(10, 4))
```

如果不加 `if __name__ == "__main__"`，别人 `import calculator` 时，测试代码也会被执行，输出一堆东西。加了之后，测试代码只在直接运行 `calculator.py` 时执行，导入时不会。

### 4.11.3 21点程序中的例子

```python
if __name__ == "__main__":
    play_blackjack()
```

直接运行 `blackjack.py` 时，启动游戏。如果其他程序 `import blackjack`，不会自动开局，只会导入 `create_deck()`、`calculate_hand()` 等函数供其他程序使用。

---

## 4.12 总结

这一节拆解了第三节 21 点程序中出现的所有“陌生语法”。你可以把下表当作速查卡：

| 语法 | 一句话总结 | 出现在哪里 |
|------|------------|------------|
| 列表推导式 | `[expr for x in iterable]` | `create_deck()` 洗牌 |
| 生成器表达式 | `(expr for x in iterable)` | `calculate_hand()` 求和 |
| 切片 | `seq[start:stop:step]` | `display_hand()` 隐藏第一张 |
| `pop()` | 移除并返回列表末尾元素 | 发牌 |
| 元组解包 | `a, b = (1, 2)` | 遍历手牌 |
| `_` 占位符 | 忽略不需要的值 | 循环和解包 |
| `join()` | 用分隔符连接字符串列表 | 显示手牌 |
| 默认参数 | `def func(x, y=10)` | `display_hand(hide_first)` |
| `None` | 表示“没有值” | `play_blackjack() -> None` |
| `if __name__ == "__main__"` | 判断是否直接运行 | 程序入口 |

---

## 4.13 后记

这四节内容大约可以对应普通 Python 教材半本书到的内容，其中21点的难度基本可以和非CS专业的期末作业媲美，这四节建议用至少一周的空余时间吃透，后面我们开始讲真正的目的即数据处理，但在这之前，我想让大家体味到程序可以做很多有趣的东西，不单单是为了课程服务的。一部笔记本，一颗愿意学习的脑袋，足以创造许许多多的工具。
