请自己实现一个最小堆（不要用 `std::priority_queue`），并补全下面的类：

```cpp
class MinHeap {
public:
    void push(int value);
    int pop();                 // 弹出并返回最小值
    int top() const;           // 返回最小值，不弹出
    std::size_t size() const;
    bool empty() const;
};
```

评测代码会直接包含你的实现并调用这些成员函数。**不要写 `main`。**

### 输入

无。

### 输出

无。评测代码检查函数返回值。

### 示例

```cpp
MinHeap heap;
heap.push(5);
heap.push(1);
heap.push(3);
heap.pop();   // 返回 1
heap.top();   // 返回 3
```

### 约束

- 元素个数最多 100000。
- `int` 范围内任意值，包括负数。
- `pop` 和 `top` 只在堆非空时调用。
- 评测会用固定种子生成随机操作序列，并与多重集合的结果比较。
