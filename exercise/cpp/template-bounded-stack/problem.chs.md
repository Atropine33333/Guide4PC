这一题不读 stdin，也不写 stdout。实现一个容量在编译期确定的泛型栈，测试在隐藏的 harness 里。

实现类模板 `BoundedStack<T, Capacity>`，其中 `Capacity` 是正整数：

- `bool push(const T& value)`：未满时压栈并返回 `true`；已满返回 `false`，内容不变。
- `bool pop()`：非空时弹出栈顶并返回 `true`；空栈返回 `false`。
- `bool top(T& out) const`：非空时把栈顶写入 `out` 并返回 `true`；空栈返回 `false`。
- `int size() const`：返回元素个数。
- `static constexpr int capacity()`：返回模板参数 `Capacity`。

harness 会用 `BoundedStack<int, 3>`、`BoundedStack<std::string, 2>`、`BoundedStack<double, 1>` 等实例测试；元素类型需要可默认构造、可拷贝构造并可赋值。
