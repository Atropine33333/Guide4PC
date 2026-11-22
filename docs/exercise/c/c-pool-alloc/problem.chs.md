（货运飞船的舱单。整艘船只有一排货格，编号 0 到 7，每格恰好 32 字节。）

船长说：别问这趟装什么，格子就这么大，装不下的东西不上船。

装货时从编号最小的空格开始，一格一格往外发；八格发完，再有人来，只能回一句“没格了”。

卸货也认死理：只认格子的起始地址。同一个格子卸两回，第二回不认；拿一个不在舱单上的地址来，也不认。

靠港之后统一清舱，八格重新算空，下一趟从头再发。

**人话**：实现一个定长块池。`pool_init` 清空所有格子；`pool_alloc` 借出下标最小的空闲格；`pool_free` 只接受仍在占用中的格子首地址；`pool_reset` 一次收回全部格子。

**接口**：

```c
#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 8

typedef struct {
    unsigned char cells[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
    unsigned char used[POOL_BLOCK_COUNT];
    int in_use;
} Pool;

void pool_init(Pool *p);
void *pool_alloc(Pool *p);
int pool_free(Pool *p, void *block);
void pool_reset(Pool *p);
int pool_in_use(const Pool *p);
```

- `pool_init` 把 `used` 全部清 `0`，`in_use` 置 `0`。
- `pool_alloc` 返回下标最小的空闲格子的首地址，并把它标成占用；没有空格子或 `p` 为 `NULL` 时返回 `NULL`。
- `pool_free` 在 `block` 正好是某个占用中格子的首地址时归还它并返回 `0`。
- `block` 为 `NULL`、不是格子首地址、或者那一格本来就空着时，`pool_free` 返回 `-1`，池子保持不变。
- `pool_reset` 收回全部格子，`in_use` 归零；`pool_reset(NULL)` 什么也不做。
- `pool_in_use` 返回当前占用格数。

### 示例

```c
Pool p;
pool_init(&p);
void *a = pool_alloc(&p);        /* &p.cells[0] */
void *b = pool_alloc(&p);        /* &p.cells[1] */
pool_free(&p, a);                /* 0 */
pool_free(&p, a);                /* -1，已经空着 */
void *c = pool_alloc(&p);        /* 又是 &p.cells[0] */
pool_reset(&p);                  /* 八格全空 */
```

### 约定

- 只需要实现这五个函数，不要写 `main`。
- 格子就是 `Pool` 里的数组，不额外申请内存。
- 发出去的格子内容由调用者自己写，`pool_free` 不清零。
- 归还之后，同一格可以再次发出去复用。
