# 第6节：数据结构

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 数据结构 | 组织和存储数据的方式 |
| 列表（list） | 有序、可变的序列，可以放任意类型的元素 |
| 元组（tuple） | 有序、不可变的序列，创建后不能修改 |
| 字典（dict） | 键值对（key-value）的集合，通过键来快速查找值 |
| 集合（set） | 无序、不重复的元素的集合 |
| 可变（mutable） | 对象创建后可以修改其内容 |
| 不可变（immutable） | 对象创建后不能修改其内容 |
| 索引（index） | 元素在序列中的位置，从 0 开始 |
| 键（key） | 字典中用来查找值的标识符 |
| 值（value） | 字典中与键关联的数据 |
| 浅拷贝（shallow copy） | 只复制最外层容器，内部元素共享引用 |
| 深拷贝（deep copy） | 递归复制所有层级的元素 |

---

## 6.1 为什么需要数据结构

你已经用过一些变量了。`name = "MKJ"`，`age = 18`。但比如你要存一个班 50 个人的名字呢？开 50 个变量显然不现实。

数据结构就是解决这个问题的——它们在单个变量里存放多个值，并提供方便的方式来组织、访问和操作这些值。

Python 有四种内置的数据结构，分别对应不同的使用场景：

| 数据结构 | 是否有序 | 是否可修改 | 是否允许重复 | 访问方式 |
|----------|----------|------------|--------------|----------|
| 列表（list） | 有序 | 可修改 | 允许重复 | 索引 |
| 元组（tuple） | 有序 | 不可修改 | 允许重复 | 索引 |
| 字典（dict） | 有序（Python 3.7+）（“有序”指插入顺序被保留，不是指键按大小排序） | 可修改 | 键唯一，值可重复 | 键 |
| 集合（set） | 无序 | 可修改 | 不重复 | 无索引 |

在写代码之前，先看一个判断场景：

- 需要按顺序存多个东西，并且之后可能要增删改 → 列表
- 需要按顺序存多个东西，但存完之后不想被改动 → 元组
- 需要用名字/ID 来查找对应的值 → 字典
- 需要去重或做集合运算（交集、并集） → 集合

---

## 6.2 列表（list）

### 6.2.1 创建和访问

```python
# 空列表
empty = []

# 存不同元素（列表可以混合类型）
fruits = ["apple", "banana", "cherry"]      # 统一类型
mixed = [1, "hello", 3.14, True]            # 混合类型，但不推荐

# 通过索引访问（从 0 开始）
fruits[0]   # "apple"
fruits[1]   # "banana"
fruits[-1]  # "cherry"  -1 表示最后一个元素
```

### 6.2.2 修改列表

```python
fruits = ["apple", "banana", "cherry"]

# 追加到末尾
fruits.append("date")        # ["apple", "banana", "cherry", "date"]

# 插入到指定位置
fruits.insert(1, "blueberry")  # ["apple", "blueberry", "banana", "cherry", "date"]

# 修改指定位置的元素
fruits[2] = "blackberry"     # ["apple", "blueberry", "blackberry", "cherry", "date"]

# 删除指定位置的元素
removed = fruits.pop(0)      # 删除并返回第一个元素
fruits.remove("cherry")      # 删除第一个匹配的元素

# 清空
fruits.clear()               # []
```

### 6.2.3 常用操作

```python
numbers = [3, 1, 4, 1, 5, 9, 2]

len(numbers)      # 7 —— 长度
numbers.count(1)  # 2 —— 某个元素出现次数
numbers.index(9)  # 5 —— 某个元素首次出现的位置
numbers.sort()    # 原地排序 —— [1, 1, 2, 3, 4, 5, 9]
numbers.reverse() # 原地反转 —— [9, 5, 4, 3, 2, 1, 1]

# 用 + 拼接列表
[1, 2] + [3, 4]   # [1, 2, 3, 4]

# 用 * 重复
[0] * 5           # [0, 0, 0, 0, 0]

# 判断元素是否存在
"apple" in fruits    # True / False
"grape" not in fruits
```

### 6.2.4 一个例子：背包系统

```python
# 玩家的背包
inventory = ["sword", "shield", "potion", "gold"]

# 捡到新物品
inventory.append("magic ring")
print(inventory)  # ['sword', 'shield', 'potion', 'gold', 'magic ring']

# 使用物品（从背包移除并使用）
potion = inventory.pop(2)  # 拿出第三格（索引2）的 potion
print(f"Used {potion}! Health restored.")
print(inventory)  # ['sword', 'shield', 'gold', 'magic ring']

# 检查是否有某物品
if "key" in inventory:
    print("You can open the door.")
else:
    print("You don't have the key.")
```

---

## 6.3 元组（tuple）

### 6.3.1 创建和访问

```python
# 创建
empty = ()
single = (5,)        # 注意：单元素元组要加逗号
coordinates = (10, 20)
colors = ("red", "green", "blue")

# 访问（和列表一样）
coordinates[0]   # 10
coordinates[1]   # 20
```

### 6.3.2 不可变性

```python
point = (3, 5)
point[0] = 4      # TypeError: 'tuple' object does not support item assignment
```

一旦创建，不能修改、删除、或增加元素。

### 6.3.3 什么时候用元组

**元组和列表的区别只有一个：元组不可变，列表可变。**

- 坐标 `(x, y)`：坐标变了应该创建新坐标，而不是改旧的
- RGB 颜色 `(255, 128, 64)`：颜色本身不变
- 函数的多个返回值：本质上就是元组
- 字典的键（列表不能当键，元组可以）

```python
# 元组可以当字典的键
game_moves = {
    (0, 0): "start",
    (1, 3): "treasure",
    (2, 5): "boss"
}
```

选择标准很简单：

- 如果数据需要修改 → 列表
- 如果数据不需要修改 → 元组（占内存更小，代码更安全）

---

## 6.4 字典（dict）

### 6.4.1 创建和访问

```python
# 空字典
empty = {}

# 创建
player = {
    "name": "MKJ",
    "level": 5,
    "hp": 100,
    "skills": ["fireball", "heal"]
}

# 通过键访问
player["name"]      # "MKJ"
player["level"]     # 5

# 用 get() 安全访问（键不存在时返回 None 或指定默认值）
player.get("mana")           # None
player.get("mana", 0)        # 0（默认值）
```

### 6.4.2 修改字典

```python
# 添加或修改键值对
player["hp"] = 80              # 修改
player["mana"] = 50            # 添加

# 删除键值对
del player["skills"]           # 删除整个键值对
removed = player.pop("level")  # 删除并返回值（5）
removed = player.pop("xxx", 0) # 如果不存在，返回默认值

# 获取所有键、值、键值对
player.keys()      # dict_keys(['name', 'hp', 'mana'])
player.values()    # dict_values(['MKJ', 80, 50])
player.items()     # dict_items([('name', 'MKJ'), ('hp', 80), ('mana', 50)])

# 检查键是否存在
if "hp" in player:
    print(f"HP: {player['hp']}")
```

### 6.4.3 遍历字典

```python
# 遍历键
for key in player:
    print(key)

# 遍历键值对（最常用）
for key, value in player.items():
    print(f"{key}: {value}")

# 遍历键和值分别用 .keys() 和 .values()
```

### 6.4.4 一个例子：技能冷却管理

```python
# 技能名称 → 冷却时间（回合数）
cooldowns = {
    "fireball": 0,
    "heal": 2,
    "shield": 5,
    "ultimate": 10
}

# 每回合冷却 -1
def tick_cooldowns(cooldowns):
    for skill in list(cooldowns.keys()):  # 用 list() 复制一份
        if cooldowns[skill] > 0:
            cooldowns[skill] -= 1
    return cooldowns

# 使用技能（设置冷却）
def use_skill(skill):
    if cooldowns[skill] == 0:
        print(f"✨ Using {skill}!")
        cooldowns[skill] = cooldowns.get(skill, 0) + 3
    else:
        print(f"⏳ {skill} is on cooldown for {cooldowns[skill]} more turns.")
```

为什么用 `list(cooldowns.keys())` 复制：在遍历字典时不能修改字典的大小（添加或删除键）。但可以修改已有的值。`list(cooldowns.keys())` 会生成一个键的列表副本，即使遍历过程中字典变化了也不影响这个副本。

---

## 6.5 集合（set）

### 6.5.1 创建和基本操作

```python
# 创建
empty = set()           # 不能用 {}，因为那是空字典
numbers = {1, 2, 3, 4, 5}
letters = {"a", "b", "c"}

# 自动去重（集合的关键特性）
duplicates = {1, 2, 2, 3, 3, 3}   # {1, 2, 3}

# 添加
numbers.add(6)          # {1, 2, 3, 4, 5, 6}

# 删除
numbers.remove(3)       # 如果存在则删除，不存在则报错
numbers.discard(10)     # 如果存在则删除，不存在也不报错

# 随机取出一个元素（常用于待办列表）
numbers.pop()           # 因为无序，pop 删除的是随机元素
```

### 6.5.2 集合运算

```python
a = {1, 2, 3, 4}
b = {3, 4, 5, 6}

# 并集（所有元素）
a | b           # {1, 2, 3, 4, 5, 6}
a.union(b)      # 同上

# 交集（共同元素）
a & b           # {3, 4}
a.intersection(b)

# 差集（在 a 中不在 b 中）
a - b           # {1, 2}
a.difference(b)

# 对称差集（在 a 或 b 中但不在交集中）
a ^ b           # {1, 2, 5, 6}
a.symmetric_difference(b)

# 判断子集
{1, 2}.issubset(a)      # True
{1, 2, 3, 4}.issuperset({1, 2})  # True
```

### 6.5.3 一个例子：好友关系分析

```python
MKJ_friends = {"YZY", "WYT", "WYH", "LYZ"}
YZY_friends = {"MKJ", "WYT", "TYZ", "MFY"}

# 共同好友
mutual = MKJ_friends & YZY_friends
print(f"Mutual friends: {mutual}")  # {'WYT'}

# 总好友数（去重）
all_friends = MKJ_friends | YZY_friends
print(f"Total unique friends: {len(all_friends)}")  # 6

# MKJ 有但 YZY 没有的
only_MKJ = MKJ_friends - YZY_friends
print(f"Only MKJ has: {only_MKJ}")  # {'WYH', 'LYZ'}

# 判断是否为好友
def are_friends(person1, person2):
    return person2 in person1_friends
```

---

## 6.6 可变与不可变

### 6.6.1 哪些类型可变

| 可变（mutable） | 不可变（immutable） |
|-----------------|---------------------|
| 列表（list） | 元组（tuple） |
| 字典（dict） | 字符串（str） |
| 集合（set） | 整数（int） |
| 自定义对象 | 浮点数（float） |
| | 布尔值（bool） |

### 6.6.2 为什么这很重要

当你把一个列表传给函数：

```python
def add_item(inventory, item):
    inventory.append(item)   # 直接修改了外部列表
    return inventory

my_inv = ["sword", "shield"]
add_item(my_inv, "potion")
print(my_inv)   # ['sword', 'shield', 'potion'] —— 被改了
```

如果不想让函数修改列表，传入副本：

```python
def add_item(inventory, item):
    inventory = inventory[:]   # 复制一份再操作
    inventory.append(item)
    return inventory
```

或者传入元组（不可变，天然无法修改）：

```python
def add_item(inventory_tuple, item):
    # inventory_tuple.append(item)  # 这里会报错
    return inventory_tuple + (item,)  # 返回一个新元组
```

记住这个区别：Python 中的赋值操作，对于可变类型是共享引用；对于不可变类型是给变量分配新值。

---

## 6.7 深浅拷贝（重要！）

这是数据结构中最容易被忽略的坑。

### 6.7.1 赋值不复制

```python
a = [1, 2, 3]
b = a           # b 和 a 指向同一个对象！
b.append(4)
print(a)        # [1, 2, 3, 4] —— a 也被改了
```

这里 `b = a` 没有复制列表，只是复制了“引用”。两个变量指向同一块内存。

### 6.7.2 浅拷贝

```python
# 方式1：切片
b = a[:]

# 方式2：list() 构造
b = list(a)

# 方式3：copy 模块
import copy
b = copy.copy(a)
```

浅拷贝复制了最外层容器，但内部元素仍然是同一个引用。

```python
a = [[1, 2], [3, 4]]
b = a[:]          # 浅拷贝
b[0].append(5)    # 修改 b[0] 这个内部列表
print(a)          # [[1, 2, 5], [3, 4]] —— a[0] 也被改了！
```

这里的 `b[0]` 和 `a[0]` 指向的是同一个内部列表。浅拷贝只复制了外层列表。

### 6.7.3 深拷贝

```python
import copy
a = [[1, 2], [3, 4]]
b = copy.deepcopy(a)   # 完全复制
b[0].append(5)
print(a)          # [[1, 2], [3, 4]] —— a 没有被改
```

深拷贝会递归地复制所有层级的元素，生成完全独立的对象。

### 6.7.4 什么时候需要注意

```python
# 场景1：字典的值是列表
player = {
    "name": "MKJ",
    "inventory": ["sword", "shield"]
}

# 复制这个字典时，如果不深拷贝，inventory 会被共享
backup = player.copy()           # 浅拷贝
backup["inventory"].append("potion")
print(player["inventory"])       # ['sword', 'shield', 'potion'] —— 被改了

# 用深拷贝
backup = copy.deepcopy(player)
backup["inventory"].append("ring")
print(player["inventory"])       # ['sword', 'shield', 'potion'] —— 没变
```

### 6.7.5 一个例子：游戏存档

```python
import copy

# 当前游戏状态
save = {
    "player": {
        "name": "MKJ",
        "hp": 85,
        "position": (5, 12)
    },
    "items": ["sword", "shield", "potion"],
    "quests_completed": ["slime", "goblin"]
}

# 快速保存（浅拷贝的陷阱）
quick_save = save.copy()           # 浅拷贝
quick_save["player"]["hp"] = 100   # 修改快速存档
print(save["player"]["hp"])        # 100 —— 主存档也被改了！

# 正确的快速保存
def save_game(state):
    return copy.deepcopy(state)

def load_game(state):
    return copy.deepcopy(state)

quick_save = save_game(save)
quick_save["player"]["hp"] = 100
print(save["player"]["hp"])        # 85 —— 主存档没有被改
```

---

## 6.8 你学到了什么

这一节覆盖了 Python 的四种内置数据结构，以及它们背后的重要概念。回顾一下你学到了什么：

**列表（list）**
- 有序、可变、允许重复
- 增删改查的基本操作：`append()`、`insert()`、`pop()`、`remove()`、`sort()`、`reverse()`
- 用索引访问元素，用切片取子序列
- 适用场景：需要按顺序存东西，并且之后可能要增删改

**元组（tuple）**
- 有序、不可变、允许重复
- 创建后不能修改，但访问方式与列表相同
- 适用场景：坐标、颜色值、函数多返回值、字典的键

**字典（dict）**
- 键值对存储，键唯一且不可变，值任意
- 通过键快速查找值，时间复杂度 O(1)
- 常用操作：`dict[key]`、`.get()`、`.keys()`、`.values()`、`.items()`
- 适用场景：需要用名字/ID 来查找对应数据

**集合（set）**
- 无序、可变、不重复
- 自动去重，支持集合运算：并集 `|`、交集 `&`、差集 `-`、对称差集 `^`
- 适用场景：去重、好友关系分析、成员资格检查

**深浅拷贝**
- 赋值不复制，只复制引用
- 浅拷贝复制外层容器，内部元素仍共享
- 深拷贝递归复制所有层级，生成完全独立的对象
- 使用 `copy.deepcopy()` 时需要导入 `copy` 模块

**可变与不可变**
- 可变类型：list、dict、set
- 不可变类型：int、float、str、bool、tuple
- 可变类型作为函数参数时，函数内部修改会影响外部

**选择指南**

| 你需要…… | 用什么 |
|-----------|--------|
| 按顺序存东西，且会修改 | 列表 |
| 按顺序存东西，但不会修改 | 元组 |
| 用名字查找对应的值 | 字典 |
| 去重或集合运算 | 集合 |
| 完整复制一个嵌套结构 | 深拷贝 |
