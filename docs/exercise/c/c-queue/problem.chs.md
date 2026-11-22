一排窗口，八个位置，先来的先办。队伍从队头取人，从队尾进人；队头一路往后挪，挪到第八个格之后回到第一格，像转盘一样接着排。

**接口**：

```c
#include <stddef.h>

#define QUEUE_CAP 8

typedef struct {
    int data[QUEUE_CAP];
    size_t head;
    size_t len;
} IntQueue;

void queue_init(IntQueue *q);
int queue_push(IntQueue *q, int value);
int queue_pop(IntQueue *q, int *out);
size_t queue_size(const IntQueue *q);
int queue_is_empty(const IntQueue *q);
int queue_is_full(const IntQueue *q);
```

- `queue_init` 把队头下标与长度置为 `0`。
- `queue_push` 把 `value` 放到队尾；队列已满返回 `-1`，队列不变。
- `queue_pop` 把队头元素写进 `*out`，返回 `0`；空队列返回 `-1`，`*out` 不动。
- `queue_size` 返回当前人数；`queue_is_empty` 空时返回 `1`；`queue_is_full` 满时返回 `1`。
- 位置只有 `QUEUE_CAP` 个，队尾下标要绕回 `0`。

### 示例

队列容量为 8。依次放入 `1` 到 `8` 后队列满；取走 `1` 到 `5`，再放入 `9`、`10`、`11`，此时队里是 `6 7 8 9 10 11`。连续取六次，得到的顺序正是 `6 7 8 9 10 11`，再取返回 `-1`。

### 约定

- 只实现这些函数，不要写 `main`。
- 不要用动态分配，容量就是 `QUEUE_CAP`。
- 无论队头绕到哪一格，顺序都要保持先进先出。
- 结构体保持题面给出的字段，函数签名不变。
