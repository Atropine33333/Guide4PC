# 两头都能排

食堂窗口前的队伍，两头都有用：前头可以再挤进一个人，后头也能再续上一个。开饭时从前往后走，收摊时从后往前清。现在请你管住这样一支能装 64 个整数的双端队列。

**人话**：实现容量 64 的双端队列，前端和后端都能入队、都能出队；满了拒绝入队，空了拒绝出队。

**接口**：

```c
#define DEQUE_CAP 64

typedef struct {
    int data[DEQUE_CAP];
    int head;
    int size;
} Deque;

void deque_init(Deque *d);
int deque_push_front(Deque *d, int value);
int deque_push_back(Deque *d, int value);
int deque_pop_front(Deque *d, int *out);
int deque_pop_back(Deque *d, int *out);
int deque_size(const Deque *d);
```

- `deque_push_front` 与 `deque_push_back` 成功返回 `0`；已有 64 个元素时返回 `-1`，队列不变。
- `deque_pop_front` 与 `deque_pop_back` 成功把元素写进 `*out` 并返回 `0`；空队列返回 `-1`，`*out` 不动。
- `deque_size` 返回当前元素个数。

### 示例

依次 `deque_push_back(&d, 1)`、`deque_push_back(&d, 2)`、`deque_push_front(&d, 0)` 之后，从队头到队尾是 `0 1 2`。`deque_pop_front` 得到 `0`，`deque_pop_back` 得到 `2`，队列里只剩 `1`。

### 约定

- 只实现这些函数，不要写 `main`。
- 不使用动态分配；容量就是 `DEQUE_CAP`。
- 下标绕回 `0` 之后，队内顺序仍要保持。
- 操作失败时结构体内容不得改变。
