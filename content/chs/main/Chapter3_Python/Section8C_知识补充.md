# 第8C节：知识补充 —— 前 7 节没讲过的知识

8B 里出现的很多写法，前 7 节并没有讲过。这一节把它们集中补齐，每一条都对应回项目中的真实用法。

---

## 8C 前 7 节没讲过、但项目里用到的知识

前面 7 节覆盖了：注释/变量/类型、控制流、列表推导式/切片/解包、函数与作用域、list/tuple/dict/set、拷贝，等等。但 TankBattle 是一个真实项目，它还用到了一批“课本外”的语法和标准库。

下面这份表先做总览，然后逐条补充讲解。**每条都给出：语法 → 最小示例 → 项目中的真实用法 → 注意事项。**

| 知识点 | 项目中的位置 | 前 7 节的状态 |
|--------|--------------|---------------|
| 类、对象、`__init__`、`self`、方法 | 所有 `class`（tank/player/enemy/bullet/powerup/game/display/input_handler） | 未讲（只在 2.4 出现过 `type()` 输出 `<class 'int'>`） |
| 继承、`super()`、多态 | `Player(Tank)`、`Enemy(Tank)` | 未讲 |
| `@property` | `player.py`、`bullet` 的 `position`、`game.base_position` 等 | 未讲 |
| 类属性 vs 实例属性 | `Tank` 顶部的四个默认值 | 未讲 |
| 私有命名约定 `_name` | `_step`、`_lines`、`_clock`、`_hud_lines` 等 | 未讲 |
| 异常：`try/except/else/finally`、多异常、`raise` | `game.load_high_score`、`main`、`input_handler` | 只在 3.6 的 21 点程序里出现过一次 `try/except ValueError` |
| `with` 与上下文管理器（`__enter__`/`__exit__`） | `main.py` + `InputHandler` | 7.9 用过 `with open(...)`，但没有解释 |
| 类型注解进阶：`list[list[str]]`、`tuple[int, int]`、`str \| None` | 几乎每个函数签名 | 2.6.2 讲过基本注解；联合类型未讲 |
| 条件表达式 `a if cond else b` | `enemy._choose_path`、`display`、`game` 多处 | 未讲 |
| 链式比较 `0 <= r < H` | `battlefield.in_bounds`、`input_handler.parse_keys` | 未讲 |
| f-string 格式说明符 `{score:06d}` | `display._hud_lines` | 未讲 |
| `any()` / `all()` / `sum(生成器)` | `game.can_tank_enter`、`active_bullets` | 只在 4.3.3 的一行表格里出现过名字 |
| 边遍历边修改容器 | `game._update_powerups` | 未讲（6.7 的拷贝知识可以衔接） |
| `random.choice / random / uniform / choices(weights)` | `battlefield`、`enemy`、`game` | 只讲过 `shuffle`（3.6）、`randint`（5.8）、`seed`（7.9） |
| `time.perf_counter / monotonic / sleep` | `main`、`player`、`input_handler` | 7.9 只用过 `time.time()` |
| `pathlib` 的 `read_text / write_text` | `config`、`game` | 7.9 用过 `Path` 但没有解释 |
| `shutil.get_terminal_size()` | `main`、`display` | 未讲 |
| `unicodedata.east_asian_width()` | `display.display_width` | 未讲 |
| ANSI 光标定位、清行、备用屏幕 | `display.Renderer` | 改写前的第 8 节 只讲过颜色/清屏/光标 |
| 非阻塞输入：`termios`/`tty`/`select`/`msvcrt` | `input_handler` | 改写前的第 8 节 有基础版，这里的完整版是新的 |
| `os.name` / `sys.platform` | `input_handler` | 未讲 |
| `signal` | `main._install_signal_handlers` | 未讲 |
| 固定步长循环、帧差分渲染 | `main`、`display` | 未讲（属于工程模式，不是语法） |

### 8C.1 类与对象入门

**为什么需要类**：当一组数据（位置、血量、速度）和一组操作（移动、受伤、开火）总是绑在一起时，用类把它们打包。

```python
class Counter:
    """A simple counter."""

    def __init__(self, start: int = 0) -> None:
        self.value = start          # 实例属性

    def add(self, amount: int = 1) -> int:
        self.value += amount
        return self.value


c = Counter(10)      # c 是一个“实例”，__init__ 被自动调用
print(c.value)       # 10
print(c.add(5))      # 15
print(c.value)       # 15
```

需要记住的三个角色：

| 角色 | 在示例里 | 作用 |
|------|----------|------|
| 类 | `Counter` | 模板，定义“有哪些属性、哪些方法” |
| 实例 | `c` | 按模板造出来的具体对象，拥有自己的 `value` |
| `self` | `add` 的第一个参数 | 代表“调用这个方法的实例本身”，Python 自动传入 |

方法定义时的第一个参数必须是 `self`。`c.add(5)` 等价于 `Counter.add(c, 5)`——这就是为什么调用时不用写 `self`。

**项目中的用法**：`Tank`、`Player`、`Enemy`、`Bullet`、`PowerUp`、`Game`、`Renderer`、`InputHandler`、`Effect` 都是类。它们各自持有自己的状态：每辆坦克有自己的 `row/col/hp`，每个子弹有自己的 `accum`，游戏有全局的 `score/stage`。

**常见误区**：不要为了“面向对象”而面向对象。`battlefield.py`、`display.py` 里的很多逻辑写成了普通函数——因为它们不需要保存状态，写成函数更简单。

### 8C.2 继承、`super()` 与多态

**继承**表达“是一种”的关系：`Player` 是一种 `Tank`。

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
        super().__init__(row, col, hp=1)   # 让父类完成公共初始化
        self.power = 0                     # 子类特有的属性

    def shout(self) -> str:
        return "player!"
```

- `super()` 指的是“父类”；`super().__init__(...)` 调用父类的 `__init__`，避免把 `row/col/hp` 的赋值抄一遍。
- 子类可以**新增**属性/方法（`power`），也可以**重写**父类方法：本项目的 `Player.update` 和 `Enemy.update` 都是同名但行为完全不同的方法。
- **多态**：`game._update_playing` 里写的是：
  ```python
  self.player.update(dt, self)
  for enemy in self.enemies:
      enemy.update(dt, self)
  ```
  这段代码不关心 `update` 里具体做了什么。以后你加一个“会分裂的敌人”，只要它也有 `update(dt, game)`，`game.py` 一行都不用改。

**项目中的用法**：`Enemy` 和 `Player` 都继承 `Tank`，复用了 `try_move`、`take_damage`、`can_fire`、`position`、`mark()` 等公共方法。子类只写“不一样的东西”。

### 8C.3 `@property`：把计算伪装成属性

有些“值”不是存起来的，而是由别的数据算出来的。比如玩家的子弹速度由“火力等级”决定：

```python
class Demo:
    def __init__(self) -> None:
        self.power = 0

    @property
    def speed(self) -> float:
        return 1.0 + 0.3 * self.power


d = Demo()
print(d.speed)     # 1.0   —— 注意：没有括号！
d.power = 2
print(d.speed)     # 1.6
```

- 加了 `@property` 的方法，用起来像属性：写 `d.speed`，不写 `d.speed()`。
- 它每次访问都会**重新计算**，所以永远和 `power` 保持一致。
- 默认**只读**：`d.speed = 5` 会报 `AttributeError: can't set attribute`。

**项目中的用法**：`Player.max_bullets`、`Player.bullet_speed`、`Player.breaks_steel` 都是 `@property`。火力升级只改 `self.power`，这三个值自动更新：

```python
    @property
    def max_bullets(self) -> int:
        return PLAYER_BULLETS_BY_POWER[self.power]
```

子弹发射时会读 `tank.bullet_speed`，所以刚升完级的下一发立刻变快。

**一个真实的坑**（项目开发时踩过）：如果父类的 `__init__` 里写了 `self.max_bullets = 1`，而子类把 `max_bullets` 定义成了只有 getter 的 `@property`，那么创建子类实例时会直接 `AttributeError`——因为 `property` 是“数据描述符”，赋值会先找它的 setter，找不到就报错。

所以 `Tank` 的默认值不是写在 `__init__` 里，而是写成**类属性**：

```python
class Tank:
    # Defaults; subclasses may override them with properties.
    bullet_speed = 1.0
    max_bullets = 1
    shoot_cooldown = 0.3
    breaks_steel = False
```

子类的 `@property` 会覆盖父类的类属性，读取时优先走子类的 property，而父类的 `__init__` 不会去“赋值”它。这就是那段注释在提醒的事。

### 8C.4 类属性 vs 实例属性

```python
class Dog:
    species = "canis"          # 类属性：所有狗共享

    def __init__(self, name: str) -> None:
        self.name = name        # 实例属性：每只狗自己一份


a = Dog("A")
b = Dog("B")
print(Dog.species, a.species, b.species)   # 都是 canis
a.species = "wolf"        # 给 a 单独“遮蔽”了一个类属性
print(Dog.species)        # canis（类属性没变）
print(a.species, b.species)   # wolf canis
```

- 只读的类属性，所有实例共享同一份值，省内存；
- 一旦 `a.species = ...`，就会在 `a` 自身上新建一个实例属性，**遮蔽**类属性；
- 判断标准：**每个实例都可能不同的 → 实例属性；所有实例都一样的常量默认值 → 类属性**。

**项目中的用法**：`Tank` 的 `bullet_speed/max_bullets/shoot_cooldown/breaks_steel` 是“基础坦克的默认值”；`Enemy.__init__` 会把 `bullet_speed` 等赋成实例属性；`Player` 用 `@property` 覆盖。

### 8C.5 私有命名约定：`_name`

Python 没有 `private` 关键字，只有命名约定：

| 写法 | 含义 |
|------|------|
| `name` | 公开，随便用 |
| `_name` | “内部使用”，请在类/模块内部调用，别在外部依赖它 |
| `__name` | 名称改写（name mangling），一般用于避免子类冲突，新手很少需要 |

**项目中的用法**：`game._step_bullet`、`game._update_playing`、`renderer._lines`、`player._clock`、`input_handler._read_unix` 都是内部实现。外部模块（`main.py`、`display.py`）只调用公开接口，比如 `game.update`、`game.handle_input`、`renderer.draw`。

约定不是强制。测试代码有时会调用 `game._step_bullet(...)` 来构造场景，但你要意识到：**下划线开头的接口可能在没有通知的情况下改变**。

### 8C.6 异常处理：`try` / `except` / `else` / `finally`

第 3.6 节的 21 点程序里出现过一次：

```python
try:
    bet = int(bet_input)
except ValueError:
    print("Invalid input. Enter a number.")
```

那是最基本的用法。完整结构是：

```python
try:
    risky()                 # 可能出错的代码
except ValueError:          # 捕获指定异常
    print("值是错的")
except (OSError, TypeError) as exc:   # 一个 except 捕获多种异常
    print(f"另一个问题：{exc}")
else:
    print("没出错才执行这里")          # 可选
finally:
    print("不管出不出错都执行")        # 可选，收尾用
```

关键点：

- **异常是分类型的**。`ValueError`（值不对）、`OSError`（文件和系统相关）、`KeyError`（字典键不存在）、`TypeError`（类型不对）等。
- **`except Exception as exc`** 可以把异常对象绑到变量上，打印它的信息。
- **`raise`** 主动抛出异常：
  ```python
  if not 0 <= index < len(items):
      raise IndexError("下标越界")
  ```
- **不要写裸的 `except: pass`**。它会连程序自己的 bug 一起吞掉，让你 debug 到崩溃。
- 原则：**只有你确实知道怎么恢复的异常才捕获**；否则让它往上抛。

**项目中的用法**：

1. 读最高分：第一次玩文件不存在，或者文件被人手改坏：

   ```python
   try:
       return int(HIGH_SCORE_FILE.read_text(encoding="utf-8").strip())
   except (OSError, ValueError):
       return 0
   ```

   - `OSError` 管“文件问题”（不存在、没权限）；
   - `ValueError` 管“内容问题”（`int("abc")` 会抛这个）；
   - 两种情况都用默认值 0 恢复，游戏继续。

2. 写最高分：目录可能只读，写失败也不该让游戏崩：

   ```python
   try:
       HIGH_SCORE_FILE.write_text(f"{score}\n", encoding="utf-8")
   except OSError:
       pass
   ```

3. `main.py` 最外层捕获 `KeyboardInterrupt`，让 Ctrl+C 走正常退出流程，而不是打印一长串 traceback。

### 8C.7 `with` 与上下文管理器

第 7.9 节用过：

```python
with open(csv_path, 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f)
    ...
```

`with` 保证“**离开代码块时一定收尾**”，哪怕中间出现异常。文件会自动关闭。

它是怎么做到的？对象实现两个方法：

```python
class Timer:
    def __enter__(self):
        self.start = time.perf_counter()
        return self                    # as 后面的变量拿到这个返回值

    def __exit__(self, exc_type, exc, tb):
        self.elapsed = time.perf_counter() - self.start
        print(f"用了 {self.elapsed:.3f} 秒")
        return False                   # 返回 False：不吞异常，继续往外抛


with Timer() as t:
    do_something()
print(t.elapsed)
```

- `__enter__`：进入 `with` 块时调用；
- `__exit__`：离开时调用（正常离开、`return`、抛异常都会调用）；
- `__exit__` 返回 `True` 会“吞掉”异常，返回 `False`/`None` 则让异常继续传播——新手建议永远返回 `False`。

**项目中的用法**：`InputHandler` 就是一个上下文管理器。进入时把终端切到 raw 模式，退出时恢复：

```python
with InputHandler() as keys:
    renderer.start()
    try:
        ...
    finally:
        renderer.stop()
```

如果不用 `with`，用户按到一半程序崩了，终端就会停留在 raw 模式——按键不回显、回车不换行，只能重开终端。这就是“收尾”的价值。

### 8C.8 类型注解进阶

2.6.2 讲过基本注解（`name: str`、`-> str`），当时强调：**注解只是给人/工具看的，解释器不检查**。项目把注解用得很足，这里补充几种新写法：

```python
def grid_size() -> tuple[int, int]:            # 元组类型
    ...

def make_grid(w: int, h: int) -> list[list[str]]:   # 嵌套列表
    ...

def find_player() -> "Player | None":          # 可能找不到 → 允许 None
    ...

def tanks(self):                                # 不关心返回什么具体类型
    yield self.player
```

| 写法 | 含义 | 备注 |
|------|------|------|
| `list[int]` | 整数列表 | Python 3.9+ 支持 |
| `list[list[str]]` | 字符串的二维列表 | 项目里表示地图 |
| `tuple[int, int]` | 两个整数的元组 | 表示 `(row, col)` |
| `str \| None` | 字符串或 None | Python 3.10+；老版本写 `Optional[str]` |
| `-> None` | 不返回有意义的值 | 函数只是“做事” |
| `-> "Bullet"` | 返回值是 Bullet | 字符串形式可以延迟解析，避免导入循环 |

**项目中的用法**：`battlefield.create_battlefield(stage) -> list[list[str]]` 一眼就能看出返回地图；`game.can_tank_enter(...) -> bool` 一眼就能看出是判断；`Game.__init__(self) -> None`。

**注意**：`str \| None` 这种“联合类型”在 Python 3.10 之前会报语法错误。如果你的环境是 3.8/3.9，要写 `Optional[str]`（需要 `from typing import Optional`）。

### 8C.9 条件表达式：`a if cond else b`

这是把一个简单的 `if/else` “压进一行”的写法，结果是一个**表达式**（可以放在 `=` 右边、函数参数里、字符串里）。

```python
direction = "down" if dr > 0 else "up"
```

等价于：

```python
if dr > 0:
    direction = "down"
else:
    direction = "up"
```

再看嵌套（不好读，但项目里有）：

```python
d = ("down" if dr > 0 else "up") if abs(dr) > abs(dc) \
    else ("right" if dc > 0 else "left")
```

**项目中的用法**：

- `enemy._choose_path`：`horizontal = "right" if dc > 0 else "left"`；
- `display._build_board`：`color = ARMOR_HP_COLORS.get(enemy.hp, color)` 上面还有 `if ... else` 的三元；
- `player.handle_key` 里没有连写，但 `game._destroy_base(by_player)` 的调用方用了 `bullet.faction == "player"` 传布尔值。

**注意**：条件表达式适合“二选一”且短小的场合。超过两层嵌套就该改回 `if` 语句。

### 8C.10 链式比较

Python 允许把多个比较连起来写，含义和数学里一样：

```python
0 <= row < FIELD_HEIGHT
```

等价于 `0 <= row and row < FIELD_HEIGHT`，但 **`row` 只会被求值一次**，而且更短、更不容易写错（不会把 `row` 抄错）。

项目里还有字符的链式比较：

```python
while final < len(rest) and not ("@" <= rest[final] <= "~"):
```

这是利用 ASCII 码的连续性：`"@"`（64）到 `"~"`（126）之间包括了所有字母和多数符号。转义序列的“结束字符”就落在这个范围里。**不过这种写法可读性差**，只在解析二进制/协议时常见；日常业务代码里别用。

**项目中的用法**：`battlefield.in_bounds`、`display.overlay` 里的 `if not 0 <= row < FIELD_HEIGHT`。

### 8C.11 f-string 的格式说明符

2.6.2 只讲了 `f"{x}"`。其实花括号里可以加 `:` 后面的**格式说明符**：

```python
score = 1234
print(f"{score:06d}")    # 001234  —— 补零到 6 位
print(f"{score:>8d}")    #     1234 —— 右对齐宽度 8
print(f"{score:<8d}")    # 1234     —— 左对齐
print(f"{score:^8d}")    #   1234   —— 居中
rate = 0.5
print(f"{rate:.1%}")     # 50.0%   —— 百分比
print(f"{rate:.3f}")     # 0.500   —— 保留 3 位小数
```

格式说明符的结构是 `[[填充]对齐][符号][宽度][,][.精度][类型]`，常用的就上面这几种。

**项目中的用法**：HUD 里的分数：

```python
f"{game.score:06d}"
```

分数从 0 涨到几万，如果直接显示 `{game.score}`，数字长度变化时整个 HUD 会左右跳动；`06d` 固定成 6 位，HUD 稳定不动。

### 8C.12 `any()` / `all()` / `sum()` 与生成器

4.3.3 的表格里出现过它们，这里正式讲：

```python
print(any([False, True, False]))    # True  —— 只要有一个真
print(all([True, True, False]))     # False —— 必须全是真
print(sum([1, 2, 3]))               # 6
```

它们也接受**生成器表达式**（4.3 节的内容），而且有**短路**特性：

```python
# 只要有一个敌人站在 (r, c)，就返回 True，后面的不再检查
any(t.alive and t.position == (r, c) for t in self.tanks())

# 统计活着的子弹数量：每个活子弹贡献一个 1
sum(1 for b in self.bullets if b.alive and b.owner is tank)
```

**项目中的用法**：

- `game.can_tank_enter`：`any(...)` 判断目标格是否被别的坦克占用；
- `game.active_bullets`：`sum(1 for ...)` 统计某辆坦克在场上的子弹数（决定能否再开火）；
- `game._drop_powerup`：用 `any(...)` 过滤掉“已经有坦克/道具”的格子；
- `display.display_width`：`sum(...)` 逐字符累加宽度。

**注意**：`sum(1 for ...)` 比先建一个列表再 `len()` 更省内存，但差别通常可以忽略；写代码时首选清晰。

### 8C.13 边遍历边修改容器

这是一个经典陷阱：

```python
numbers = [1, 2, 3, 4]
for n in numbers:
    if n % 2 == 0:
        numbers.remove(n)
print(numbers)      # [1, 3]？
```

看起来没问题，实际上可能会漏删。因为 `for` 用的是下标在走，`remove` 会让后面的元素前移，导致“跳过”一个。

**三种安全写法**：

```python
# 写法一：遍历副本
for n in list(numbers):
    if n % 2 == 0:
        numbers.remove(n)

# 写法二：切片副本
for n in numbers[:]:
    ...

# 写法三：直接重建（最推荐）
numbers = [n for n in numbers if n % 2 != 0]
```

**项目中的用法**：

- `game._update_powerups`：一边倒计时一边可能要 `remove`，所以 `for powerup in list(self.powerups)`；
- `game._remove_dead`：直接用推导式重建：`self.enemies = [e for e in self.enemies if e.alive]`；
- `game._hit_player`：标记子弹死亡，而不是当场 `remove`，统一留给 `_remove_dead`。

**和 6.7 的联系**：`list(lst)` 是**浅拷贝**——新列表，但里面的元素对象还是同一批。如果你在循环里修改的是“元素的属性”而不是“列表本身”，浅拷贝完全够用。

### 8C.14 常用标准库速览

标准库是 Python 自带的功能包，`import` 就能用，不需要安装。

**`random`：随机数**

| 函数 | 作用 | 项目中的例子 |
|------|------|--------------|
| `random.randint(a, b)` | 闭区间随机整数 | `battlefield._find_blank_area` 随机选位置 |
| `random.random()` | `[0, 1)` 随机浮点 | `_add_brick_blocks` 按概率决定放不放砖 |
| `random.choice(seq)` | 从序列随机取一个 | `game._drop_powerup` 选格子 |
| `random.shuffle(seq)` | 原地打乱 | `enemy._choose_path` 打乱备选方向 |
| `random.uniform(a, b)` | 区间随机浮点 | 敌人开火间隔的抖动 |
| `random.choices(pop, weights=w, k=n)` | 带权重抽取 | `game._pick_enemy_type` 按关卡权重抽敌人类型 |

`random.choices` 是“抽卡”最常用的函数：

```python
names = ["basic", "fast", "armor"]
weights = [8, 3, 1]
kind = random.choices(names, weights=weights, k=1)[0]
```

**`time`：时间**

| 函数 | 用途 | 项目中的例子 |
|------|------|--------------|
| `time.perf_counter()` | 高精度、只增不减的计时器，适合测“两次调用之间过了多久” | 主循环计算 `dt` |
| `time.monotonic()` | 单调时钟，不会因为系统改时间而回退 | 输入解析的“ESC 过期”判断 |
| `time.sleep(s)` | 睡眠 | 主循环让出 CPU |

**为什么不用 `time.time()`**：它返回“墙上的钟”（1970 年至今的秒数），系统对时或用户改时间时可能跳变。游戏循环如果拿它算 `dt`，会突然出现负数或巨大值。

**`pathlib`：文件路径**

```python
from pathlib import Path

path = Path(__file__).resolve().parent / "highscore.txt"
text = path.read_text(encoding="utf-8")   # 读全部内容
path.write_text("123\n", encoding="utf-8")  # 写内容
```

`read_text` / `write_text` 是“一行代码完成打开→读写→关闭”的语法糖（内部还是用 `open`）。7.9 节用的 `open(...)` 更底层，适合按行流式处理大文件；小文件用 pathlib 更省事。

**`shutil.get_terminal_size()`：终端尺寸**

```python
import shutil

size = shutil.get_terminal_size((80, 24))   # 拿不到时用默认值
print(size.columns, size.lines)
```

`main.py` 用它检查终端够不够大；`display.Renderer` 用它检测窗口有没有被缩放（缩放后强制整屏重画）。

**`unicodedata.east_asian_width(ch)`：字符宽度**

```python
import unicodedata

unicodedata.east_asian_width("砖")   # 'W'（Wide，宽）
unicodedata.east_asian_width("R")    # 'Na'（Narrow，窄）
```

返回值 `"W"` 或 `"F"` 表示终端里占 2 列。`display.display_width` 用它计算字符串的实际显示宽度，再决定补几个空格——这是中文终端程序不“歪行”的关键。

### 8C.15 终端进阶：ANSI 定位、清行、备用屏幕

改写前的第 8 节 讲过颜色、清屏、光标显隐。实时游戏还需要更多控制：

| 序列 | 作用 | 项目中的用途 |
|------|------|--------------|
| `\033[{row};{col}H` | 光标移动到第 row 行第 col 列（从 1 开始） | 差分刷新时定位到某一行 |
| `\033[K` | 清除光标到行尾的内容 | 新行比旧行短时擦掉剩余字符 |
| `\033[2J` | 清屏 | 初始化 |
| `\033[H` | 光标回左上角 | 初始化 |
| `\033[?25l` / `\033[?25h` | 隐藏 / 显示光标 | 游戏期间隐藏 |
| `\033[?1049h` / `\033[?1049l` | 进入 / 退出备用屏幕 | 不污染用户的终端历史 |

**raw 模式下的换行坑**：正常情况下终端会把 `\n` 自动翻译成“回车 + 换行”；raw 模式关闭了这个翻译，`\n` 只往下走、不回到行首。本项目用**绝对定位**（`\033[{row};1H`）写每一行，所以完全绕开了这个问题；如果你用 `print()` 逐行输出，就要写 `\r\n`。

**`print` vs `sys.stdout.write`**：`print` 会自动加换行并刷新缓冲。渲染器需要把多段内容拼成一次写出（减少终端闪烁），所以用 `sys.stdout.write("".join(out))` 再手动 `flush()`。

**Windows 的兼容问题**：Windows 10 以前的控制台默认不识别 ANSI 序列，需要打开“虚拟终端处理”：

```python
import ctypes

kernel32 = ctypes.windll.kernel32
handle = kernel32.GetStdHandle(-11)          # 标准输出
mode = ctypes.c_uint32()
if kernel32.GetConsoleMode(handle, ctypes.byref(mode)):
    kernel32.SetConsoleMode(handle, mode.value | 0x0004)   # 0x0004 = VT 模式
```

这段代码在 `display.enable_windows_ansi()` 里，通过 `os.name != "nt"` 判断只在 Windows 上执行。`ctypes` 属于“平台胶水”，看不懂细节没关系——知道“这是为了打开 ANSI 支持”就够了。

### 8C.16 非阻塞输入与平台差异

`input()` 是**阻塞**的：程序会停在那一行，直到用户输入一整行并回车。实时游戏需要“随时问一句：现在有按键吗？没有就继续跑”。

**Unix（Linux/macOS/WSL）**：

```python
import os, select, sys, termios, tty

fd = sys.stdin.fileno()               # 文件描述符
old = termios.tcgetattr(fd)           # 保存终端设置
tty.setraw(fd)                        # 切到 raw 模式
try:
    ready, _, _ = select.select([sys.stdin], [], [], 0)   # 0 秒超时 = 立刻返回
    if ready:                          # 有数据才读
        data = os.read(fd, 64)         # 按字节读，不会卡住
        text = data.decode("utf-8", errors="ignore")
finally:
    termios.tcsetattr(fd, termios.TCSADRAIN, old)   # 恢复终端
```

要点：

- raw 模式下终端不再做行缓冲和回显，按键立刻变成字节；
- `select` 只负责“问一下有没有数据”，不会等待；
- `os.read` 读的是**字节**（`bytes`），要用 `.decode()` 转成字符串；
- `errors="ignore"` 表示遇到解码不了的字节就跳过，不让程序崩；
- 终端的设置**必须恢复**（`finally` 或 `with`）。

**Windows**：

```python
import msvcrt

while msvcrt.kbhit():                 # 有按键吗？
    ch = msvcrt.getwch()              # 读一个字符（Unicode）
    if ch in ("\x00", "\xe0"):        # 功能键/方向键的第一字节
        code = msvcrt.getwch()        # 第二个字节
        ...
```

**方向键的统一表示**：不同平台给出的原始数据不一样（Unix 是 `\x1b[A` 这样的转义序列，Windows 是 `\xe0` + 一个码），但都会被 `input_handler.py` 翻译成同样的动作名 `"up"`。这就是“**在模块边界处统一数据格式**”的价值：换平台只改一个文件。

### 8C.17 `os.name` 与 `sys.platform`

```python
import os, sys

os.name         # "nt"（Windows）/ "posix"（Linux/macOS）
sys.platform    # "win32" / "linux" / "darwin"
```

`os.name` 只有两个值，适合“Windows 还是类 Unix”这种粗判断；`sys.platform` 更细，可以区分 macOS 和 Linux。项目里用的是：

```python
if os.name == "nt":
    import msvcrt
else:
    import termios
    import tty
```

**注意**：平台判断应该只出现在最外层的适配模块里（`input_handler.py`、`display.enable_windows_ansi`），游戏逻辑里不要到处写 `if os.name == "nt"`。

### 8C.18 `signal` 与优雅退出

`signal` 模块让程序能响应操作系统的信号（比如 `kill`、终端关闭）。

```python
import signal

def _handler(signum, frame):
    raise KeyboardInterrupt

signal.signal(signal.SIGINT, _handler)    # Ctrl+C
signal.signal(signal.SIGTERM, _handler)   # kill
```

`main.py` 用它做两件事：

1. 当程序被 `kill` 或收到 Ctrl+C 时，把控制流变成 `KeyboardInterrupt`；
2. 这样 `try/finally` 就有机会执行 `renderer.stop()`、恢复终端设置。

有意思的是：在 Unix 的 raw 模式下，终端不再把 Ctrl+C 翻译成信号（`ISIG` 被关掉了），所以你收到的是字节 `\x03`，由 `input_handler` 映射成 `"quit"`。**两条路都能退出，是因为层与层各自把自己那份工作做好了。**

### 8C.19 固定步长循环：让速度不受电脑快慢影响

先看一个错误的写法：

```python
while True:
    dt = compute_dt()
    enemy.move_one_step()      # 每帧走一格
    render()
```

问题：电脑快，帧率高，敌人就快；电脑慢，敌人就慢。两台电脑玩同一个游戏，速度不一样。

**固定步长（fixed timestep）** 的做法：把逻辑更新切成一模一样的小片。

```python
LOGIC_STEP = 1 / 120      # 每一片 8.3 毫秒
accumulator = 0.0
last = time.perf_counter()

while running:
    now = time.perf_counter()
    accumulator += min(now - last, 0.25)   # 这轮过去了多少秒（封顶防快进）
    last = now

    while accumulator >= LOGIC_STEP:       # 攒够一片就更新一次
        game.update(LOGIC_STEP)
        accumulator -= LOGIC_STEP

    if now >= next_frame:                  # 渲染按自己的节奏走
        draw()
        next_frame = now + FRAME_TIME
```

好处：

- **速度一致**：无论电脑快慢、无论一帧里循环几次，游戏时间都是真实的；
- **可复现**：同样的输入，结果基本可预测，方便调试和测试；
- **逻辑与渲染解耦**：逻辑 120 Hz，渲染 60 Hz，互不影响。

项目里 `game.update(dt)` 的每一个动作（移动计时、子弹计时、护盾倒计时）都围绕 `dt` 写，所以替换 `LOGIC_STEP` 的大小不会改变游戏速度，只会改变精度。

`min(now - last, 0.25)` 是必要的保险：如果系统休眠了 10 秒，差值 10 会导致内层 `while` 补 1200 次更新，游戏“快进”还越补越卡。封顶 0.25 秒等于“卡顿就丢弃这段游戏时间”。

### 8C.20 帧差分渲染

旧版渲染每帧：

```python
clear_screen()          # 清掉整屏
打印 19 行
```

30/60 FPS 下这样会闪，而且每帧要写几千个字符，终端忙不过来。

本项目的思路：**只重画变化的行**。

```python
# 第一帧：缓存为空，所有行都“变了”，全部写一遍
# 第二帧：只有玩家所在行和 HUD 变了 → 只写那两行
for index, line in enumerate(lines):
    if line != self._lines[index]:
        write(f"\033[{index + 1};1H{line}\033[K")
self._lines = list(lines)
```

代价是需要保存“上一帧的每一行”；收益是静止的墙面完全不产生输出。实测单帧渲染成本约 0.2 毫秒，60 FPS 毫无压力。

**一个必须处理的边界**：终端窗口大小改变后，某些行的内容可能没变，但位置/宽度变了，缓存里旧的“没变”判断会导致花屏。所以 `draw()` 会先比较终端尺寸，不同就清空缓存、强制整屏重画。

差分思想不只用在终端：前端的 Virtual DOM、数据库的增量备份、Git 的 diff，本质都是“**只处理变化的部分**”。

---

### 8C.21 补充知识清单回顾

把上面这些“新面孔”再列一次，并对应到项目文件，方便你回查：

| 新知识 | 一句话 | 到哪看 |
|--------|--------|--------|
| 类/对象/self/`__init__` | 数据+行为的打包 | `tank.py`、`bullet.py` |
| 继承/`super()`/多态 | 子类复用父类，接口一致 | `player.py`、`enemy.py` |
| `@property` | 计算属性、只读字段 | `player.py` |
| 类属性 vs 实例属性 | 共享默认值 vs 各自一份 | `tank.py` |
| `_name` 约定 | 内部接口标记 | 几乎每个文件 |
| 异常处理 | 出错时的恢复路径 | `game.py` 存档、`main.py` |
| `with` / 上下文管理器 | 进入准备、退出收尾 | `input_handler.py` + `main.py` |
| 类型注解进阶 | 容器泛型、联合类型 | 所有签名 |
| 条件表达式 | 一行二选一 | `enemy.py`、`display.py` |
| 链式比较 | `0 <= x < n` | `battlefield.py`、`input_handler.py` |
| f-string 格式符 | 补零/对齐/精度 | `display.py` |
| `any/all/sum` | 聚合判断 | `game.py` |
| 遍历副本 | 边遍历边删的安全姿势 | `game.py` |
| `random.choice/choices` | 抽卡/加权随机 | `game.py`、`enemy.py` |
| `time.perf_counter` | 高精度计时 | `main.py` |
| `pathlib` | 路径与读写 | `config.py`、`game.py` |
| `shutil.get_terminal_size` | 终端尺寸 | `main.py`、`display.py` |
| `unicodedata` | 中日韩字符宽度 | `display.py` |
| ANSI 定位/清行/备用屏 | 终端“画笔” | `display.py` |
| 非阻塞输入 | 不卡死的键盘读取 | `input_handler.py` |
| 平台判断 | Windows/Unix 分流 | `input_handler.py` |
| `signal` | 优雅退出 | `main.py` |
| 固定步长 | 逻辑与帧率解耦 | `main.py` |
| 帧差分 | 只画变化的部分 | `display.py` |

---

语法补齐了。最后一节（8D）讲两件事：拿到一个陌生项目时怎么系统地读，以及用 8 个练习把 TankBattle 变成你自己的项目。
