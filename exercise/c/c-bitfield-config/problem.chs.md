【深空站 · 环控舱面板】

环控舱的配置只有一排小灯：模式三盏、档位四盏、总开关一盏，挤在同一块面板上。

值班员把这三组写进去，再读回来核对。写入之前先看值装不装得下：模式最多七，档位最多十五，总开关只有开与关两种说法。装不下就退回，原来的灯不动。

**人话**：实现下面这组函数，用位域结构体保存 `mode`（3 位）、`level`（4 位）、`enabled`（1 位）；设置时检查上界，越界返回 `-1` 且不改动字段；读取时把当前值交回来。

**接口**：

```c
#define CFG_MODE_MAX 7u
#define CFG_LEVEL_MAX 15u

typedef struct {
    unsigned int mode : 3;
    unsigned int level : 4;
    unsigned int enabled : 1;
} Cfg;

void cfg_init(Cfg *c);
int cfg_set_mode(Cfg *c, unsigned mode);
int cfg_set_level(Cfg *c, unsigned level);
int cfg_set_enabled(Cfg *c, int on);
unsigned cfg_mode(const Cfg *c);
unsigned cfg_level(const Cfg *c);
int cfg_enabled(const Cfg *c);
```

- `cfg_init`：三个字段全部清 `0`。
- `cfg_set_mode`：`mode` 不超过 `CFG_MODE_MAX` 时写入并返回 `0`，否则返回 `-1`。
- `cfg_set_level`：`level` 不超过 `CFG_LEVEL_MAX` 时写入并返回 `0`，否则返回 `-1`。
- `cfg_set_enabled`：只接受 `0` 与 `1`，写入并返回 `0`；其他值返回 `-1`。
- 三个读取函数分别交回字段当前的值，`cfg_enabled` 用 `0` 与 `1` 表示。

### 示例

`cfg_init` 之后三个字段都是 `0`。依次写入模式 `5`、档位 `12`、总开关 `1`，三次都返回 `0`；读回来正是 `5`、`12`、`1`。再往档位写 `16` 会返回 `-1`，档位仍是 `12`。

### 约定

- 结构体字段的名字、类型、位宽与声明顺序保持原样。
- 每个设置函数只改自己那一个字段，旁边的字段原封不动。
- 写入被拒绝时，字段保持调用前的值。
- 这些函数会被直接调用，不要写 `main`。
- 只使用标准库，不要动态分配。
