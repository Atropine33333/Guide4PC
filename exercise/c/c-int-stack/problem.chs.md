栈的脾气是后进先出：最后压进去的，最先被取走。现在有一叠能放 128 个 `int` 的位置，你要把这叠位置管起来：压栈、弹栈、看看顶上是谁、报出当前层数。

**接口**：

```c
#define INT_STACK_CAP 128

typedef struct {
    int data[INT_STACK_CAP];
    size_t size;
} IntStack;

void stack_init(IntStack *s);
int stack_push(IntStack *s, int value);
int stack_pop(IntStack *s, int *out);
int stack_peek(const IntStack *s, int *out);
int stack_size(const IntStack *s);
```

- `stack_push` 成功返回 `0`；已经放满 128 个时返回 `-1`，栈保持不变。
- `stack_pop` 成功把栈顶写进 `*out` 并返回 `0`；空栈返回 `-1`，`*out` 不动。
- `stack_peek` 只看不取；空栈返回 `-1`。
- `stack_size` 返回当前元素个数。

### 示例

依次压入 `7`、`9`，栈顶是 `9`；弹一次得到 `9`，再弹一次得到 `7`；此时栈为空，再弹返回 `-1`。

### 约定

- 只需要实现这些函数，不要写 `main`。
- 结构体按值复制后，两个栈各管各的，互不牵连。
- 不要用动态分配；容量就是 `INT_STACK_CAP`。
