# 第8B节：项目剖析 —— 拆开一个真正的坦克大战

8A 讲了模块与导入的规则。这一节把镜头拉近，逐层拆开 TankBattle：先看目录、依赖图和完整的数据流，再逐个模块分析它的职责与设计。

---

## 8B.1 项目全貌

在逐行读代码之前，先建立“地图感”：这个项目有多少个抽屉，抽屉之间怎么连接。

### 8B.1.1 目录结构与运行

```
TankBattle/
├── main.py             # 入口：主循环、终端初始化与恢复
├── config.py           # 全部常量：尺寸、速度、颜色、敌人/道具属性
├── battlefield.py      # 地形生成与地形查询
├── tank.py             # 坦克基类
├── player.py           # 玩家坦克
├── enemy.py            # 敌方坦克 + AI
├── bullet.py           # 子弹
├── powerup.py          # 道具
├── game.py             # 游戏状态机、碰撞、刷怪、计分
├── display.py          # ANSI 渲染、HUD、差分刷新
├── input_handler.py    # 跨平台非阻塞键盘输入
├── README.md           # 玩法、按键、图例
└── highscore.txt       # 运行时生成的最高分存档（可不存在）
```

运行方式：

```bash
cd TankBattle
py main.py
```

要求终端至少 78 列 × 19 行。所有文件平铺在同一个目录里，没有包——因为模块数量不多，依赖关系也很清楚。

### 8B.1.2 每个模块的职责与规模

| 模块 | 约行数 | 职责 | 导入了谁 |
|------|-------:|------|---------|
| `config.py` | 148 | 全部常量：尺寸、速度、颜色、敌人数值、道具表 | 标准库 `pathlib` |
| `battlefield.py` | 189 | 生成地图；判断格子能不能走；提供基地/出生点 | `config`、`random` |
| `tank.py` | 78 | 坦克基类：位置、朝向、血量、移动、开火许可 | `config` |
| `player.py` | 94 | 玩家坦克：火力等级、护盾、按键记忆 | `tank`、`config` |
| `enemy.py` | 114 | 敌人坦克：按类型初始化、寻路、瞄准开火 | `tank`、`config`、`random` |
| `bullet.py` | 24 | 子弹：位置、方向、飞行计时、归属 | `config` |
| `powerup.py` | 18 | 道具：类型、位置、存活时间 | `config` |
| `game.py` | 450 | 游戏状态机、碰撞、刷怪、道具、计分、存档 | 上面全部业务模块 + `config` |
| `display.py` | 213 | 把游戏数据渲染成字符串，差分刷新终端 | `config`、`unicodedata`、`shutil` 等 |
| `input_handler.py` | 126 | 非阻塞读取键盘，解析方向键/功能键 | 标准库 `os`、`select`、`sys`、`time` |
| `main.py` | 103 | 入口：组装对象、主循环、退出时恢复终端 | `config`、`display`、`game`、`input_handler` |

把这几个数字读一遍，你会发现两个事实：

1. **450 行的 `game.py` 是唯一的“大文件”**，因为游戏规则天然复杂；其余模块都在 20~220 行之间，单个文件可以一口气读完。
2. **`config.py` 被几乎所有人依赖，它自己谁都不依赖**。这是有意的——常量放在最底层，改一处全局生效。

### 8B.1.3 依赖图与分层

```
                                main.py
                    ┌──────────────┼──────────────┬─────────────┐
                    ▼              ▼              ▼             ▼
               input_handler   display         game        config
                (无本地依赖)   (只用 config)      │
                    ┌──────────┬──────────┬───────┼────────┬──────────┐
                    ▼          ▼          ▼       ▼        ▼          ▼
              battlefield   player     enemy   bullet  powerup     (config)
                    │          │          │       │        │
                    ▼          ▼          ▼       ▼        ▼
                 config      tank ────── tank   config   config
                               │
                               ▼
                             config
```

简化成“分层”的说法：

```
第 0 层（常量）     config.py
第 1 层（基础数据） battlefield.py  bullet.py  powerup.py  tank.py
第 2 层（实体）     player.py  enemy.py
第 3 层（世界/规则） game.py
第 4 层（外围）     display.py  input_handler.py
第 5 层（入口）     main.py
```

**关键原则**（和改写前的版本 一致，但用新项目重新验证）：

- `config.py` **不依赖任何业务模块**，所以任何模块都能安全地导入它，不会产生循环。
- `battlefield.py` 只生产“地形数据”，它不知道坦克是什么，也不知道谁会来画它。
- `tank.py` 是基类，`player.py` 和 `enemy.py` 继承它；实体不导入 `game`，需要 world 信息时由调用者把 `game` 传进来（8A.2.9 的依赖倒置）。
- `game.py` 是唯一同时知道所有实体的模块：它是“世界”。
- `display.py` **不导入 `game`**：`render_frame(game)` 里的 `game` 只是一个参数，鸭子类型。
- `main.py` 是唯一的“知道所有人”的模块，但它的代码最薄——只负责创建对象和跑循环。

### 8B.1.4 一帧里发生了什么

实时游戏和前面几节的“输入 → 计算 → 输出”最大的区别：它**不停地循环**，即使你没按键，敌人、子弹、计时器也在动。

下面是 TankBattle 运行中的一轮循环（一帧）：

```
1. 轮询输入（main.py）
   调用 keys.read_keys() 把这一小段时间内按下的键全部取出来
   → game.handle_input("up") / game.handle_input("fire") ...

2. 推进逻辑（main.py → game.py）
   把真实流逝的时间累加到 accumulator
   只要累加够 LOGIC_STEP（1/120 秒）就调用一次 game.update(LOGIC_STEP)
   （一帧里可能调用 0 次、1 次或多次）

3. game.update 按状态机分派
   READY        → 倒计时结束进入 PLAYING
   PLAYING      → _update_playing()
   STAGE_CLEAR  → 倒计时结束，进入下一关
   GAME_OVER    → 只更新爆炸特效，等待玩家按 R/Q
   PAUSED       → 直接返回

4. _update_playing 的固定顺序
   a. 冻结计时减一减
   b. player.update()   —— 处理按键意图：移动、开火、护盾倒计时
   c. _update_spawning() —— 该不该刷出新敌人
   d. 每个 enemy.update() —— 移动、重新规划路线、开火
   e. _update_bullets()   —— 子弹逐格飞行并结算碰撞
   f. _update_powerups()  —— 道具倒计时、拾取
   g. _remove_dead()      —— 统一清掉死亡实体
   h. _update_extra_life() / _check_stage_clear()

5. 渲染（main.py → display.py）
   render_frame(game) 把第 4 步之后的数据组装成：
     16 行战场 + 3 行 HUD（分数/最高分/关卡/生命/火力/敌军/提示）
   Renderer.draw() 只把“和上一帧不同”的行写到终端

6. 睡眠 2 毫秒，回到第 1 步
```

主循环的三个时间尺度：

| 频率 | 用途 | 常量 |
|------|------|------|
| 约 500 Hz | 轮询键盘，保证按键不丢 | `INPUT_POLL_SLEEP = 0.002` |
| 120 Hz | 游戏逻辑更新（固定步长） | `LOGIC_STEP = 1/120` |
| 60 Hz | 渲染画面 | `FRAME_TIME = 1/60` |

为什么逻辑和渲染要分开？这正是 8C.19 要补的知识。

### 8B.1.5 为什么这样分层

用三个“如果混在一起会怎样”来验证：

- **如果地图生成和渲染写在一起**：你想给砖墙换颜色，却可能改坏地图随机算法。现在换颜色只动 `config.TERRAIN_COLORS`。
- **如果敌人 AI 写在 `game.py` 里**：`game.py` 会从 450 行涨到 600 行以上，而且以后想换更聪明的 AI 时，要在规则代码里做手术。现在只动 `enemy.py`。
- **如果按键读取写在 `player.py` 里**：玩家坦克会同时依赖 `termios`、`msvcrt`、`select`，在 Windows 上想跑就得改玩家类。现在这些平台差异全部隔离在 `input_handler.py`，游戏逻辑一行都不知道。

这就是**关注点分离**：每个模块只关心自己那一件事。

### 8B.1.6 什么时候该继续拆

`game.py` 450 行，是下一个可能要拆的对象。经验法则：

- 单文件超过 **500 行**，读起来开始费劲；
- 文件里出现**第二个明显的职责**（比如“存档”和“碰撞”混在一起）；
- 某个函数**只想单独测试**，却因为文件里的其他东西导入不进来；
- 两个人经常同时改**同一个文件**。

对 `game.py` 来说，候选拆分点是：`collision.py`（碰撞判定）、`spawning.py`（刷怪与关卡难度）、`scoring.py`（计分与存档）。但目前 450 行还在“一口气能读完”的范围内，**不拆也是正确的选择**——过度拆分会把调用关系切得七零八落。架构是权衡，不是越多文件越好。

---

## 8B.2 逐模块分析：每个抽屉里装了什么

这一节按“从底层到上层”的顺序读代码。每段只贴**关键片段**，重点讲设计意图和容易误解的地方。建议你打开对应文件对照阅读。

### 8B.2.1 config.py：唯一的事实来源

**职责**：放所有“数字”和“字符表”，不放任何逻辑。

**片段一：时间参数**

```python
# ------------------------------------------------------------------ timing
FRAME_TIME = 1 / 60          # target seconds per rendered frame
LOGIC_STEP = 1 / 120         # fixed simulation step
INPUT_POLL_SLEEP = 0.002     # seconds between input polls in the main loop
TAP_HOLD_TIMEOUT = 0.15      # keep moving briefly after a tap
HOLD_TIMEOUT_MIN = 0.09      # timeout once the key repeat cadence is known
HOLD_GRACE_MAX = 0.60        # cap for the adaptive hold timeout
REPEAT_MAX_GAP = 0.45        # larger gaps are treated as a fresh key press
FIRE_HOLD_TIMEOUT = 0.15     # keep trying to fire after the last key event
```

**片段二：地形字符表**

```python
TERRAIN_GLYPHS = {
    FLOOR: " ",
    BRICK: "砖",
    STEEL: "钢",
    WATER: "水",
    FOREST: "林",
    BASE: "鹰",
    BASE_RUBBLE: "毁",
}

TERRAIN_COLORS = {
    FLOOR: None,
    BRICK: "yellow",
    STEEL: "bright_white",
    WATER: "bright_blue",
    FOREST: "green",
    BASE: "bright_yellow",
    BASE_RUBBLE: "bright_red",
}
```

**片段三：敌人类型表**

```python
ENEMY_TYPES = {
    "basic": {"char": "敌", "color": "red",
              "hp": 1, "speed": 1.0, "bullet_speed": 1.0, "score": 100},
    "fast":  {"char": "快", "color": "bright_cyan",
              "hp": 1, "speed": 1.8, "bullet_speed": 1.0, "score": 200},
    "power": {"char": "猛", "color": "magenta",
              "hp": 1, "speed": 1.0, "bullet_speed": 2.0, "score": 300},
    "armor": {"char": "甲", "color": "bright_yellow",
              "hp": 4, "speed": 0.7, "bullet_speed": 1.0, "score": 400},
}
# Armour tanks change color as they lose plating.
ARMOR_HP_COLORS = {4: "bright_yellow", 3: "yellow", 2: "red", 1: "gray"}
```

**分析**

1. **为什么地形用字符串常量而不是直接写“砖”？**

   ```python
   FLOOR = "floor"
   BRICK = "brick"
   STEEL = "steel"
   ```

   `battlefield.py` 里写的是 `grid[r][c] = BRICK`，而不是 `grid[r][c] = "砖"`。这一层间接看起来多余，但它把“**逻辑上的地形种类**”和“**显示成什么字符**”分开了：想把砖墙换成 `🧱`，只改 `TERRAIN_GLYPHS[BRICK]`；想加一种新地形，逻辑代码里用的是常量名，不容易写错字符串。

2. **字典当“小数据库”——数据驱动设计**

   `ENEMY_TYPES` 把“敌人类型”的四个属性（字符、颜色、血量、速度、子弹速度、分数）集中在一个表里。`enemy.py` 拿到 `type_name` 后只是查表：

   ```python
   info = ENEMY_TYPES[type_name]
   ```

   后面 8D.2 你会发现，**加一种新敌人只需要在 `config.py` 的表里加一行**，`enemy.py` 一行都不用改。如果不用表，而是写一串 `if type_name == "basic": ... elif ...`，加类型就要改逻辑代码。

3. **元组还是字典？**

   ```python
   PLAYER_BULLETS_BY_POWER = (1, 1, 2, 2)
   PLAYER_BULLET_SPEED_BY_POWER = (1.0, 1.6, 1.6, 2.0)
   ```

   “火力等级 → 子弹数”是一个从 0 开始的连续整数映射，用**元组**比字典更紧凑，访问方式是 `PLAYER_BULLETS_BY_POWER[self.power]`。

4. **`Path(__file__).resolve().parent`**

   ```python
   HIGH_SCORE_FILE = Path(__file__).resolve().parent / "highscore.txt"
   ```

   `__file__` 是当前文件路径；`resolve()` 转成绝对路径；`.parent` 取所在目录。这样无论你从哪个目录运行 `python /path/to/TankBattle/main.py`，最高分文件都紧挨着代码，而不是跟着“当前工作目录”乱跑。

5. **调游戏手感只需要改这里**。把 `PLAYER_MOVE_INTERVAL` 从 `0.11` 改成 `0.3`，玩家立刻变慢；把 `FRAME_TIME` 改成 `1/30`，画面降到 30 FPS。不需要动任何逻辑。

> **提醒**：`config.py` 只放**常量**。如果你在里面写了一个会改变状态的函数，所有导入它的模块都会共享那份状态，问题很难查。常量表是最安全的“公共依赖”。

---

### 8B.2.2 battlefield.py：只生产地图，不认识坦克

**职责**：生成每关的地形；回答“这个格子能不能走”“基地在哪里”。

**片段一：生成函数的主干**

```python
def create_battlefield(stage: int) -> list[list[str]]:
    """Build the terrain grid for a stage."""
    grid = [[FLOOR] * FIELD_WIDTH for _ in range(FIELD_HEIGHT)]
    _add_border(grid)
    _add_brick_blocks(grid, stage)
    _add_steel_blocks(grid, stage)
    _add_water(grid, stage)
    _add_forest(grid)
    _build_base(grid)
    _clear_spawn_zones(grid)
    return grid
```

**片段二：查询函数**

```python
def in_bounds(row: int, col: int) -> bool:
    """Return True if (row, col) is inside the battlefield."""
    return 0 <= row < FIELD_HEIGHT and 0 <= col < FIELD_WIDTH


def is_tank_passable(tile: str) -> bool:
    """Return True if tanks may drive onto this tile."""
    return tile in (FLOOR, FOREST, BASE_RUBBLE)
```

**片段三：基地与护盾**

```python
base_row, base_col = base_position()
# 先把“家门口”清空
for r in range(base_row - 2, base_row + 1):
    for c in range(base_col - 5, base_col + 3):
        if in_bounds(r, c):
            grid[r][c] = FLOOR
# 再放老鹰
grid[base_row][base_col] = BASE
# 最后砌 5 块砖当护盾
shield = ((base_row - 1, base_col - 1), (base_row - 1, base_col),
          (base_row - 1, base_col + 1), (base_row, base_col - 1),
          (base_row, base_col + 1))
```

**分析**

1. **全是函数，没有类**。地图生成是“输入 stage → 输出 grid”，不需要在两次调用之间记住任何东西，所以不需要类。**不是所有东西都该装进类里**——这是新手常见的过度设计。

2. **二维列表推导式**（回顾 4.2）：

   ```python
   grid = [[FLOOR] * FIELD_WIDTH for _ in range(FIELD_HEIGHT)]
   ```

   先造一行，再复制出 `FIELD_HEIGHT` 行。注意必须是**推导式**，不能写 `[[FLOOR] * W] * H`——后者会让所有行指向同一个列表，改一行等于改所有行（回顾 6.7 的深浅拷贝）。

3. **生成顺序是有讲究的**。先铺地板 → 加边框 → 随机砖块/钢块/水/树林 → **最后**才清理基地和出生点区域。如果反过来先建基地再加随机砖块，砖块可能把基地埋了。这种“先生成、再保底”的思路在程序生成里很常见。

4. **随机但不至于没法玩**。`_clear_spawn_zones()` 保证 3 个敌人出生点、玩家出生点周围是空的；`_build_base()` 保证基地周围有可站立区域和砖墙护盾。随机地图最常见的 bug 就是“刷出一个被墙封死的出生点”，保底清理就是防它的。

5. **`is_tank_passable(tile)` 只接收一个格子**，而不是整张地图——因为“能不能过”只取决于这一格的地形。把函数参数设计得尽量小，测试起来最方便。

> **常见疑问**：为什么 `destroy_brick()` 不在这个文件里？
>
> 实时的子弹碰撞需要“销毁砖块”和“产生爆炸特效”“判断是否击中基地”一起做，所以销毁动作写在了 `game._step_bullet()` 里：`self.grid[r][c] = FLOOR`。这不算破坏分层——`game.py` 本来就有权修改地形。如果哪天“修改地形”的逻辑变得复杂（比如砖块有 4 个耐久度），可以再抽一个 `terrain.py`。

---

### 8B.2.3 tank.py：基类与依赖倒置

**职责**：玩家和敌人共用的属性/方法：位置、朝向、血量、移动、能不能开火。

**片段一：类属性默认值**

```python
class Tank:
    # Defaults; subclasses may override them with properties.
    bullet_speed = 1.0
    max_bullets = 1
    shoot_cooldown = 0.3
    breaks_steel = False
```

**片段二：移动**

```python
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

**分析**

1. **为什么是“try_move”而不是“move”？** 名字里带 `try_` 的函数明确告诉调用者“可能失败”。它返回 `bool`，调用者可以决定失败后怎么办（玩家：原地不动；敌人：换一个方向）。

2. **它不 import `game`，而是接收 `game`**。这是 8A.2.9 讲的依赖倒置。结果：
   - 依赖方向单一，不会循环导入；
   - `Tank` 不关心 `Game` 是什么类，只要求它有一个 `can_tank_enter(tank, row, col)` 方法；
   - 可以在测试里传一个只有这个方法的小假对象。

3. **转向和移动是分开的**：即使撞墙失败，`self.direction = direction` 也已经执行，坦克会“原地转头”。这对射击游戏很重要——你贴着墙按方向键，坦克应该把炮口转过去，而不是毫无反应。

4. **类属性当默认值**（`bullet_speed = 1.0` 等）。这三个值对“基础坦克”是常量，而 `Player`/`Enemy` 会用自己的方式覆盖（子类用 `@property` 或在 `__init__` 里赋值）。为什么要用类属性而不是在 `__init__` 里逐条赋值？8C.3 会讲到一个真实的坑。

5. **`take_damage` 里的防御式写法**：

   ```python
   self.hp = max(0, self.hp - amount)
   if self.hp == 0:
       self.alive = False
   ```

   `max(0, ...)` 保证血量不会变成负数；`alive` 是显式标志，而不是每次去判断 `hp > 0`。两种写法都行，但显式标志让“死亡”只发生一次（比如只播放一次爆炸）。

---

### 8B.2.4 player.py：子类、属性、输入手感

**职责**：玩家坦克的专属逻辑：火力等级、护盾、把按键变成“移动/开火意图”。

**片段一：继承与初始化**

```python
class Player(Tank):
    """The player tank: moves in real time and fires on demand."""

    def __init__(self, row: int, col: int) -> None:
        super().__init__(
            row, col, PLAYER_CHAR, PLAYER_COLOR,
            hp=1, move_interval=PLAYER_MOVE_INTERVAL, faction="player",
        )
        self.power = 0                 # 0..3, raised by star power-ups
        self.shield_time = SPAWN_SHIELD_TIME
        self.held_direction = None
        self.held_timer = 0.0
        self.fire_held = False
        self.fire_timer = 0.0
        self.shoot_cooldown = PLAYER_SHOOT_COOLDOWN
```

**片段二：三种火力参数是算出来的**

```python
    @property
    def max_bullets(self) -> int:
        return PLAYER_BULLETS_BY_POWER[self.power]

    @property
    def bullet_speed(self) -> float:
        return PLAYER_BULLET_SPEED_BY_POWER[self.power]

    @property
    def breaks_steel(self) -> bool:
        return self.power >= 3
```

**片段三：按键只记录“意图”**

```python
    def handle_key(self, key: str) -> None:
        """Remember a direction or the fire key (called on key events)."""
        if key in DELTAS:
            gap = self._clock - self._last_event
            if key == self.held_direction and gap < REPEAT_MAX_GAP:
                # 终端的按键自动重复：识别出重复间隔后，松手很快停下
                ...
                self.held_timer = max(
                    HOLD_TIMEOUT_MIN,
                    min(HOLD_GRACE_MAX, self._repeat_gap * 2.5),
                )
            else:
                # 新的方向按键：立刻走一步，手感更跟手
                if key != self.held_direction:
                    self.move_accum = self.move_interval
                self.held_timer = TAP_HOLD_TIMEOUT
            self._last_event = self._clock
            self.held_direction = key
        elif key == "fire":
            self.fire_held = True
            self.fire_timer = FIRE_HOLD_TIMEOUT
```

**分析**

1. **`super().__init__(...)`** 是“先让父类把公共部分初始化好”。玩家特有的 `power`、`shield_time` 等才在子类里补。这是继承最常见的用法。

2. **`@property` 把“算出来的值”伪装成普通属性**。使用者写 `tank.max_bullets`，不需要知道它是由 `power` 查表算出来的。火力升级只需要改 `self.power`，子弹数、子弹速度、能否破钢会自动跟着变——**单一数据源**。

3. **`handle_key` 不移动坦克，只记住意图**。真正的移动发生在 `update(dt, game)` 里，按时间推进。这样做的好处：
   - 按键事件可能在两次逻辑更新之间到达（主循环 500 Hz 轮询），先把意图存起来绝不会丢；
   - 逻辑更新是固定步长的，移动速度稳定，不受“一帧按了几次键”影响；
   - 这也是“输入 → 逻辑 → 渲染”分离的具体体现。

4. **长按的手感问题**（终端特有的坑）：终端的“按键自动重复”是操作系统发的，按住方向键时先停顿约 0.2~0.6 秒，然后才开始连续发字符。如果程序每次收到字符只走一格，手感会一顿一顿。`Player` 的做法是：
   - 记录两次方向键事件的间隔 `gap`；
   - 如果间隔小于 `REPEAT_MAX_GAP`（0.45 秒）且方向相同，说明是“自动重复”，把最小的重复间隔记到 `_repeat_gap`；
   - 松手后允许再走 `2.5 × 间隔` 的时间（下限 0.09 秒），既不断档也不会滑太远；
   - 新按下一个方向时直接把 `move_accum` 拉满，**立刻走一格**，解决“按下去没反应”的问题。

5. **`update` 里用 `while` 消耗 `move_accum`**：

   ```python
   self.move_accum += dt
   while self.move_accum >= self.move_interval:
       self.move_accum -= self.move_interval
       self.try_move(self.direction, game)
   ```

   如果某次 `dt` 比较大（比如窗口卡了一下），`while` 会补上多步；用 `if` 的话，坦克速度就会随帧率变化而变化。这就是“按时间积分”的雏形。

---

### 8B.2.5 enemy.py：AI 也只是一个子类

**职责**：敌人坦克的初始化、寻路、瞄准和开火节奏。

**片段一：查表初始化**

```python
class Enemy(Tank):
    def __init__(self, row: int, col: int, type_name: str = "basic") -> None:
        info = ENEMY_TYPES[type_name]
        super().__init__(
            row, col, info["char"], info["color"], hp=info["hp"],
            move_interval=ENEMY_MOVE_INTERVAL / info["speed"],
            faction="enemy",
        )
        self.type_name = type_name
        self.bullet_speed = info["bullet_speed"]
        self.score = info["score"]
        self.shoot_timer = random.uniform(0.5, 1.4)
        self.plan = None                 # 方向尝试列表
        self.rethink_timer = random.uniform(0.2, 0.8)
```

**片段二：贪心寻路**

```python
    def _step(self, game) -> None:
        if not self.plan:
            self.plan = self._choose_path(game)
        for order, direction in enumerate(self.plan):
            if self.try_move(direction, game):
                if order:
                    self.plan.insert(0, self.plan.pop(order))
                return
        # 四个方向都走不通：掉头
        self.direction = OPPOSITE[self.direction]
        self.plan = [self.direction]

    def _choose_path(self, game) -> list[str]:
        """Return directions ordered from most to least useful."""
        if random.random() < 0.5:
            target_row, target_col = game.base_position
        else:
            target_row, target_col = game.player.position
        dr = target_row - self.row
        dc = target_col - self.col
        horizontal = "right" if dc > 0 else "left"
        vertical = "down" if dr > 0 else "up"
        ...
        rest = [d for d in DELTAS if d not in preferred]
        random.shuffle(rest)
        return preferred + rest
```

**分析**

1. **数据驱动再次出现**：`Enemy` 自己不知道“快速敌人速度是 1.8”，它只查 `ENEMY_TYPES`。想加“重型敌人”，改表即可（8D.2 练习二）。

2. **AI 用“计划列表”而不是单步决策**：`_choose_path` 返回一个按优先级排序的方向列表，比如 `["right", "down", "left", "up"]`。每步按顺序尝试，走得通就把这个方向提到最前。这样敌人不会在两个方向之间来回抖，也天然带有“撞墙换路”的能力。

3. **目标选择的随机性**：50% 概率追基地，50% 概率追玩家。纯追玩家会让游戏变成“躲猫猫”，纯追基地又太机械。随机混合让局面有变化。这是最简单的“权重决策”。

4. **和玩家的对称性**：两个子类都继承 `Tank`，都实现自己的 `update(dt, game)`。对 `game.py` 来说，它先调用 `player.update(...)`，再循环 `enemy.update(...)`——**多态**：不同的对象，同一种调用方式。

5. **AI 不写在 game.py 的理由**：AI 会不断迭代（加掩体规避、加协同进攻），把它隔离在 `enemy.py` 里，改坏也只影响敌人。`game.py` 只需要在每帧喊一声 `enemy.update(dt, self)`。

---

### 8B.2.6 bullet.py 与 powerup.py：小模块的价值

**职责**：极简的数据实体。

**powerup.py 全文**（它只有 18 行，是本项目最小的模块）：

```python
"""Collectible power-ups dropped by destroyed enemies."""

from config import POWERUP_LIFETIME


class PowerUp:
    """An item the player can collect by driving over it."""

    def __init__(self, kind: str, row: int, col: int) -> None:
        self.kind = kind
        self.row = row
        self.col = col
        self.ttl = POWERUP_LIFETIME

    @property
    def position(self) -> tuple[int, int]:
        """Return (row, col)."""
        return self.row, self.col
```

**bullet.py 的关键字段**：

```python
class Bullet:
    def __init__(self, row: int, col: int, direction: str, owner,
                 speed: float = 1.0) -> None:
        self.row = row
        self.col = col
        self.direction = direction
        self.owner = owner
        self.faction = owner.faction          # "player" / "enemy"
        self.breaks_steel = owner.breaks_steel
        self.interval = BULLET_MOVE_INTERVAL / speed
        self.accum = 0.0                      # 飞行计时器
        self.alive = True
```

**分析**

1. **小文件不等于没价值**。`powerup.py` 只有 18 行，但它的存在让 `game.py` 里可以写 `from powerup import PowerUp`，让“道具是什么”有唯一的定义处。如果把它塞进 `game.py`，下次你想复用“道具”概念时又要从大文件里抠。

2. **`owner` 反向引用**：子弹记住“谁发射的我”，于是 `bullet_speed`、`breaks_steel`、`faction` 都能从发射者身上取。好处是逻辑单一；代价是子弹持有坦克的引用（如果坦克被删了，子弹还在飞，Python 也不会释放坦克对象——本项目里坦克死了子弹也会被清掉，所以没问题）。

3. **`position` 属性统一了“实体”的接口**。坦克、子弹、道具都有 `.position`，于是 `game.py` 里可以写 `tank.position == bullet.position`，不用关心对方是什么类。

4. **为什么“飞行计时器”放在子弹身上**：每颗子弹的速度可以不同（火力等级、敌人类型），各自的 `interval` 和 `accum` 互不干扰。这就是“把状态放在最需要它的对象里”。

---

### 8B.2.7 game.py：大脑与物理引擎

**职责**：维护整个游戏世界：地图、实体列表、状态机、碰撞规则、刷怪、计分、存档。

**片段一：状态机的分派（节选）**

```python
    def update(self, dt: float) -> None:
        """Advance the whole game by *dt* seconds."""
        self.time += dt
        ...
        if self.state == STATE_PAUSED:
            return

        self._update_effects(dt)

        if self.state == STATE_READY:
            self.timer -= dt
            if self.timer <= 0:
                self.state = STATE_PLAYING
                self.set_message("开始！", "bright_white", 1.0)
        elif self.state == STATE_STAGE_CLEAR:
            self.timer -= dt
            if self.timer <= 0:
                self.stage += 1
                self._start_stage()
        elif self.state == STATE_PLAYING:
            self._update_playing(dt)
```

**片段二：每帧的固定顺序**

```python
    def _update_playing(self, dt: float) -> None:
        if self.freeze_time > 0:
            self.freeze_time = max(0.0, self.freeze_time - dt)

        self.player.update(dt, self)
        if self.state != STATE_PLAYING:
            return

        self._update_spawning(dt)
        for enemy in self.enemies:
            if enemy.alive:
                enemy.update(dt, self)
        if self.state != STATE_PLAYING:
            return

        self._update_bullets(dt)
        if self.state != STATE_PLAYING:
            return

        self._update_powerups(dt)
        self._remove_dead()
        self._update_extra_life()
        self._check_stage_clear()
```

**片段三：子弹逐格飞行**

```python
    def _step_bullet(self, bullet: Bullet) -> None:
        dr, dc = DELTAS[bullet.direction]
        bullet.row += dr
        bullet.col += dc
        if not in_bounds(bullet.row, bullet.col):
            bullet.alive = False
            return

        tile = self.grid[bullet.row][bullet.col]
        if tile == BRICK:
            self.grid[bullet.row][bullet.col] = FLOOR
            self._add_effect(bullet.row, bullet.col, "碎", "yellow", 0.12)
            bullet.alive = False
            return
        if tile == STEEL:
            if bullet.breaks_steel:
                self.grid[bullet.row][bullet.col] = FLOOR
            bullet.alive = False
            return
        if tile == BASE:
            bullet.alive = False
            self._destroy_base(bullet.faction == "player")
            return
        # 水、树林、地板、基地废墟都不挡子弹；接下来再检查坦克……
```

**片段四：子弹更新循环**

```python
    def _update_bullets(self, dt: float) -> None:
        for bullet in self.bullets:
            if not bullet.alive:
                continue
            bullet.accum += dt
            while bullet.alive and bullet.accum >= bullet.interval:
                bullet.accum -= bullet.interval
                self._step_bullet(bullet)
        self._resolve_bullet_duels()
```

**片段五：统一清尸**

```python
    def _remove_dead(self) -> None:
        self.enemies = [e for e in self.enemies if e.alive]
        self.bullets = [b for b in self.bullets if b.alive]
```

**片段六：难度曲线（节选）**

```python
    def _pick_enemy_type(self) -> str:
        weights = {
            "basic": max(1, 8 - self.stage),
            "fast": 2 + self.stage,
            "power": 1 + self.stage // 2,
            "armor": max(0, self.stage - 1),
        }
        names = [name for name, weight in weights.items() if weight > 0]
        chances = [weights[name] for name in names]
        return random.choices(names, weights=chances, k=1)[0]
```

**片段七：最高分持久化**

```python
def load_high_score() -> int:
    """Read the saved high score, ignoring a missing or broken file."""
    try:
        return int(HIGH_SCORE_FILE.read_text(encoding="utf-8").strip())
    except (OSError, ValueError):
        return 0


def save_high_score(score: int) -> None:
    """Best-effort persistence of the high score."""
    try:
        HIGH_SCORE_FILE.write_text(f"{score}\n", encoding="utf-8")
    except OSError:
        pass
```

**分析**

1. **Game 是“世界”，实体只保存自己**。子弹不知道“撞到东西该怎样”，它只有位置和方向；所有判定都在 `game._step_bullet` 里做。好处：规则集中，容易平衡和调试；实体类保持简单。

2. **状态机用字符串常量**（`STATE_READY` 等定义在 `config.py`）。状态决定了这一帧哪些代码会运行，也决定了 `display.py` 在画面上叠什么字（准备/暂停/结束）。

3. **更新顺序是设计过的**：
   - 玩家先动 → 玩家在“子弹结算”之前还能开火；
   - 然后刷怪、敌人动 → 敌人也在本帧开火；
   - 最后统一更新子弹 → 同一帧里所有人的子弹一起飞；
   - 这样安排能让交互公平，也避免“子弹先飞、敌人后动”造成的视觉错位。

4. **每一步之后都检查 `state != STATE_PLAYING` 就返回**。因为玩家开火可能在 `player.update()` 里就击毁了基地、结束游戏，后面的逻辑就不应该继续跑。

5. **子弹逐格推进而不是一次跳一大段**。假设子弹速度是 2 格/帧，如果直接从 `col=2` 跳到 `col=4`，中间那格有没有坦克/砖墙就漏判了（“穿墙 bug”）。`_update_bullets` 用 `while` 一次推进一格、每格结算一次，所以再快也不会穿墙。

6. **子弹在“出膛”时立刻走一步**（`spawn_bullet` 里调用 `_step_bullet`）。这叫“枪口结算”：如果炮口正对着砖墙，子弹应该立刻把砖打掉，而不是卡在坦克格子里等下一帧。

7. **死亡实体用列表推导统一清理**，而不是在每个击毁点手动 `remove`。手动 remove 很容易漏、也容易在遍历时改列表（8C.13 会讲这个坑）。

8. **难度曲线是公式，不是配置**：敌人总数量、刷新间隔、类型权重都写成 `stage` 的函数。调整公式就调整了整条曲线，不需要为每一关写死一张表。

9. **存档失败也不能崩游戏**：读文件可能遇到“文件不存在”（第一次玩）或“内容被人手改坏”，写文件可能遇到“目录只读”。`try/except` 分别把这两类意外吞掉，返回默认值或静默失败——对游戏来说，最高分丢了远不如崩溃严重。

---

### 8B.2.8 display.py：把数据翻译成“像素”

**职责**：只读游戏数据，产出终端画面；处理中文宽度、颜色、差分刷新。

**片段一：显示宽度与补齐**

```python
def display_width(text: str) -> int:
    """Return how many terminal columns *text* occupies."""
    return sum(
        2 if unicodedata.east_asian_width(ch) in ("W", "F") else 1
        for ch in text
    )


def pad(text: str, width: int = CELL_WIDTH) -> str:
    """Right-pad *text* with spaces so it fills *width* columns."""
    missing = width - display_width(text)
    if missing > 0:
        return text + " " * missing
    return text
```

**片段二：差分刷新**

```python
    def draw(self, lines: list[str]) -> None:
        """Write *lines* at the top of the screen, touching only changes."""
        size = shutil.get_terminal_size((80, 24))
        if (size.columns, size.lines) != self._size:
            self._size = (size.columns, size.lines)
            self._lines = []          # 窗口大小变了，强制整屏重画
        out = []
        for index in range(max(len(lines), len(self._lines))):
            new_line = lines[index] if index < len(lines) else ""
            old_line = self._lines[index] if index < len(self._lines) else None
            if new_line != old_line:
                out.append(f"\033[{index + 1};1H{new_line}\033[K")
        if out:
            sys.stdout.write("".join(out))
            sys.stdout.flush()
        self._lines = list(lines)
```

**片段三：HUD**

```python
def _hud_lines(game) -> list[str]:
    shield = "  护盾" if game.player.shield_time > 0 else ""
    hud = (
        f"{colorize('分数', 'bright_white')} {game.score:06d}  "
        f"{colorize('最高', 'bright_white')} {game.high_score:06d}  "
        f"{colorize('关卡', 'bright_white')} {game.stage}  "
        f"{colorize('生命', 'bright_white')} {game.lives}  "
        ...
    )
```

**分析**

1. **三件事**：构造字符画布（`board` 二维列表）→ 上色 → 输出。渲染顺序是“地形 → 道具 → 子弹 → 敌人 → 玩家 → 爆炸特效”，后面的会覆盖前面的；敌人如果站在森林格子上就**跳过不画**（树林藏敌）。这些规则全在 `render_frame` 里，游戏逻辑完全不知道。

2. **中文占两列的问题**。终端里“砖”占 2 列宽，“R”占 1 列。如果一行里混着 1 列和 2 列的字符，表格就会错位。解决方案：
   - 把每个逻辑格子固定渲染成 3 列（`CELL_WIDTH = 3`）：一个汉字 2 列 + 1 个空格；3 个 ASCII 字符刚好 3 列；
   - `display_width()` 用 `unicodedata.east_asian_width()` 判断字符是宽（W/F）还是窄，`pad()` 负责补齐。
   - HUD 不参与格子对齐，用 `\033[K` 清掉行尾即可。

3. **差分刷新**。如果每帧清屏重画 19 行，终端会闪、输出量也大。`Renderer` 缓存上一帧的每一行，**只写发生变化的行**：
   - `\033[{row};1H` 把光标移到第 row 行第 1 列；
   - 写入新行；
   - `\033[K` 清除这一行光标之后的内容。
   于是静止的墙面完全不会被重写，只有移动的坦克/子弹和 HUD 在变。60 FPS 因此非常轻松。

4. **窗口大小变化时强制整屏重画**。`self._size` 和当前终端尺寸一比，不同就把缓存清空——否则某些没变的行永远不会重画，画面会花。

5. **备用屏幕与光标**。`Renderer.start()` 里发送 `\033[?1049h` 切到终端的“备用屏幕”，退出时 `\033[?1049l` 切回来——玩家的终端历史不会被游戏画面刷满。隐藏光标（`\033[?25l`）避免闪烁，退出时再显示。

6. **`display.py` 不导入 `game.py`**。`render_frame(game)` 只要求参数对象有 `grid`、`player`、`enemies`、`bullets`、`state` 等属性。测试渲染时，可以造一个“假 game”，不需要初始化整个游戏。

---

### 8B.2.9 input_handler.py：和终端打交道

**职责**：在任何平台上，非阻塞地拿到“玩家按了哪些键”，并翻译成动作名（`"up"`、`"fire"`、`"pause"`……）。

**片段一：纯函数解析**

```python
def parse_keys(buffer: str, now: float, last_data: float) -> tuple[list[str], str]:
    """Split *buffer* into key names, keeping incomplete escapes buffered."""
    keys: list[str] = []
    index = 0
    while index < len(buffer):
        char = buffer[index]
        if char == "\x1b":
            rest = buffer[index:]
            if rest.startswith("\x1b["):
                if len(rest) < 3:
                    break                      # 转义序列还没收全，等下次
                code = rest[2]
                if code in _ARROW_CODES:
                    keys.append(_ARROW_CODES[code])
                    index += 3
                    continue
                ...
            if index == len(buffer) - 1:
                if now - last_data > _ESCAPE_GRACE:
                    keys.append("pause")
                    index += 1
                break
            index += 1
            continue
        action = _KEY_CHARS.get(char.lower())
        if action:
            keys.append(action)
        index += 1
    return keys, buffer[index:]
```

**片段二：上下文管理器进出 raw 模式**

```python
class InputHandler:
    """Puts the terminal in raw mode and polls it without blocking."""

    def __enter__(self) -> "InputHandler":
        self._buffer = ""
        self._last_data = time.monotonic()
        if os.name == "nt":
            import msvcrt
            self._msvcrt = msvcrt
        else:
            import termios
            import tty
            self._termios = termios
            self._fd = sys.stdin.fileno()
            self._old_settings = termios.tcgetattr(self._fd)
            tty.setraw(self._fd)
        return self

    def __exit__(self, exc_type, exc, traceback) -> bool:
        if os.name != "nt":
            self._termios.tcsetattr(self._fd, self._termios.TCSADRAIN,
                                    self._old_settings)
        return False
```

**分析**

1. **平台差异被彻底隔离**。Unix 用 `termios` + `tty` 进 raw 模式，Windows 用 `msvcrt`；其他模块只知道“`read_keys()` 返回动作名列表”。

2. **raw 模式**：默认的终端是“行缓冲”——你按了键，程序要等你按回车才收到。raw 模式让每个按键立刻可见，代价是**必须自己恢复终端设置**（否则用户退出后终端会变得很奇怪）。这就是 `__enter__` / `__exit__` 的职责，也是 `with` 语句存在的意义（8C.7）。

3. **非阻塞轮询**：Unix 用 `select.select(..., timeout=0)` 问“现在有数据吗”，有才 `os.read`；Windows 用 `msvcrt.kbhit()`。这样游戏循环永远不会卡在等按键上。

4. **方向键是转义序列**。按一次 ↑，终端实际发出的是一串字符 `\x1b[A`（ESC + `[` + `A`）。而且这串字符**可能分两次到达**（比如网络终端、慢速串口）。`input_handler` 用一个 `buffer` 保存“还没解析完的尾巴”：
   - 收到 `\x1b[`：不够 3 个字符，先留着；
   - 下次收到 `A`：拼成 `\x1b[A`，解析为 `"up"`。

5. **单独的 ESC 键有二义性**。用户按 Esc 发一个 `\x1b`；但 ↑ 的第一个字节也是 `\x1b`。程序不能立刻判断。这里的策略是：如果 `\x1b` 是缓冲区里最后一个字符，且距离上次收到数据超过 `_ESCAPE_GRACE`（0.12 秒），才当作“用户按了 Esc”（映射为暂停）；否则继续等。

6. **`parse_keys` 被写成纯函数**：输入“缓冲区字符串 + 两个时间戳”，输出“按键列表 + 剩余缓冲区”，不碰任何全局状态。这样它可以**不启动游戏单独测试**（8D.2 练习七）。这是从“能跑”走向“可维护”的典型手段。

7. **Ctrl+C 变成了普通字节**。raw 模式关闭了终端的信号处理，所以 Ctrl+C 不会触发 `KeyboardInterrupt`，而是送来字节 `\x03`。程序把它映射为 `"quit"`。

---

### 8B.2.10 main.py：薄薄的入口

**职责**：检查终端、创建对象、跑主循环、退出前恢复现场。

**主循环（节选）**：

```python
    try:
        with InputHandler() as keys:
            renderer.start()
            try:
                last = time.perf_counter()
                accumulator = 0.0
                next_frame = last
                while not game.quit:
                    now = time.perf_counter()
                    for key in keys.read_keys():
                        game.handle_input(key)

                    # Fixed-step simulation, independent of render speed.
                    accumulator += min(now - last, 0.25)
                    last = now
                    while accumulator >= LOGIC_STEP:
                        game.update(LOGIC_STEP)
                        accumulator -= LOGIC_STEP

                    if now >= next_frame:
                        size = shutil.get_terminal_size((80, 24))
                        if (size.columns < MIN_COLS
                                or size.lines < MIN_LINES):
                            renderer.draw([_too_small_message()])
                        else:
                            renderer.draw(render_frame(game))
                        next_frame = now + FRAME_TIME

                    time.sleep(INPUT_POLL_SLEEP)
            finally:
                renderer.stop()
    except KeyboardInterrupt:
        pass
```

**分析**

1. **入口要薄**。`main.py` 里没有一条游戏规则：它不知道子弹怎么飞、敌人怎么想。它只做“组装 + 循环 + 收尾”。以后如果想做 Python 网络版，只需要换一个 `main` 和 `display`，`game.py` 原样复用。

2. **三个时间尺度在一个循环里**：
   - `keys.read_keys()` 每轮都调用（约 500 Hz）；
   - 逻辑按固定步长 `LOGIC_STEP` 追帧（`while accumulator >= LOGIC_STEP`）；
   - 渲染按 `FRAME_TIME` 节流（`if now >= next_frame`）。
   这就是 8C.19 要展开的“固定步长 + 帧率解耦”。

3. **`min(now - last, 0.25)` 是保险丝**。如果电脑卡了 10 秒（比如系统休眠），差值会是 10，累加器一口气补 1200 步逻辑，游戏瞬间“快进”，还可能越补越卡。限制单次最多 0.25 秒，牺牲一点时间精度，换来稳定。

4. **`try/finally` 保证收尾**。即使循环里抛出异常（或用户 Ctrl+C），`renderer.stop()` 也会执行，光标会恢复、备用屏幕会退出。用户在 `main()` 外层还捕获了 `KeyboardInterrupt`，避免打印难看的 traceback。

5. **退出后打印成绩**。`game.save_high_score()` 在 finally 之后调用，此时终端已经恢复正常，`print` 的内容会留在用户的正常屏幕里。

---

### 8B.2.11 README.md 与 highscore.txt：数据文件和文档也是项目的一部分

- `README.md`：写清楚“怎么运行、按哪些键、图例是什么意思、每个模块干什么”。别人拿到项目，先读它。
- `highscore.txt`：游戏运行后自动生成的一行数字。它**不应该**提交到版本库，可以用 `.gitignore` 忽略它。
- 这两个文件提醒我们：一个项目不只是 `.py`。文档和运行数据同样需要规划。

---

### 8B.2.12 小结：模块边界是怎么定的

回头看这一圈，模块的划分标准其实只有三条：

1. **数据 vs 规则**：地图数据（battlefield）、配置数据（config）与游戏规则（game）分开；
2. **通用 vs 专用**：坦克的公共部分（tank）与玩家/敌人各自的逻辑（player/enemy）分开；
3. **核心 vs 外围**：游戏规则（game）与输入/输出（input_handler/display/main）分开。

每一条都对应一个具体的修改场景：换地图、加敌人、换终端。**模块边界应该贴着“最可能变化的点”来画。**

---

项目本身看完了。你大概已经注意到：它能跑起来，靠的不只是这些模块，还有一批前 7 节没讲过的语法和标准库——下一节（8C）逐条补齐。
