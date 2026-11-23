# 第8E节：单元测试与集成测试

> **这一节不是必须掌握的。** 你完全可以跳过它：只要程序能跑、你能改，本阶段的目标就已经达到了。测试是等你开始“怕把旧功能改坏”时才会觉得有用的工具。现在看不懂，先知道有这回事也行；以后需要了再回来读。

8D 的结尾留了一个问题：代码写完、也读懂了，怎么确定下次修改没有把原来的功能弄坏？

到目前为止，你的验证方式都是**手工测试**：改一行代码，打开游戏，按一遍按键，看看对不对。程序小的时候这没问题。但 TankBattle 有 11 个模块、1600 行代码，手工测试会越来越累，也越来越不可靠。

这一节讲两件事：

- **单元测试**：单独检查一个函数或一个类，比如“`in_bounds(-1, 0)` 必须返回 `False`”；
- **集成测试**：把多个模块拼起来检查，比如“玩家开一枪，敌人应该死、分数应该加 100”。

用的还是 8D 练习七里那三个函数，外加完整的 TankBattle 场景。所有代码都在标准库范围内，不需要安装任何东西。

---

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 测试 | 一段自动检查程序行为的代码 |
| 测试用例 | 一个具体的检查场景，比如“负数坐标应该越界” |
| 断言 | 一句话声明“这里应该是……”，不成立就报错 |
| 单元测试 | 单独测试一个函数/类，尽量不牵扯其他模块 |
| 集成测试 | 把多个模块拼起来一起测 |
| 回归 | 以前正常、改完之后坏了 |
| 测试替身 | 用来替代真实依赖的假对象（本项目叫 fake） |
| 无头运行 | 不需要终端、不需要按键，直接在内存里跑逻辑 |
| 冒烟测试 | 只求“能跑、不崩”的粗粒度测试 |
| 模糊测试 | 随机输入乱打，检查程序仍然保持合法状态 |
| 覆盖率 | 测试执行到的代码占总代码的比例 |
| TDD | 测试驱动开发：先写测试，再写实现（了解即可） |

---

## 8E.1 为什么可以不写，为什么还是建议看看

### 8E.1.1 手工测试有多累

假设你想把玩家移动速度从 `0.11` 调到 `0.13`。手工测试的流程大概是：

1. 启动游戏，等 1.8 秒的“准备”结束；
2. 按住方向键，感觉一下速度；
3. 开一枪，确认子弹还能打中敌人；
4. 退出，改回 `0.11` 再感觉一次；
5. 第二天你改了敌人 AI，又要把上面的事情重做一遍。

改一个数，验证五分钟；改十个地方，验证一晚上。而且人是会偷懒的：你多半只测自己刚改的那部分，不会每次都把基地、道具、暂停、过关全部走一遍——**旧功能就是这么悄悄坏掉的**。

### 8E.1.2 两个真实的小故事

开发 TankBattle 的过程中，有两个 bug 特别典型：

**故事一：子弹打到基地时程序崩溃**

`game.py` 里判断“子弹是否击中基地”的代码用到了一个常量 `BASE`，它必须从 `config.py` 导入。一次整理导入语句时，`BASE` 被误删了。这个 bug 平时完全看不出来——直到敌人的子弹穿过层层砖墙打到老鹰，程序才抛出一个 `NameError`。

**故事二：道具掉落的隐藏代码**

掉落道具时需要从所有可走的格子里挑一个。当时的代码写了 `p.position`，但 `PowerUp` 类其实只有 `row` 和 `col`，没有 `position` 属性。这个 bug 的触发条件更苛刻：要**先打死 4 个敌人**，才可能掉落道具。

这两个 bug 的共同点是：**手工测试要几分钟甚至几局才能碰到，而测试只要几毫秒就能暴露**。故事一对应的集成测试会在本节 8E.5.4 出现，故事二对应 8E.5.6。

> 测试不是用来“证明程序没有 bug”的，而是用来“让 bug 自己撞上来的”。

### 8E.1.3 什么时候确实不用写

测试也有成本：测试代码要跟着程序一起维护，改接口时测试也要改。下面这些情况，不写测试是合理的：

- 一次性的小脚本，跑完就扔；
- 课堂演示程序，只要演示那几分钟能跑；
- 玩法还在探索中的原型，接口一天变三次；
- 你自己还在学语法，写测试会打击信心。

判断标准很简单：**如果这个程序你还要改很多次，或者要交给别人用，测试就开始划算**。TankBattle 恰好属于这种——它有 11 个模块、会不断加功能，所以值得为它写测试。

---

## 8E.2 最朴素的测试：assert

8C.6 提过一句话：`assert` 就是“不满足就抛异常”的语句。它是最小的测试工具：

```python
from battlefield import in_bounds

assert in_bounds(0, 0)          # 地图左上角，必须为 True
assert not in_bounds(-1, 0)     # 行号 -1，必须为 False
assert not in_bounds(0, 26)     # 列号越界，必须为 False

print("全部通过")
```

如果所有断言都成立，程序打印“全部通过”；如果有一个不成立：

```
Traceback (most recent call last):
  File "demo.py", line 3, in <module>
    assert not in_bounds(-1, 0)
           ^^^^^^^^^^^^^^^^^^^
AssertionError
```

注意两点：

- `assert` 只在“调试模式”有效——用 `python -O` 运行时会被全部忽略，所以**不要用 assert 做参数校验**（别拿它检查用户输入）；
- 用它写测试非常方便，但原生的 assert 有三个缺点：第一个失败就停住、没有通过/失败的统计、不能单独运行某一个检查。

下一节的 `unittest` 把这些问题都解决了。

---

## 8E.3 unittest：标准库自带的测试框架

### 8E.3.1 第一个测试文件

```python
"""Unit tests for battlefield.in_bounds."""

import unittest

from battlefield import in_bounds


class TestInBounds(unittest.TestCase):
    """in_bounds() must accept the whole map and reject everything else."""

    def test_inside_is_true(self):
        self.assertTrue(in_bounds(0, 0))

    def test_outside_is_false(self):
        self.assertFalse(in_bounds(-1, 0))


if __name__ == "__main__":
    unittest.main()
```

把文件保存成 `test_in_bounds.py`，在 TankBattle 目录下运行：

```bash
py -m unittest test_in_bounds -v
```

（Linux / macOS / WSL 上写 `python3 -m unittest test_in_bounds -v`。）

输出：

```
test_inside_is_true (test_in_bounds.TestInBounds.test_inside_is_true) ... ok
test_outside_is_false (test_in_bounds.TestInBounds.test_outside_is_false) ... ok

----------------------------------------------------------------------
Ran 2 tests in 0.000s

OK
```

需要记住三条规则：

1. 测试类要**继承 `unittest.TestCase`**；
2. 测试方法名必须**以 `test_` 开头**，否则会被忽略；
3. 断言用 `self.assertXxx(...)` 方法，而不是裸 `assert`——失败时它会打印更清楚的信息，也方便工具统计。

### 8E.3.2 常用断言方法

| 方法 | 含义 | 例子 |
|------|------|------|
| `assertEqual(a, b)` | a 等于 b | `self.assertEqual(score, 100)` |
| `assertNotEqual(a, b)` | a 不等于 b | |
| `assertTrue(x)` / `assertFalse(x)` | x 为真 / 为假 | `self.assertTrue(player.alive)` |
| `assertIn(x, seq)` / `assertNotIn` | x 在 / 不在序列里 | `self.assertIn("up", keys)` |
| `assertIsNone(x)` / `assertIsNotNone(x)` | x 是 / 不是 None | |
| `assertAlmostEqual(a, b)` | 浮点数近似相等 | `self.assertAlmostEqual(x, 0.11, places=2)` |
| `assertGreater(a, b)` / `assertLess` | 大小比较 | |
| `assertRaises(异常类型)` | “这里应该抛异常” | 见 8E.4.3 |

### 8E.3.3 运行测试

| 目的 | 命令（在 TankBattle 目录下） |
|------|------------------------------|
| 自动发现并运行当前目录所有测试 | `py -m unittest -v` |
| 只运行某个文件 | `py -m unittest test_battlefield -v` |
| 只运行某个类 | `py -m unittest test_battlefield.TestInBounds -v` |
| 测试放在 `tests/` 子目录 | 先建一个空的 `tests/__init__.py`，再运行 `py -m unittest discover -s tests -v` |

**为什么必须在项目根目录运行？** 因为测试文件里有 `from battlefield import in_bounds`。Python 查找模块时会把“当前目录”加进搜索路径；如果在别的目录运行，它就找不到 `battlefield.py` 了。

### 8E.3.4 失败是什么样

把断言故意写错，运行 `py -m unittest tests.test_demo_fail -v`：

```
test_example (tests.test_demo_fail.TestDemo.test_example) ... FAIL

======================================================================
FAIL: test_example (tests.test_demo_fail.TestDemo.test_example)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "tests/test_demo_fail.py", line 8, in test_example
    self.assertEqual(2 + 2, 5)
    ~~~~~~~~~~~~~~~~^^^^^^^^^^
AssertionError: 4 != 5

----------------------------------------------------------------------
Ran 1 test in 0.003s

FAILED (failures=1)
```

读失败信息的方法：

1. 找到 `FAIL:` 后面那一行，它告诉你**哪个测试**失败；
2. 看 traceback 的最后一行，它告诉你**期望和实际差在哪**（这里是 `4 != 5`）；
3. 用 `-v` 可以看到每个测试的名字，用不带 `-v` 的默认模式则看到一排点：`. = 通过`，`F = 失败`。

### 8E.3.5 放哪里、怎么命名

| 建议 | 说明 |
|------|------|
| 文件名 `test_*.py` | 这样 `unittest` 才能自动发现 |
| 一个源文件对应一个测试文件 | `battlefield.py` → `test_battlefield.py` |
| 类名 `TestXxx` | `TestInBounds`、`TestGeneration` |
| 方法名 `test_行为_期望` | `test_blocked_move_only_turns` |
| 一个测试只验证一件事 | 失败时你能立刻知道坏的是什么 |

两种目录方案任选：

```
TankBattle/
├── battlefield.py
├── test_battlefield.py        # 方案一：测试和源码放一起，最简单
├── test_game.py
└── ...

TankBattle/
├── battlefield.py
├── tests/                     # 方案二：集中管理，多放一个 __init__.py
│   ├── __init__.py
│   ├── test_battlefield.py
│   └── test_game.py
└── ...
```

---

## 8E.4 单元测试实战

### 8E.4.1 纯函数最好测：test_battlefield.py

`battlefield.py` 里的函数几乎都是纯函数：一样的地图尺寸和坐标，永远得到一样的结果。这类函数测试起来最省心。

新建 `test_battlefield.py`：

```python
"""Unit tests for battlefield.py."""

import unittest

from battlefield import (
    base_position, create_battlefield, enemy_spawn_points, in_bounds,
    is_tank_passable, player_spawn,
)
from config import (
    BASE, BRICK, FIELD_HEIGHT, FIELD_WIDTH, FLOOR, FOREST, STEEL, WATER,
)


class TestInBounds(unittest.TestCase):
    """in_bounds() must accept the whole map and reject everything else."""

    def test_corners_and_edges(self):
        self.assertTrue(in_bounds(0, 0))
        self.assertTrue(in_bounds(FIELD_HEIGHT - 1, FIELD_WIDTH - 1))
        self.assertFalse(in_bounds(-1, 0))
        self.assertFalse(in_bounds(FIELD_HEIGHT, 0))
        self.assertFalse(in_bounds(0, -1))
        self.assertFalse(in_bounds(0, FIELD_WIDTH))


class TestTiles(unittest.TestCase):
    """Tanks may drive on floor and forest only."""

    def test_tank_passable_tiles(self):
        self.assertTrue(is_tank_passable(FLOOR))
        self.assertTrue(is_tank_passable(FOREST))
        self.assertFalse(is_tank_passable(BRICK))
        self.assertFalse(is_tank_passable(STEEL))
        self.assertFalse(is_tank_passable(WATER))


class TestGeneration(unittest.TestCase):
    """The generated map must always satisfy a few invariants."""

    def test_size_and_steel_border(self):
        grid = create_battlefield(1)
        self.assertEqual(len(grid), FIELD_HEIGHT)
        for row in grid:
            self.assertEqual(len(row), FIELD_WIDTH)
        for col in range(FIELD_WIDTH):
            self.assertEqual(grid[0][col], STEEL)
            self.assertEqual(grid[FIELD_HEIGHT - 1][col], STEEL)
        for row in range(FIELD_HEIGHT):
            self.assertEqual(grid[row][0], STEEL)
            self.assertEqual(grid[row][FIELD_WIDTH - 1], STEEL)

    def test_base_and_spawns_are_clear(self):
        for stage in (1, 3, 6):
            with self.subTest(stage=stage):
                grid = create_battlefield(stage)
                base_row, base_col = base_position()
                self.assertEqual(grid[base_row][base_col], BASE)
                for row, col in enemy_spawn_points():
                    self.assertEqual(grid[row][col], FLOOR)
                spawn_row, spawn_col = player_spawn()
                self.assertEqual(grid[spawn_row][spawn_col], FLOOR)

    def test_most_of_the_map_is_walkable(self):
        grid = create_battlefield(2)
        floor = sum(row.count(FLOOR) for row in grid)
        self.assertGreater(floor, FIELD_WIDTH * FIELD_HEIGHT // 2)


if __name__ == "__main__":
    unittest.main()
```

**这段测试里值得学的写法**

- **边界值测试**：`in_bounds` 测了六个位置——左上、右下、四条边外的越界情形。边界是最容易写错的地方。
- **`with self.subTest(stage=stage)`**：同一个测试里跑多组参数。如果某一组失败，报告会显示是哪一组，而不是笼统的“这个测试失败了”。
- **反复出现的 `self.assertEqual`**：写起来啰嗦，但失败时会打印“第几行、什么值、和期望差多少”，比 `assert` 强得多。

### 8E.4.2 有随机性的函数：测“不变量”

`create_battlefield` 每局生成的地图都不一样，所以不能写：

```python
# 错误示范：下一局地图就变了，测试必然时灵时不灵
self.assertEqual(grid[3][4], BRICK)
```

要测的是**不管随机出什么地图，都必须成立的事情**（术语叫“不变量”）：

- 地图的尺寸永远对；
- 最外圈永远是钢墙；
- 基地永远在 `base_position()` 那一格；
- 三个敌人出生点永远是空地。

上面 `test_base_and_spawns_are_clear` 和 `test_size_and_steel_border` 做的就是这件事。**随机的东西不要去猜它随机成了什么，去测它必须守住什么。**

### 8E.4.3 用假对象隔离依赖：test_player.py

`Tank.try_move(direction, game)` 需要一个 `game` 对象来判断目标格能不能走。测试时不需要真的造一个 `Game`——按 8A.2.9 讲的依赖倒置，`Tank` 只要求传进来的对象有 `can_tank_enter()` 方法。我们写一个最小的假对象（fake）：

新建 `test_player.py`：

```python
"""Unit tests for Tank and Player, using a fake game object."""

import unittest

from player import Player
from tank import Tank


class FakeGame:
    """Stand-in for Game: Tank.try_move() only needs can_tank_enter()."""

    def __init__(self, blocked=()):
        self.blocked = set(blocked)

    def can_tank_enter(self, tank, row, col):
        return (row, col) not in self.blocked


class TestTank(unittest.TestCase):
    """Movement and damage rules of the base class."""

    def setUp(self):
        self.tank = Tank(5, 5, "T", "white", hp=2,
                         move_interval=0.1, faction="test")

    def test_move_changes_position_and_direction(self):
        self.assertTrue(self.tank.try_move("right", FakeGame()))
        self.assertEqual(self.tank.position, (5, 6))
        self.assertEqual(self.tank.direction, "right")

    def test_blocked_move_only_turns(self):
        game = FakeGame(blocked={(5, 6)})
        self.assertFalse(self.tank.try_move("right", game))
        self.assertEqual(self.tank.position, (5, 5))
        self.assertEqual(self.tank.direction, "right")

    def test_invalid_direction_raises(self):
        with self.assertRaises(KeyError):
            self.tank.try_move("diagonal", FakeGame())

    def test_damage_stops_at_zero(self):
        self.tank.take_damage(10)
        self.assertEqual(self.tank.hp, 0)
        self.assertFalse(self.tank.alive)


class TestPlayerStats(unittest.TestCase):
    """Fire power is computed from the star level."""

    def test_power_level_drives_bullet_stats(self):
        player = Player(1, 1)
        self.assertEqual(player.max_bullets, 1)
        self.assertFalse(player.breaks_steel)
        player.power = 2
        self.assertEqual(player.max_bullets, 2)
        player.power = 3
        self.assertTrue(player.breaks_steel)
        self.assertGreater(player.bullet_speed, 1.0)


if __name__ == "__main__":
    unittest.main()
```

**这段测试里值得学的写法**

- **`FakeGame` 就是 8C.6 说的“测试替身”**：它不实现游戏的全部功能，只实现被测试代码需要的那一个方法。这正是 8A 里“把 `game` 当参数传进来”的回报——依赖倒置让代码更可测。
- **`setUp()` 在每个测试方法运行前都会被调用**。这里它负责造一辆新坦克，避免测试之间互相影响。注意：`setUp` 每次都会重新执行，所以每个测试都是独立的。
- **`assertRaises`** 测的是“这里**应该**报错”：
  ```python
  with self.assertRaises(KeyError):
      self.tank.try_move("diagonal", FakeGame())
  ```
  四个方向之外的输入会让 `DELTAS["diagonal"]` 抛 `KeyError`——这是有意设计的“大声报错”，测试把它固定下来。
- **测试属性联动**：`Player` 的火力属性是 `@property`（8C.3），改了 `power` 之后 `max_bullets`、`breaks_steel` 应该立刻跟着变。这类“算出来的值”特别适合用测试保护。

### 8E.4.4 协议解析：test_input_handler.py

`parse_keys` 是 8B.2.9 特意写成纯函数的：输入“缓冲区 + 两个时间戳”，输出“动作名列表 + 剩余缓冲区”，不碰终端。

新建 `test_input_handler.py`：

```python
"""Unit tests for input_handler.parse_keys (no terminal needed)."""

import unittest

from input_handler import parse_keys


class TestParseKeys(unittest.TestCase):
    """parse_keys() is a pure function: text in, action names out."""

    def test_plain_keys_are_mapped(self):
        keys, rest = parse_keys("wjq", 0.0, 0.0)
        self.assertEqual(keys, ["up", "fire", "quit"])
        self.assertEqual(rest, "")

    def test_arrow_key_sequences(self):
        keys, rest = parse_keys("\x1b[A\x1b[B\x1b[C\x1b[D", 0.0, 0.0)
        self.assertEqual(keys, ["up", "down", "right", "left"])
        self.assertEqual(rest, "")

    def test_incomplete_escape_stays_in_buffer(self):
        keys, rest = parse_keys("\x1b[", 5.0, 5.0)
        self.assertEqual(keys, [])
        self.assertEqual(rest, "\x1b[")
        keys, rest = parse_keys(rest + "A", 5.0, 5.0)
        self.assertEqual(keys, ["up"])
        self.assertEqual(rest, "")

    def test_lone_escape_needs_a_grace_period(self):
        keys, rest = parse_keys("\x1b", 10.0, 9.99)
        self.assertEqual(keys, [])
        self.assertEqual(rest, "\x1b")
        keys, rest = parse_keys("\x1b", 10.2, 10.0)
        self.assertEqual(keys, ["pause"])
        self.assertEqual(rest, "")

    def test_ctrl_c_is_quit(self):
        keys, _ = parse_keys("\x03", 0.0, 0.0)
        self.assertEqual(keys, ["quit"])


if __name__ == "__main__":
    unittest.main()
```

**为什么这些测试特别值钱**

方向键的转义序列可能被拆成两次到达、单独的 ESC 和方向键开头的 ESC 无法区分、Ctrl+C 在 raw 模式下是一个普通字节——这些情况**手工几乎测不出来**（你要在精确的时刻按下精确的键），但用纯函数写测试时只是几行字符串：

```python
keys, rest = parse_keys("\x1b[", 5.0, 5.0)      # 半截序列
self.assertEqual(keys, [])
self.assertEqual(rest, "\x1b[")                 # 应该留在缓冲区里
```

### 8E.4.5 单元测试的三条纪律

1. **每个测试独立**：不依赖别的测试先跑、不依赖上一次运行的残留（这也是 `setUp` 存在的意义）。
2. **不依赖真实时间和随机**：需要时间就手动传 `dt`，需要随机就 `random.seed(8)` 固定种子。
3. **不依赖终端、网络、文件**：需要在屏幕上输出就先重定向；本项目连终端都不需要，因为逻辑和显示是分开的（8B.1.5 的分层在这里再一次得到回报）。

---

## 8E.5 集成测试：把模块拼起来

### 8E.5.1 单元测试 vs 集成测试

| | 单元测试 | 集成测试 |
|--|----------|----------|
| 测试对象 | 一个函数/一个类 | 多个模块协作 |
| 依赖 | 用假对象隔离 | 用真实的类 |
| 速度 | 极快 | 稍慢（本项目仍然很快） |
| 失败定位 | 容易，范围小 | 需要排查，范围大 |
| 例子 | `in_bounds`、`parse_keys` | “开一枪 → 敌人死 → 加分” |
| 数量比例 | 多 | 少 |

经验法则：**先写足够的单元测试，再补几条关键的集成测试**。全用集成测试会跑得慢、失败难定位；全用单元测试则没人验证“拼起来还能不能工作”。

### 8E.5.2 无头启动一局游戏

集成测试的关键是：`Game` 不需要终端就能运行。

```python
from game import Game
from config import STATE_PLAYING

game = Game()
game.state = STATE_PLAYING
game.spawn_queue = ["basic"]      # 控制下一波出什么敌人
game.freeze_time = 10.0           # 冻结敌人 AI，让场景可复现
```

- `Game()` 会生成地图、创建玩家，全部在内存里完成；
- 直接修改 `state`、`spawn_queue`、`freeze_time` 是“测试对内部状态作弊”。在测试里这是允许的：你的目标是**制造场景**，不是模拟玩家点了一遍菜单；
- `game.update(1 / 120)` 推进一步逻辑，和主循环里做的事情一模一样，只是没有渲染、没有睡眠。

### 8E.5.3 战斗场景：开一枪，敌人死，加分

新建 `test_game.py`，先写基础类和第一个场景：

```python
"""Integration tests: several modules working together, no terminal."""

import random
import re
import unittest

from battlefield import base_position
from bullet import Bullet
from config import (
    BASE_RUBBLE, CELL_WIDTH, FIELD_HEIGHT, FIELD_WIDTH, FLOOR, MIN_COLS,
    STATE_GAME_OVER, STATE_PLAYING, STATE_READY, STATE_STAGE_CLEAR,
)
from display import display_width, render_frame
from enemy import Enemy
from game import Game
from powerup import PowerUp

ANSI = re.compile(r"\033\[[0-9;?]*[A-Za-z]")


class GameTestCase(unittest.TestCase):
    """Base class that builds a deterministic, already-playing game."""

    def make_game(self, queue):
        game = Game()
        game.state = STATE_PLAYING
        game.spawn_queue = list(queue)
        game.freeze_time = 10.0        # keep enemies still and quiet
        return game


class TestCombat(GameTestCase):
    """Rules that need player, bullet, enemy and game together."""

    def test_player_bullet_kills_enemy_and_scores(self):
        game = self.make_game(["basic"])
        for col in range(3, 7):        # clear the firing line
            game.grid[5][col] = FLOOR
        game.enemies = [Enemy(5, 5, "basic")]
        game.player.row, game.player.col = 5, 3
        game.player.direction = "right"
        game.spawn_bullet(game.player)
        game.update(0.05)
        self.assertEqual(game.enemies, [])
        self.assertEqual(game.score, 100)
```

**这段测试覆盖了 5 个模块**：`Enemy`（敌人）、`Player`/`Tank`（玩家和基类）、`Bullet`（子弹）、`Game`（规则）、`battlefield`（用 `base_position`）。这就是集成测试的价值：它验证“拼起来能不能工作”。

**值得学的写法**

- `for col in range(3, 7): game.grid[5][col] = FLOOR`——**把测试环境清理成确定状态**。地图是随机生成的，不清空弹道的话，子弹可能先打中一块砖，测试就会时灵时不灵（这种“偶发失败”最消耗耐心）。
- `game.update(0.05)` 推进一步。`0.05` 比子弹的移动间隔（`0.04`）大，所以子弹一定前进一步——**测试里的时间是可以精确控制的**。
- 断言很直接：敌人列表空了、分数是 100。如果程序里把击杀分数从 100 改成 200，这条测试会立刻变红，提醒你“行为变了”。

### 8E.5.4 基地被毁：游戏结束

继续往 `TestCombat` 里加：

```python
    def test_enemy_bullet_destroys_the_base(self):
        game = self.make_game([])
        base_row, base_col = base_position()
        enemy = Enemy(1, 1, "basic")
        game.enemies = [enemy]
        game.player.row, game.player.col = 1, 3
        bullet = Bullet(base_row, base_col - 1, "right", enemy)
        game.bullets = [bullet]
        game._step_bullet(bullet)
        self.assertEqual(game.state, STATE_GAME_OVER)
        self.assertEqual(game.grid[base_row][base_col], BASE_RUBBLE)
```

这条测试就是 8E.1.2 里“故事一”的防线：**如果 `game.py` 忘了导入 `BASE`，子弹打到基地时会抛 `NameError`，这条测试立刻抓住它**，而你不用真的玩到那一刻。

另外注意 `game._step_bullet(bullet)` 用的是下划线开头的内部方法。测试偶尔可以“走后门”直接调内部方法，换取更精确的场景控制；这是权衡，不是错误。

### 8E.5.5 过关：清空敌人 → 下一关

```python
    def test_clearing_the_field_starts_the_next_stage(self):
        game = self.make_game([])
        game.enemies = []
        game.update(1 / 120)
        self.assertEqual(game.state, STATE_STAGE_CLEAR)
        game.update(3.0)
        self.assertEqual(game.stage, 2)
        self.assertEqual(game.state, STATE_READY)
```

它验证了状态机的流转：`PLAYING → STAGE_CLEAR → (等待 2.2 秒) → READY`。状态机是 8B.2.7 里最复杂的一块逻辑，值得用一条集成测试固定下来。

### 8E.5.6 道具掉落：4 连杀

```python
    def test_powerup_drops_after_four_kills(self):
        game = self.make_game(["basic"])
        for _ in range(4):
            game._kill_enemy(Enemy(8, 8, "basic"))
        self.assertTrue(game.powerups)
```

这条测试对应“故事二”：道具掉落的代码路径只有击杀 4 次才会走到。有了它，`PowerUp.position` 那类“写了不存在的属性”的错误会立刻暴露，而不是等你玩到第三关才崩。

### 8E.5.7 冒烟测试：随机输入跑几千帧

前面几条测试是“定点检查”。还有一类测试**不检查具体行为，只检查程序不崩、世界保持合法**：

```python
class TestSmoke(GameTestCase):
    """Fuzz the whole game and check the world stays legal."""

    def test_random_input_for_a_few_thousand_frames(self):
        random.seed(8)
        game = Game()
        actions = ["up", "down", "left", "right", "fire"]
        for _ in range(3000):
            if random.random() < 0.3:
                game.handle_input(random.choice(actions))
            game.update(1 / 60)
            for tank in list(game.tanks()):
                self.assertTrue(0 <= tank.row < FIELD_HEIGHT)
                self.assertTrue(0 <= tank.col < FIELD_WIDTH)
            for bullet in game.bullets:
                self.assertTrue(0 <= bullet.row < FIELD_HEIGHT)
                self.assertTrue(0 <= bullet.col < FIELD_WIDTH)
```

要点：

- `random.seed(8)` 固定随机种子。这样测试**可复现**：今天失败的那一串“乱按”，明天还能按出来；
- 3000 帧约等于 50 秒游戏时间，在电脑上跑不到一秒；
- 它检查的是“不变量”：所有坦克、所有子弹的坐标必须在地图内。这种测试也叫**模糊测试（fuzz test）**，是发现“边界情况崩溃”的利器。

### 8E.5.8 渲染也要测：每行宽度必须正确

渲染模块也能无头测试，因为 `render_frame` 只是“数据进、字符串出”：

```python
class TestRendering(GameTestCase):
    """Rendering is data in, strings out; no real terminal required."""

    def test_every_board_line_is_exactly_one_screen_wide(self):
        game = self.make_game([])
        game.update(1 / 120)
        lines = render_frame(game)
        for line in lines[:FIELD_HEIGHT]:
            text = ANSI.sub("", line)
            self.assertEqual(display_width(text), FIELD_WIDTH * CELL_WIDTH)
        for line in lines:
            self.assertLessEqual(display_width(ANSI.sub("", line)), MIN_COLS)

    def test_unknown_powerup_kind_is_a_programming_error(self):
        game = self.make_game([])
        game.powerups.append(PowerUp("unknown", 1, 1))
        with self.assertRaises(KeyError):
            render_frame(game)
```

**原理**：`render_frame` 返回的每一行里混着 ANSI 颜色码，先用正则把它们去掉，再用 `display_width()` 数实际占用的列数。中文占两列、ASCII 占一列——只要有一行宽度不对，终端画面就会错位，而这条测试能在不打开终端的情况下发现它。

第二个测试用 `assertRaises` 固定“未知道具类型必须大声报错”的行为。`render_frame` 会在 `POWERUP_TYPES[kind]` 处抛 `KeyError`，这正是我们想要的：**错误要早发现、要明显**，而不是悄悄画出一个空白格。

### 8E.5.9 集成测试的注意事项

- **控制环境**：随机地图、随机敌人、真实时间都是不稳定因素，测试里要把它们固定住（清空弹道、固定队列、固定 `dt`、设置种子）。
- **一条测试只测一个场景**：别把“射击+过关+道具”塞进一个方法，失败时你不知道是哪段坏了。
- **允许一点副作用**：`test_enemy_bullet_destroys_the_base` 会触发游戏结束、写一次 `highscore.txt`。这是正常现象，不用为它特别处理。
- **先单元、后集成**：单元测试覆盖细节，集成测试覆盖协作。本项目 22 条测试全部跑完只要 0.02 秒，完全可以每次改完都跑一遍。

运行整套测试：

```bash
py -m unittest -v
```

参考输出（本项目的 22 条测试）：

```
......................

----------------------------------------------------------------------
Ran 22 tests in 0.023s

OK
```

---

## 8E.6 让测试进入开发流程

### 8E.6.1 三个习惯

1. **改代码前先跑一遍测试**：全绿，说明起点正常；改完再跑，还绿，说明没改坏。
2. **每修一个 bug，补一条测试**：这样它不会再犯。上面两个真实故事就是这么变成防线的。
3. **新功能先想“怎么验证它”**：不一定要严格的 TDD，但在写实现之前想清楚“我怎么知道它是对的”，能省很多返工。

### 8E.6.2 unittest 和 pytest

`pytest` 是第三方测试框架，语法更简洁，社区里很流行：

```python
# pytest 写法：不需要继承 TestCase，直接用 assert
from battlefield import in_bounds

def test_outside():
    assert not in_bounds(-1, 0)
```

| | unittest | pytest |
|--|----------|--------|
| 来源 | 标准库，装好 Python 就有 | 需要 `pip install pytest` |
| 写法 | 继承 `TestCase`，用 `self.assertXxx` | 直接写函数和 `assert` |
| 功能 | 够用 | 插件多、参数化方便 |
| 适合 | 教学、零依赖项目（TankBattle 就是） | 大型项目 |

TankBattle 追求“零第三方依赖”，所以用标准库的 `unittest` 就够了。

### 8E.6.3 覆盖率和 CI（知道概念即可）

- **覆盖率**：测试执行到的代码占总代码的比例。工具能告诉你“哪些代码从来没被测试跑到”。覆盖率不是越高越好，100% 覆盖率也可能测不出业务错误。
- **CI（持续集成）**：每次把代码推送到服务器，自动安装环境并跑一遍测试。TankBattle 现在还没有，等你以后把项目放到远程仓库再说。

### 8E.6.4 测试不是越多越好

- 测试是**要维护的代码**：接口改了，测试也要改。
- 不要测试实现细节（比如“`_step_bullet` 里面先判断了砖还是钢”），要测试**对外行为**（“子弹应该摧毁砖墙”）。
- 如果一条测试经常误报，要么修改它，要么删掉它——**没有人看的红色告警等于没有告警**。

---

## 8E.7 动手练习（选做）

1. **给显示宽度补测试**：`display_width("钢")`、`display_width("abc")`、`display_width("我^")`、空字符串，各应该返回多少？
2. **给伤害写边界测试**：`hp=1` 时挨 1 点和挨 100 点，结果应该一样吗？`hp` 会不会变成负数？
3. **给敌人 AI 写测试**：`Enemy._choose_path(game)` 的返回列表里，四个方向是不是恰好各出现一次？把结果打成 `set` 再断言长度。
4. **给解析器加戏**：一次输入两个方向键 `"\x1b[A\x1b[C"`，应该解析出 `["up", "right"]`。
5. **把冒烟测试拉长**：3000 帧改成 30000 帧，看要跑多久；再把 `random.seed` 换几个值，看能不能跑出崩溃。
6. **体验“回归”**：故意把 `config.FIELD_WIDTH` 改成 `27`，跑一遍测试，看看哪些变红；然后改回来。你会对“测试是怎么报警的”有直观感受。
7. **（进阶）给道具写生命周期测试**：道具 12 秒后应该消失。写一个测试连续推进 12.5 秒，断言 `game.powerups` 为空。

---

## 8E.8 本节你学到了什么（选学内容）

即使这一节是选学的，只要读完了，你就已经接触过这些概念：

1. **测试的价值**：让 bug 自己撞上来，而不是等玩家撞上；
2. **`assert` 与 `unittest`**：从“不满足就抛异常”到标准的测试框架——`TestCase`、`test_` 命名、`self.assertXxx`、`setUp`、`subTest`、`assertRaises`；
3. **单元测试**：纯函数、随机函数测不变量、用假对象隔离依赖；
4. **集成测试**：无头运行游戏、控制测试环境、定点场景测试、冒烟/模糊测试；
5. **渲染测试**：去掉 ANSI 色码后检查每一行的显示宽度；
6. **测试纪律**：独立、确定、不依赖真实时间和终端；先单元后集成；测试是代码，也要维护。

**没学会也不影响**：8A–8D 的内容已经足够让你读懂、修改和扩展这个项目。等你哪天因为改坏一个功能而苦恼时，再翻回这一节，你会发现它比现在好懂很多。
