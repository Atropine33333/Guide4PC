# 营地守则

营地门口钉着一块木牌，边角被雨泡得发黑，六行字还认得出来。

一、本营只有一块地，叫营地，共 256 字节。进场先清点，已用记作 0。

二、领东西从地头往后排，每件占的地方按 8 字节取整，起头一律落在 8 的整数倍上。

三、地不够就当场回绝。回绝时已用的数目不改，先前领走的东西一寸不动。

四、领 0 件不发东西，已用也不改。

五、拔营即清空：已用归 0，地上的旧痕不扫。下一批人仍从地头排起，第一件的起头与上一批第一件相同。

六、已用多少，随时报得出来。

**人话**：实现一个固定大小的营地分配器：`arena_init` 做清点，`arena_alloc` 从已用处按 8 字节取整往后切一块，`arena_reset` 把已用归 0，`arena_used` 报出当前已用字节数。

**接口**：

```c
#include <stddef.h>

#define ARENA_CAP 256

typedef struct {
    _Alignas(8) unsigned char buf[ARENA_CAP];
    size_t used;
} Arena;

void arena_init(Arena *a);
void *arena_alloc(Arena *a, size_t n);
void arena_reset(Arena *a);
size_t arena_used(const Arena *a);
```

- `arena_init`：把 `used` 置为 0。
- `arena_alloc(a, n)`：`n == 0` 返回 `NULL`；`n > ARENA_CAP` 返回 `NULL`；需要 `((n + 7) / 8) * 8` 字节；`used + 需要 > ARENA_CAP` 时返回 `NULL`，`used` 不变；否则返回 `a->buf + used`，并把 `used` 加上需要的字节数。
- 返回的地址是 8 的整数倍，从该地址起 `n` 字节可读可写。
- `arena_reset`：把 `used` 置为 0。旧字节不擦除，下一件仍从 `a->buf` 开始排。
- `arena_used`：返回当前已用字节数。
- 没有单独归还，整块地一起清空。

### 示例

```c
Arena a;
arena_init(&a);                 /* used 为 0 */
void *p = arena_alloc(&a, 16);  /* p == a.buf，used 变 16 */
void *q = arena_alloc(&a, 20);  /* q == a.buf + 16，20 取整为 24，used 变 40 */
arena_reset(&a);                /* used 归 0 */
void *r = arena_alloc(&a, 16);  /* r == p，从地头重排 */
```

### 约定

- 只实现这四个函数，不要写 `main`。
- 营地就是结构体里的 `buf`，不要另外申请动态内存。
- `used` 按取整后的字节数累计，所以它总是 8 的整数倍。
- 回绝请求时 `used` 与已有数据都不变。
- 领到的每一块都可读可写，写入一块不影响另一块的内容。
- 越界的请求不会写坏任何东西。
