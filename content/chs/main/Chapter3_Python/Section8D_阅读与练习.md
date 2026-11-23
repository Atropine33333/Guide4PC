# 第8D节：阅读方法与扩展练习

前几节把项目读完了、把语法补上了。这一节讲“方法”：当你拿到一个完全陌生的多文件项目时，应该从哪里下手；然后再用 8 个练习，把 TankBattle 真正变成你自己的项目。

---

## 8D.1 怎样阅读一个多文件项目

读代码和写代码一样重要。给你一个陌生的多文件项目，不要从第一个文件的第一行开始硬啃。下面这套方法，用 TankBattle 演示。

### 8D.1.1 第一步：读文档和入口

1. 先读 `README.md`：知道它是什么、怎么运行、按哪些键；
2. 再读 `main.py`：这是入口，也是**调用关系的总目录**。它 import 了谁，就说明游戏由哪几块拼成。

`main.py` 只有 100 行，读完你就知道：输入来自 `InputHandler`，逻辑在 `Game`，画面由 `render_frame` + `Renderer` 画。

### 8D.1.2 第二步：画依赖图

把所有文件的 `import` 语句抄下来，就是一张依赖图（8B.1.3 那张图就是这么画的）。画完你会自然发现：

- `config.py` 被所有人依赖，自己谁也不依赖；
- `game.py` 是唯一同时依赖所有实体的模块；
- 没有任何模块导入 `main.py` 和 `display.py` 的“上层”。

依赖图能回答“改这个文件会影响谁”，这在维护时非常有用。

### 8D.1.3 第三步：追一条调用链

选一个你最感兴趣的功能，从按键追到结果。**追调用链是读代码效率最高的方式**，因为它只走你关心的路径，不会迷失在无关代码里。

**练习一：按下 `J` 之后发生了什么？**

```
1. main.py 主循环
   keys.read_keys() 返回 ["fire"]
2. game.handle_input("fire")
   → self.player.handle_key("fire")
   → player.fire_held = True（只是记下意图）
3. 下一次 game.update(1/120)
   → _update_playing → player.update(dt, self)
   → if self.fire_held and self.can_fire(game): self.shoot(game)
4. Tank.shoot(game) → game.spawn_bullet(self)
5. Game.spawn_bullet
   → bullet = Bullet(...)  瞬间创建
   → tank.cooldown = tank.shoot_cooldown
   → self._step_bullet(bullet)   枪口立刻结算一格
6. 之后每一帧 _update_bullets
   → bullet.accum 攒够 bullet.interval
   → _step_bullet(bullet)
   → 检查越界 → 检查地形（砖/钢/基地）→ 检查敌方坦克
7. 命中敌人
   → enemy.take_damage()
   → 如果死了：_kill_enemy(enemy) → 加分、爆炸特效、可能掉道具
8. display.render_frame(game)
   → 子弹和敌人的位置被画出来
```

这一路经过 `main.py → game.py → player.py → tank.py → bullet.py → game.py → display.py`。你只读了相关函数，却把“输入→逻辑→渲染”整条线串了起来。

**练习二：敌人被“冻结”时，谁阻止了它们？**

```
1. 玩家吃到钟道具
   → game._collect(powerup) → self.freeze_time = FREEZE_TIME
2. 下一帧 enemy.update(dt, self) 的第一行
   → if game.freeze_time > 0: return
   → 直接返回：不移动、不重新规划、不开火
3. _update_playing 开头
   → self.freeze_time -= dt   （冻结时间每帧减少）
4. display 完全不知道“冻结”这件事
   → 敌人看起来只是“站着不动”
5. 玩家吃到道具时 HUD 显示消息
   → game.set_message("敌人被冻结！", ...) → display._hud_lines 读 message
```

结论：**一个效果可以横跨多个模块**：数据在 `config`（`FREEZE_TIME`）、状态在 `game`、行为拦截在 `enemy`、提示在 `display`。追一遍你就理解了这个效果是怎么“无侵入”实现的。

### 8D.1.4 第四步：追一份数据

挑一个数据，看它的一生。比如 `grid`（地图）：

| 阶段 | 在哪里 | 发生什么 |
|------|--------|----------|
| 出生 | `battlefield.create_battlefield` | 生成二维列表 |
| 使用 | `game.can_tank_enter` | 判断坦克能不能进 |
| 修改 | `game._step_bullet` | 子弹把砖块改成 `FLOOR` |
| 变化 | `game._destroy_base` | 基地变成 `BASE_RUBBLE` |
| 消费 | `display._build_board` | 翻译成带颜色的字符串 |
| 重生 | `game._start_stage` | 过关后重新生成 |

再比如 `score`：在 `_kill_enemy` 里增加，在 `_check_stage_clear` 里加奖励，在 `_update_extra_life` 里触发奖命，在 `_hud_lines` 里显示，在 `_game_over` 里保存。

**数据的一生比调用链更能揭示模块的职责。**

### 8D.1.5 第五步：改一个常量验证理解

读懂了没有，改一下就知道：

- 把 `PLAYER_MOVE_INTERVAL` 从 `0.11` 改成 `0.5`：玩家慢如蜗牛——说明移动速度确实由它控制；
- 把 `TERRAIN_GLYPHS[BRICK]` 从 `"砖"` 改成 `"#"`：画面立刻变样——说明渲染只依赖配置；
- 把 `MAX_ENEMIES_ON_FIELD` 改成 `1`：场上永远只有一个敌人——说明刷怪受它限制。

如果改完没反应，说明你找错了地方；如果改完崩了，恭喜你发现了一处隐藏依赖。两种结果都比“读了一遍”收获大。

### 8D.1.6 小测验

1. `display.py` 里能不能直接 `from game import Game`？为什么？
2. 如果把 `tank.py` 改成 `from game import Game`，会发生什么？
3. `config.py` 里如果写了一个 `count = 0`，然后 `game.py` 和 `enemy.py` 都 `count += 1`，会出现什么现象？

<details>
<summary>参考答案</summary>

1. 技术上可以，但没必要，而且违背分层。`display` 只需要“有那些属性的对象”，不需要知道具体类；不导入 `game` 还能让 `display` 单独测试、单独复用。
2. 可能形成循环导入：`game.py` 导入 `tank.py`，`tank.py` 又导入 `game.py`。而且 `tank.py` 会从“纯数据+行为”变成依赖整个世界，无法单独测试。项目用“把 `game` 当参数传进来”解决了这个问题。
3. 两个模块引用的是**同一个** `config.count` 对象，而且 `count += 1` 不是原子操作（先读后写）。单线程下勉强能用，但状态共享让任何一处改动都可能影响别处——这正是“`config.py` 只放常量”的原因。

</details>

---

## 8D.2 扩展练习

下面 8 个练习覆盖“改数据 → 改逻辑 → 加模块 → 重构 → 测试”，建议按顺序做。每题给出**涉及文件**和**关键思路**，代码自己写。

### 练习一：调整手感（难度：★）

把游戏改成“慢速重炮”风格：

- 玩家移动更慢、子弹更慢，但敌人也更慢；
- 每关敌人更多；
- 画面 30 FPS。

**涉及文件**：`config.py`。
**用到**：8B.2.1 的常量表、8C.19 的时间参数。
**验证**：改完直接跑，感受一下“数值即代码”。

### 练习二：加一种敌人（难度：★）

加一种“重型敌人”：`重`，3 点血，移动慢（speed 0.5），子弹慢（0.8），击杀 500 分。

**涉及文件**：`config.py`（`ENEMY_TYPES` 加一项即可）。
**思考**：为什么不用改 `enemy.py`？这正是数据驱动的意义。
**进阶**：让重型敌人在关卡 3 之后才出现——改 `game._pick_enemy_type` 的权重表，让它的权重从 0 开始。

### 练习三：加一种道具（难度：★★）

加“地雷”道具 `雷`：玩家拾取后，在当前位置放一个地雷；敌人踩到时爆炸，消灭该敌人。

**涉及文件**：`config.py`（`POWERUP_TYPES`、`POWERUP_WEIGHTS`）、`game.py`（`_collect` 分支、新增地雷列表、敌人移动后的检查）、`display.py`（把地雷画出来）。
**用到**：8C.14 的 `random.choices`、8C.13 的副本遍历。
**提示**：先实现“拾取后立即爆炸消灭最近的一个敌人”这种简化版，再考虑放置与触发。

### 练习四：加一种地形“冰面”（难度：★★）

冰面 `冰`：坦克可以通过，但在冰面上移动间隔减半（更滑、更快），子弹照常飞过。

**涉及文件**：`config.py`（新常量、`TERRAIN_GLYPHS/COLORS`）、`battlefield.py`（生成冰面）、`tank.py`（读到 `game.grid[self.row][self.col]` 判断是否在冰面上，返回有效移动间隔）。
**用到**：8C.3 的 `@property`（可以给 `Tank` 加一个 `effective_move_interval`）。
**思考**：判断“在冰上”需要地图信息，而 `Tank` 不导入 `game`——所以要么把 `game` 传进 `try_move`（已经有这个参数），要么在 `game` 里算好再告诉坦克。你选哪种？为什么？

### 练习五：加“基地加固”道具（难度：★★★）

经典坦克大战里的“铁锹”：拾取后基地周围的砖墙变成钢墙，持续 15 秒，然后变回砖墙（如果钢墙没被破坏）。

**涉及文件**：`config.py`、`game.py`（新增 `shovel_time` 计时；`_collect` 分支；每帧检查计时；一个“重砌护盾”的辅助函数）。
**用到**：8B.2.2 的 `_build_base`（把那段逻辑抽出来复用）、8C.6 的 `try/finally` 思路（计时结束一定要恢复）。
**提示**：注意“变回砖墙”要处理“钢墙已经被满火力玩家打掉”的情况——不能凭空造墙。

### 练习六：拆分 `game.py`（难度：★★★）

把 `game.py` 拆成三个文件：

- `collision.py`：`can_tank_enter`、`_step_bullet` 的碰撞判定；
- `spawning.py`：`_pick_enemy_type`、`_update_spawning`、`_enemy_total`；
- `game.py`：只留状态机与协调。

**涉及文件**：`game.py`，新增 `collision.py`、`spawning.py`。
**用到**：8A.2.9 的依赖倒置（新模块不要 `from game import Game`，而是把需要的对象当参数）。
**思考**：拆完以后 `import` 关系变成什么样？有没有产生循环？哪些函数反而变难调用了？——这就是“过度拆分”的边界。

### 练习七：为纯函数写测试（难度：★★）

下一节会系统讲**单元测试**与**集成测试**。先热身，给下面三个函数写单元测试：

1. `battlefield.create_battlefield(stage)`：边框全是钢；基地位置是 `BASE`；敌人出生点是 `FLOOR`；地图尺寸正确。
2. `battlefield.in_bounds(row, col)`：边界内/外的四种情况。
3. `input_handler.parse_keys(buffer, now, last_data)`：方向键序列 `\x1b[A` 解析为 `"up"`；半截序列要留在 buffer 里；`"q"` 解析为 `"quit"`。

**涉及文件**：新建 `tests/` 或 `test_xxx.py`。
**用到**：8B.2.9 的“纯函数好测试”、8C.6 的断言失败（`assert` 其实就是一个“不满足就抛异常”的语句）。

### 练习八：把画面换成 emoji / ASCII（难度：★）

- emoji 版：`砖→🧱`、`钢→⬜`、`我→🚀`、`敌→👾`……
- 纯 ASCII 版：`砖→#`、`钢→%`、`我→P`、`敌→E`。

**涉及文件**：`config.py` 的 `TERRAIN_GLYPHS` / `ENEMY_TYPES` / `PLAYER_CHAR`，可能还有 `CELL_WIDTH`。
**用到**：8B.2.8 的宽度计算。
**注意**：emoji 在终端里通常也是 2 列宽，但有些平台会显示成 1 列或 3 列，导致错位。先小范围试，确认终端表现再全改。如果改成纯 ASCII（全部 1 列宽），可以把 `CELL_WIDTH` 从 3 改成 1，画面会更紧凑——但 `pad()`、HUD 和横幅都要过一遍。

---

## 8D.3 本节你学到了什么

1. **单文件的五个问题**：找代码慢、改代码危险、协作冲突、复用困难、测试麻烦；拆分解决它们，但也有“需要提前规划边界”的代价。
2. **导入机制**：`import` / `from ... import ...` / `as`、多行导入、`__name__`、包与目录、绝对/相对导入、模块只加载一次。
3. **循环导入与其解法**：共用常量下沉、函数内延迟导入、依赖倒置（把对象当参数）。TankBattle 的依赖方向是“上层知道下层，下层不知道上层”。
4. **架构与分层**：常量层 → 基础数据层 → 实体层 → 世界规则层 → 外围层 → 入口；每层的职责和依赖图。
5. **实时游戏的结构**：状态机、固定步长更新、输入/逻辑/渲染三个时间尺度、子弹逐格结算、统一清尸。
6. **一批新语法与标准库**：类与继承、`@property`、异常完整用法、`with` 上下文管理器、类型注解进阶、条件表达式、链式比较、格式说明符、`any/all/sum`、遍历副本、`random/time/pathlib/shutil/unicodedata`。
7. **终端编程进阶**：ANSI 定位与清行、备用屏幕、双宽字符对齐、非阻塞输入、平台判断、信号与优雅退出。
8. **两个工程模式**：固定步长循环（速度与帧率无关）、帧差分渲染（只画变化的部分）。
9. **读代码的方法**：读入口 → 画依赖图 → 追调用链 → 追数据 → 改常量验证。

下一节（8E，选学，不是必须掌握）会讲如何为这个多文件项目写测试：先用**单元测试**验证每个模块内部的函数（比如 `parse_keys`、`create_battlefield`、`display_width`），再用**集成测试**验证模块之间的协作（比如一整局游戏从刷怪到命中、从扣血到过关）——届时你会发现，这些函数之所以设计成纯函数，就是为了让测试变得轻松。
