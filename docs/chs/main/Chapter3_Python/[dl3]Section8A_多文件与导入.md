# 第8A节：Python 多文件与导入机制

前面 7 节，你的程序都是单文件：一个 `.py` 里放几十到几百行，从 `input()` 读数据，算完 `print()` 出来。从这一节开始，我们换一种方式学习：**不再从零写一个玩具，而是读一个真实的多文件项目**——随书示例 **TankBattle**（终端实时坦克大战）。它一共 11 个 Python 模块、约 1600 行代码，能在终端里以 60 FPS 跑起来，有基地、有敌人 AI、有道具、有关卡。完整代码在下载区的 `TankBattle.zip` 里。

> **第 8 部分（8A–8E）的目标**
>
> 1. **8A**：学会 Python 的模块、导入、分层与依赖管理（本节）；
> 2. **8B**：剖析 TankBattle——每个模块为什么存在，依赖关系和数据流是怎样的；
> 3. **8C**：补齐前 7 节没有讲过、但项目里大量使用的知识（类与继承、`@property`、异常与 `with`、类型注解进阶、常用标准库、终端控制……）；
> 4. **8D**：学会系统地阅读一个陌生项目，并动手扩展它；
> 5. **8E（选学，不是必须掌握）**：为项目写单元测试与集成测试。
>
> **关于代码量**：项目约有 1600 行，8A–8E 不会全部贴出来。每节只给**关键片段**，重点放在分析上。建议打开对应的代码文件对照阅读。
>
> 这一部分文本量非常大，建议分多天读完。

---

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 模块 | 一个 `.py` 文件，包含可以被其他文件导入的代码 |
| 包 | 一个包含 `__init__.py` 的文件夹，里面可以有多个模块 |
| 命名空间 | 变量和函数名的“作用域”，避免不同模块之间的命名冲突 |
| 入口点 | 程序开始执行的文件，本项目是 `main.py` |
| 依赖 | 一个模块需要使用另一个模块的功能 |
| 分层 | 把代码按职责划分成不同层级，上层依赖下层，下层不依赖上层 |
| 关注点分离 | 每个模块只负责一件事，不混合不相关的逻辑 |
| 接口 | 模块对外暴露的函数/类/属性，别人只通过接口使用它 |
| 重构 | 在不改变程序外部行为的前提下，调整代码结构 |
| 循环导入 | A 导入 B、B 又导入 A，Python 会报错或得到不完整的模块 |
| 依赖倒置 | 需要别人时不在模块顶部导入，而是“让调用者把对象传进来” |
| 上下文管理器 | 支持 `with` 语句的对象，负责“进入时准备、退出时收尾” |
| 鸭子类型 | 不检查对象是什么类，只看它有没有需要的方法/属性 |
| 数据驱动 | 用数据（字典、表）描述规则，而不是写一堆 `if/elif` |
| 固定步长 | 逻辑更新使用固定时间片（如 1/120 秒），与帧率无关 |
| 帧差分 | 渲染时只重画发生变化的部分，减少终端输出 |

---

## 8A.1 为什么要把代码拆成多文件

### 8A.1.1 单文件是怎么失控的

假设把 TankBattle 的 1600 行全部放进 `main.py`。你可以先感受一下会发生什么：

**问题一：找代码变慢**

你想把“子弹飞行速度”调快一点，于是打开文件——前面是常量、颜色、地形生成、地图随机数；中间是坦克类和 AI；后面是子弹、道具、计分；最后才是渲染。你滚动了好几屏，才在一个函数体中间找到那个 `0.04`。

**问题二：改一处，崩一片**

所有函数共享同一个命名空间。你在“子弹”函数里写了一个 `row`，在“地图生成”里也写了一个 `row`；它们各在自己的作用域里，不会互相干扰，但**模块级的变量和函数定义会互相覆盖**：你后定义了一个 `update()`，前面的 `update()` 就永远消失了。更常见的是，你改了 `update()` 的行为，另一个调用它的地方立刻出错。

**问题三：多人协作冲突**

两个人同时改一个 1600 行的文件，Git 合并时冲突一片。哪怕改的是不相干的两段代码，也会因为“都在同一个文件里”而互相干扰。

**问题四：复用困难**

你写了一个很好用的“终端双宽字符对齐”函数，想用到另一个项目里。可它和游戏主循环、颜色表混在一起，你只能整个文件复制，或者手动把那一小段抠出来。

**问题五：测试困难**

你想单独验证“按下一个键后，玩家的状态对不对”，但输入处理、游戏状态、渲染是缠在一起的，跑一次就要启动整个游戏。想测“子弹穿墙吗”，还得先造一张地图。

### 8A.1.2 拆分之后得到了什么

TankBattle 的实际数字：

| 模块 | 行数 | 它管什么 |
|------|------|----------|
| `game.py` | 450 | 游戏状态机、碰撞、刷怪、道具、计分 |
| `display.py` | 213 | 把游戏数据画到终端 |
| `battlefield.py` | 189 | 生成地图、判断某格能不能走 |
| `config.py` | 148 | 全部常量：尺寸、速度、颜色、敌人属性 |
| `input_handler.py` | 126 | 跨平台非阻塞键盘输入 |
| `enemy.py` | 114 | 敌人 AI |
| `main.py` | 103 | 入口：主循环、收尾 |
| `player.py` | 94 | 玩家坦克 |
| `tank.py` | 78 | 坦克基类 |
| `bullet.py` | 24 | 子弹 |
| `powerup.py` | 18 | 道具 |

拆分带来的好处可以具体验证：

- **改速度**：只动 `config.py` 里 4 个数字（`PLAYER_MOVE_INTERVAL` 等），别的文件一行不改；
- **换外观**：只动 `config.py` 里的 `TERRAIN_GLYPHS`、`TERRAIN_COLORS` 两个字典；
- **换输入方式**：只动 `input_handler.py`，游戏逻辑完全不知道按键是怎么读出来的；
- **让敌人更聪明**：只改 `enemy.py`，地图生成和渲染不受影响；
- **单独测试规则**：`battlefield.py`、`bullet.py` 都可以不启动终端直接 import 运行。

### 8A.1.3 一个类比：从杂物间到工具箱

单文件像一间杂物间：所有东西堆在一起，找一把螺丝刀要翻半天，拿东西时还可能碰倒旁边的箱子。

多文件像一个工具箱：螺丝刀在一个抽屉，扳手在另一个抽屉，每个抽屉只放一类工具。你想用什么，直接开对应抽屉，不会影响别的东西。

代价是：你需要提前规划“每个抽屉放什么”。这就是**架构**——在写代码之前，先想清楚模块怎么划分。

### 8A.1.4 我们的样本：TankBattle 的演化

这个项目不是一开始就长这样的。它的演化史正好说明“拆分”的价值：

1. **第一版（回合制）**：`main.py` 里是一个 `while` 循环，按一次键才走一格、子弹一帧一飞，用的是阻塞式 `get_key()`。它已经拆成了 9 个文件，但游戏性很差。
2. **第二版（实时版，8B 分析的对象）**：改成 60 FPS 实时游戏，加入了基地、关卡、道具、子弹对撞、最高分。改动方式是：
   - **新增** `game.py`（游戏状态机和规则）和 `powerup.py`（道具）；
   - **重写** `display.py`（从“每帧清屏重画”改成“差分刷新”）、`input_handler.py`（从阻塞读键改成非阻塞轮询）、`config.py`（常量表化）；
   - **小改** `player.py`、`enemy.py`、`bullet.py`（适配实时的时间参数）；
   - `battlefield.py` 的“生成地图”接口几乎没动。

注意最后一点：**升级游戏性没有导致推倒重来**，因为地图、显示、输入各自都在自己的抽屉里。这就是拆分最实际的回报。

---

## 8A.2 Python 模块与导入机制

### 8A.2.1 导入整个模块

假设同目录下有两个文件：

**math_utils.py**：

```python
"""Math utility functions."""

def add(a: int, b: int) -> int:
    """Return the sum of two numbers."""
    return a + b

PI = 3.14159
```

**main.py**：

```python
import math_utils

print(math_utils.add(3, 5))   # 8
print(math_utils.PI)          # 3.14159
```

`import math_utils` 会把整个文件加载进来，使用时加 `math_utils.` 前缀。

### 8A.2.2 从模块导入名字

```python
from math_utils import add, PI

print(add(3, 5))
print(PI)
```

不用前缀，但**命名冲突的风险变高**：如果当前文件也有一个 `add`，导入进来的会覆盖它。

项目的选择：**每个文件都尽量只导入自己真正要用的名字**，并且把名字写清楚。例如 `player.py`：

```python
from config import (
    DELTAS, FIRE_HOLD_TIMEOUT, HOLD_GRACE_MAX, HOLD_TIMEOUT_MIN,
    PLAYER_BULLETS_BY_POWER, PLAYER_BULLET_SPEED_BY_POWER, PLAYER_CHAR,
    PLAYER_COLOR, PLAYER_MOVE_INTERVAL, PLAYER_SHOOT_COOLDOWN,
    REPEAT_MAX_GAP, SPAWN_SHIELD_TIME, TAP_HOLD_TIMEOUT,
)
from tank import Tank
```

看这个导入表就知道：`player.py` 依赖 `config` 和 `tank`，不依赖 `game`、不依赖 `display`。

### 8A.2.3 给导入起别名

```python
import math_utils as mu
from math_utils import add as plus
```

别名常用于简化长模块名，或解决命名冲突。本项目里没有用别名，因为模块名都很短。

### 8A.2.4 多行导入：工程里的常见写法

一个模块要用几十个常量时，单行导入会很长。用括号折行，每行末尾加逗号（**trailing comma**）：

```python
from config import (
    FRAME_TIME, INPUT_POLL_SLEEP, LOGIC_STEP, MIN_COLS, MIN_LINES,
    STATE_GAME_OVER,
)
```

这种写法的好处：

- 加常量时只加一行，Git diff 干净；
- 顺序稳定，方便 review；
- 结尾逗号让“再加一行”不用改上一行。

### 8A.2.5 `__name__` 与入口点

第 4.11 节讲过：每个文件的 `__name__` 取决于它怎么被执行。

- 直接 `python main.py` → `main.py` 里 `__name__ == "__main__"`
- 被别的文件 `import main` → `main.py` 里 `__name__ == "main"`

TankBattle 的 `main.py` 结尾：

```python
if __name__ == "__main__":
    main()
```

这句话的意思是：“**只有直接运行这个文件时才启动游戏**”。以后如果写测试，`import main` 不会把游戏跑起来；如果做打包工具，也不会在导入时产生副作用。

### 8A.2.6 包（Package）与目录

当模块多到需要分组时，用文件夹组织，并在文件夹里放一个 `__init__.py`：

```
project/
├── main.py
└── utils/
    ├── __init__.py
    ├── math_utils.py
    └── string_utils.py
```

导入方式：

```python
from utils.math_utils import add
```

TankBattle 没有用包，因为：

- 模块只有 11 个，都在同一层；
- 依赖关系简单（常量 → 数据 → 实体 → 协调）；
- 平铺时导入语句最短、最直观。

**什么时候该建包**：模块超过 20 个、出现明显的子领域（如 `ai/`、`render/`、`net/`）、或者几个大模块需要单独成组时。

### 8A.2.7 绝对导入与相对导入

```python
# 绝对导入：从项目根开始
from utils.math_utils import add

# 相对导入：. 当前目录，.. 上级目录，只能在包内部使用
from .math_utils import add
from ..config import DEBUG
```

新手建议优先用**绝对导入**：路径清晰，不容易被“这个 `.` 到底指哪”绕晕。本项目全部使用绝对导入。

### 8A.2.8 模块只会被加载一次

这一点非常重要：**不管多少个文件 `import config`，Python 只会执行 `config.py` 一次**，之后拿到的都是同一个模块对象。

所以：

```python
# config.py
HIGH_SCORE_FILE = Path(__file__).resolve().parent / "highscore.txt"
```

这个 `Path(...)` 只在第一次导入时算一次，所有模块共享同一个路径对象——这正是我们想要的。

反过来说，**模块顶层不要写有副作用的代码**。如果 `config.py` 顶层写了一个 `print("config loaded")`，那它在整个程序里只会打印一次，但会在一个你意想不到的时机（第一次 import 时）发生。TankBattle 的 `config.py` 顶层只有常量和字典，没有副作用；`main.py` 的副作用（启动游戏）被 `if __name__ == "__main__"` 保护着。

### 8A.2.9 循环导入：多文件项目最常见的坑

**什么是循环导入**：A 导入 B，B 又导入 A。

```python
# a.py
import b

def hello():
    return "hello from a"

# b.py
import a          # ← 循环

def world():
    return a.hello() + " and b"
```

直接运行 `python a.py`：

```
ImportError: cannot import name 'hello' from partially initialized module 'a'
```

原因：`a` 执行到 `import b` 时开始加载 `b`；`b` 第一行又 `import a`，但此时 `a` 只执行了一半（`hello()` 还没定义），拿到的就是“半成品”模块。

**三种解法**：

1. **把共用的东西下沉到更底层的模块**。本项目所有常量和颜色都在 `config.py`，它不依赖任何业务模块，谁都可以导入它。
2. **函数内部延迟导入**。旧版 `player.py` 里就是这么写的：
   ```python
   def shoot(self) -> "Bullet":
       from bullet import Bullet      # 放到函数里，调用时才导入
       ...
   ```
3. **依赖倒置：让调用者把对象传进来**。这是本项目的核心技巧，重点看 `tank.py`：

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

`tank.py` **没有** `from game import Game`。它需要一个“能判断能不能进”的对象，于是直接把它作为参数 `game` 接收——只要传进来的对象有 `can_tank_enter()` 方法就行（鸭子类型）。这样：

- 依赖方向永远是 `game → tank`，不会反过来；
- `tank.py` 可以独立导入、独立测试（传一个假对象就能跑）；
- 子弹、敌人、玩家全都遵守同一条规则。

**本项目的依赖方向规则**：

```
config  ←  所有模块都可以依赖它
battlefield / bullet / powerup / tank  ←  不知道 game 的存在
player / enemy  ←  只依赖 tank + config
game  ←  知道所有实体，实体不知道 game
display / input_handler  ←  完全不知道 game 里发生什么（display 只读传进来的对象属性）
main  ←  唯一知道所有人的模块
```

**一个小提问**：`display.py` 需要画 `game.player`、`game.enemies`、`game.grid`……它为什么不 `from game import Game`？

因为 `render_frame(game)` 只是把 `game` 当参数传进来。`display.py` 里没有任何一行 `import game`——它只要求传进来的对象有那些属性。这叫**鸭子类型**：“如果它走起来像鸭子、叫起来像鸭子，那就当它是鸭子”。好处是测试渲染时，你可以造一个只有 `state`、`player`、`grid` 属性的假对象，不需要启动整个游戏。

### 8A.2.10 标准库、第三方库与本地模块

`import` 的三种来源：

| 来源 | 例子 | 本项目用了哪些 |
|------|------|----------------|
| Python 标准库 | `random`、`time`、`pathlib`、`select` | `random`、`time`、`pathlib`、`shutil`、`unicodedata`、`os`、`sys`、`select`、`signal` |
| 第三方库 | `numpy`、`requests`、`pygame` | **一个都没有**（零依赖，装好 Python 就能跑） |
| 本地模块 | 自己写的 `.py` | `config`、`battlefield`、`tank`、`game`…… |

“零第三方依赖”是一个很值钱的特性：换一台电脑，只要有 Python 就能运行，不用 `pip install` 任何东西。

---

到这里，模块与导入的规则你已经掌握了。下一节（8B）把镜头拉近：看 TankBattle 是怎么用这些规则搭起来的。
