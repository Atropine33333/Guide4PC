《出埃及记》第十九章

耶和华晓谕摩西说：你晓谕以色列全会众，说，凡呼一名，不可只在本家寻找，也要往实参所出的族里去寻；在那里寻见的，便是那一族的律例。这律例叫作实参依赖查找。

耶和华又晓谕摩西说：你要在 `gadget` 族中设立一个 `swap`，凡其族人两两相易，须数点一人，记在册上；`reset_swap_calls` 拂去册上的数目。凡连名带姓呼 `std::swap` 的，便绕过了族中的律例；至于外邦的 `int`，仍归 `std::swap` 管辖。

于是摩西在 `gadget` 中设立了 `swap`，并数点其中人数，正如耶和华所吩咐的。

**人话**：在 `gadget` 命名空间里实现定制的 `swap(Widget&, Widget&)`：交换两个成员的 `id` 与 `value`，每调用一次就让 `swap_calls()` 加一；再实现 `swap_calls()` 与 `reset_swap_calls()`，计数器初始为 0。三种调用方式要分别成立：非限定调用应当选中定制版本，`std::swap` 应当绕过它，两个 `int` 之间仍由 `std::swap` 处理。

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

### 规则

- `gadget::swap(Widget&, Widget&)` 交换两个成员的 `id` 与 `value`，每调用一次，`swap_calls()` 就加 1。
- `reset_swap_calls()` 把计数清零。
- 计数是 `gadget` 命名空间内部的全局状态，初始为 0。
- 不要写 `main`。
- 三种调用方式要分别成立：
  - 在函数内写 `using std::swap; swap(x, y);`，其中 `x`、`y` 是 `gadget::Widget`。实参类型让 ADL 找到 `gadget::swap`，定制版本应当被选中。
  - 写 `std::swap(x, y)`。这是限定调用，不会触发 ADL，因此不会调用 `gadget::swap`，计数器不应改变；两个 Widget 仍然会通过移动被交换。
  - 对两个 `int` 写 `using std::swap; swap(a, b);`。`std::swap` 应当被选中，`gadget` 的计数器保持不变。
- 不要用宏或特化 `std::swap` 来绕过问题；定制的 `swap` 必须是一个普通函数。
- 注意：`gadget::swap` 内部交换 `int` 成员时，可以调用 `std::swap`，这不会递归。
