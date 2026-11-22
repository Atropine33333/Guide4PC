《通道模块装配说明书》

§1 用途
本模块在一块配置总线上管理四条通道，编号从 `0` 到 `3`。每条通道带一个增益（`0` 到 `100`）和一个开关标记。

§2 常量
`CHANNEL_MAX` 为 `4`；`CHANNEL_MAGIC` 为 `0x5A`。

§3 类型
`Channel` 结构体保存通道号 `id`、增益 `gain` 和开关标记 `open`。

§4 守卫
接口声明块用 `G4_CHANNEL_H` 守卫，实现块用 `G4_CHANNEL_IMPL_H` 守卫。两处都写成 `#ifndef` 宏名、`#define` 宏名、正文、`#endif` 的样子，宏名重复一次写在 `#endif` 后面的注释里。同一份文件被读第二遍时，两个块各自整块跳过，声明与定义都只出现一次。

§5 接口

```c
#define CHANNEL_MAX 4
#define CHANNEL_MAGIC 0x5Au

typedef struct {
    int id;
    int gain;
    int open;
} Channel;

int channel_open(Channel *ch, int id, int gain);
int channel_set_gain(Channel *ch, int gain);
int channel_close(Channel *ch);
int channel_is_open(const Channel *ch);
unsigned channel_magic(void);
```

§6 行为
- `channel_open`：`ch` 为空返回 `-1`；`id` 不在 `0` 到 `CHANNEL_MAX - 1` 之间返回 `-1`；`gain` 不在 `0` 到 `100` 之间返回 `-1`。三项都通过时写入 `id`、`gain`，把 `open` 置 `1`，返回 `0`。
- `channel_set_gain`：`ch` 为空或 `open` 为 `0` 返回 `-1`；`gain` 越界返回 `-1`；否则改写增益并返回 `0`。
- `channel_close`：`ch` 为空或 `open` 为 `0` 返回 `-1`；否则把 `open` 置 `0` 并返回 `0`。
- `channel_is_open`：`ch` 为空或没打开返回 `0`，打开着返回 `1`。
- `channel_magic`：返回 `CHANNEL_MAGIC`。
- 任何失败都不改动结构体。

§7 调用方式
这些函数会被直接调用，请只实现守卫块与函数，不要写 `main`。

**人话**：把接口声明和函数实现分别用 `#ifndef` 守卫包好，守卫宏是 `G4_CHANNEL_H` 和 `G4_CHANNEL_IMPL_H`；再补完五个通道函数。守卫让同一段代码在第二次被读到时整块跳过。

**接口**：见 §5 的声明。

### 示例

```c
Channel ch;
channel_open(&ch, 2, 40);      // 0，ch.open 为 1
channel_set_gain(&ch, 101);    // -1，增益仍是 40
channel_close(&ch);            // 0
channel_close(&ch);            // -1，已经关掉了
```

### 约定

- 结构体与函数签名保持原样，不要改动字段名。
- 两个守卫宏定义为空宏即可，边界注释写在 `#endif` 后面。
- 出错时结构体保持调用前的样子。
- 只使用标准库，不要动态分配，不要写 `main`。
