给分配器配一本账：每借出一块内存记一笔，每收回一块再记一笔。账本只有两个数，借出总数与收回总数；两者之差就是眼下还没还的块数。记账要老实：借不到的不记，空指针不记。

**人话**：把 `malloc` 和 `free` 包一层。`ac_alloc` 申请成功才把 `allocs` 加一；`ac_free` 拿到非空指针才把 `frees` 加一；`ac_outstanding` 返回两者之差。

**接口**：

```c
typedef struct {
    size_t allocs;
    size_t frees;
} AllocStats;

void ac_init(AllocStats *st);
void *ac_alloc(AllocStats *st, size_t size);
void ac_free(AllocStats *st, void *p);
long ac_outstanding(const AllocStats *st);
```

- `ac_init` 把 `allocs` 与 `frees` 都置 `0`，旧数字一概不算。
- `ac_alloc` 申请 `size` 字节；成功返回指针并把 `allocs` 加一。
- `size` 为 `0`、申请失败或 `st` 为 `NULL` 时返回 `NULL`，并且不加计数。
- `ac_free` 归还 `p`；`p` 为 `NULL` 时什么也不做，`frees` 不动。
- `ac_outstanding` 返回 `allocs - frees`，结果不小于 `0`。

### 示例

```c
AllocStats st;
ac_init(&st);
void *a = ac_alloc(&st, 10);   /* allocs = 1 */
void *b = ac_alloc(&st, 20);   /* allocs = 2 */
ac_free(&st, a);               /* frees = 1 */
long left = ac_outstanding(&st);   /* 1 */
ac_free(&st, b);                   /* 还剩 0 块 */
```

### 约定

- 只需要实现这四个函数，不要写 `main`。
- 只用标准库的内存函数；账本里的两个计数都用 `size_t`。
- 只归还自己借出去的指针；`ac_free` 不负责检查指针来历。
