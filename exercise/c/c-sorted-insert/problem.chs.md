# 插进队伍

**人话**：维护一个从小到大的整数数组。每插入一个数，就把它放到合适的位置，让数组仍然保持非递减；数组最多放 64 个数。

**接口**：

```c
#define ARR_CAP 64

typedef struct {
    int data[ARR_CAP];
    int size;
} SortedArray;

void sa_init(SortedArray *a);
int sa_insert(SortedArray *a, int value);
int sa_size(const SortedArray *a);
```

- `sa_init` 把数组置空。
- `sa_insert` 成功返回 `0`；已有 64 个元素时返回 `-1`，数组不变。
- `sa_size` 返回元素个数。

### 示例

依次插入 `5`、`1`、`4` 之后，`data[0..2]` 是 `1 4 5`，`sa_size` 为 `3`。

### 约定

- 只实现这些函数，不要写 `main`。
- 不使用动态分配；容量就是 `ARR_CAP`。
- `data[0]` 到 `data[size - 1]` 始终非递减。
- 相等的数都要保留，先来后到不限。
- 未用到的数组位置内容不作要求。
