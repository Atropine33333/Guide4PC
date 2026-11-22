这一题不读 stdin，也不写 stdout。请在 `gadget` 命名空间中实现一个定制的 `swap`，并观察实参依赖查找（ADL）如何影响非限定调用。

```cpp
namespace gadget {

struct Widget {
    int id;
    int value;
};

int swap_calls();
void reset_swap_calls();

// 定制交换：交换两个 Widget，并让 swap_calls() 增加 1。
void swap(Widget& a, Widget& b);

}  // namespace gadget
```

规则：

- `gadget::swap(Widget&, Widget&)` 交换两个成员的 `id` 与 `value`，每调用一次，`swap_calls()` 就加 1。
- `reset_swap_calls()` 把计数清零。
- 计数是 `gadget` 命名空间内部的全局状态，初始为 0。
- 不要写 `main`。harness 会分别用以下方式调用：
  - 在函数内写 `using std::swap; swap(x, y);`，其中 `x`、`y` 是 `gadget::Widget`。实参类型让 ADL 找到 `gadget::swap`，定制版本应当被选中。
  - 写 `std::swap(x, y)`。这是限定调用，不会触发 ADL，因此不会调用 `gadget::swap`，计数器不应改变；两个 Widget 仍然会通过移动被交换。
  - 对两个 `int` 写 `using std::swap; swap(a, b);`。`std::swap` 应当被选中，`gadget` 的计数器保持不变。
- 不要用宏或特化 `std::swap` 来绕过问题；定制的 `swap` 必须是一个普通函数。
- 注意：`gadget::swap` 内部交换 `int` 成员时，可以调用 `std::swap`，这不会递归。
