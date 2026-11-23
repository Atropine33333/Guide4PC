《方砖仓使用说明书》

型号 SQ-4096。用途：按需切出一批方砖，用毕归还。

**装机**：把下面三个函数实现好，接进主程序：

```c
#define SQ_MAX 4096

int *make_squares(int n);
void release_squares(int *p);
long live_blocks(void);
```

**操作规程**

1. `make_squares(n)`：领一块能放 `n` 个 `int` 的空地，依次写入 `0, 1, 4, ..., (n-1)*(n-1)`，返回这块地的首地址。`n` 须在 `1` 到 `SQ_MAX` 之间；越界或系统无地可领时返回 `NULL`，账上不动。
2. `release_squares(p)`：把 `make_squares` 领来的地还回去，账上减一。`p` 为 `NULL` 时什么也不做。
3. `live_blocks()`：报出账上还没还的地块数。

**故障现象**：眼下这台机器出库照做，归还那道工序空着，账只增不减；出库时也不先看看系统有没有把地批下来，空着手也往下写。

**人话**：修好 `make_squares` 与 `release_squares`：领空地之前先看批没批下来，越界的请求直接回绝，还地时真正归还并把账销掉，遇到空指针不记账。

**接口**：

```c
#define SQ_MAX 4096

int *make_squares(int n);
void release_squares(int *p);
long live_blocks(void);
```

### 示例

```c
int *p = make_squares(3);   /* p[0]=0，p[1]=1，p[2]=4；账上 1 块 */
release_squares(p);         /* 归还，账上 0 块 */
release_squares(NULL);      /* 什么也不做，账上仍是 0 块 */
```

### 约定

- 不要改 `SQ_MAX`，也不要写 `main`。
- 每块地只归还一次；`release_squares` 只接受 `make_squares` 返回的指针。
- 账目要准：所有地都归还之后，`live_blocks()` 必须回到 `0`。
- 领地失败时返回 `NULL`，这时账上不许增加。
