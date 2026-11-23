营地是一次划好的一大块地。要住进来的人不各自去申请，而是找营地的管理员：管理员手里记着从哪里开始还空着，来一个人就切一块给他，切完把线往后挪。

这样申请次数极少，归还也简单：整个营地一起退掉，或者把线拨回开头重新分配。

**人话**：`arena_init` 一次性申请 `bytes` 字节；`arena_alloc` 从 `used` 处切出 `n` 字节；`arena_strdup` 把字符串连结尾符一起放进来；`arena_reset` 只把 `used` 拨回 `0`；`arena_free` 把整块地还掉。

**接口**：

```c
typedef struct {
    char *base;
    size_t cap;
    size_t used;
} Arena;

int arena_init(Arena *a, size_t bytes);
void *arena_alloc(Arena *a, size_t n);
char *arena_strdup(Arena *a, const char *s);
void arena_reset(Arena *a);
void arena_free(Arena *a);
size_t arena_used(const Arena *a);
```

- `arena_init` 成功返回 `0`；`bytes` 为 `0` 或申请失败返回 `-1`。
- `arena_alloc` 返回切片首地址，并把 `used` 增加 `n`；放不下或 `n` 为 `0` 时返回 `NULL`，`used` 保持不变。
- 切片按申请的先后紧挨着排，第一片就在 `base`，第二片接在第一片后面。
- `arena_strdup` 占用 `strlen(s) + 1` 字节，把 `s` 连同 `'\0'` 复制进去；`s` 为 `NULL` 或放不下时返回 `NULL`。
- `arena_reset` 把 `used` 置 `0`，`base` 与 `cap` 不动。
- `arena_free` 归还整块地，`base` 置 `NULL`、`cap` 与 `used` 置 `0`；对已经归还的营地再调用一次也不会出事。
- `arena_used` 返回当前用掉的字节数。

### 示例

```c
Arena a;
arena_init(&a, 64);              /* 一块 64 字节的地 */
char *s = arena_strdup(&a, "hi");    /* 占 3 字节，used == 3 */
void *p = arena_alloc(&a, 10);       /* 紧接着切 10 字节，used == 13 */
arena_reset(&a);                     /* used 回到 0，地还在 */
arena_free(&a);                      /* 整块还掉 */
```

### 约定

- 只需要实现这六个函数，不要写 `main`。
- `arena_init` 只申请一次；`arena_alloc` 与 `arena_strdup` 不再向系统要内存。
- 切片不重叠，后切的地址不小于先切的地址。
