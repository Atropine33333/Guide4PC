这一题不读 stdin。用两个栈实现一个先进先出的队列。

```cpp
#include <vector>

class TwoStackQueue {
public:
    void push(int value);
    int front() const;
    void pop();
    bool empty() const;
    int size() const;
};
```

约定：

- 队列内部的存储只能使用 `std::vector`，把它当作栈使用（`push_back` / `back` / `pop_back`）。
- `push` 把元素放到队尾。
- `front` 返回队首元素；队列为空时返回 `-1`。
- `pop` 删除队首元素；队列为空时什么也不做。
- `empty` 和 `size` 描述当前元素个数。
- 不要写 `main`。

### 提示

维护两个栈：`in` 接收新元素，`out` 提供队首。只有 `pop` 时 `out` 为空才需要把 `in` 全部倒入 `out`。`front` 可以直接看 `in` 的底部（`in.front()`），不必搬动元素。
