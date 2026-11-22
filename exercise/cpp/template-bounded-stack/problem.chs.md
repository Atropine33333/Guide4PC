Guide4PC 练习题管理办公室

编号：G4-TPL-001

关于加强定容泛型栈管理的通知

各测试单元：

为规范定容栈的压入、弹出与容量核对工作，现就有关事项通知如下。

一、栈体容量由模板参数 `Capacity` 在编译期核定。一经核定，不得扩容，不得改建。

二、压栈须先核对余量。未满者准予压入，返回 `true`；已满者一律拒收，返回 `false`，栈内内容保持原样。

三、弹出与读取栈顶须先核对存量。非空者办理，返回 `true`；空栈不予办理，返回 `false`，`out` 不写入。

四、存量与容量分别由 `size()` 与 `capacity()` 如实申报，不得虚报。

五、承办本题者须保证元素类型可默认构造、可拷贝构造并可赋值。压入的是元素，退还的也是元素。

请照此执行。

**人话**：实现类模板 `BoundedStack<T, Capacity>`（`Capacity` 是正整数），成员与行为如下；容量与空栈的处理按上面五条办理。

- `bool push(const T& value)`：未满时压栈并返回 `true`；已满返回 `false`，内容不变。
- `bool pop()`：非空时弹出栈顶并返回 `true`；空栈返回 `false`。
- `bool top(T& out) const`：非空时把栈顶写入 `out` 并返回 `true`；空栈返回 `false`。
- `int size() const`：返回元素个数。
- `static constexpr int capacity()`：返回模板参数 `Capacity`。

该模板会被实例化为 `BoundedStack<int, 3>`、`BoundedStack<std::string, 2>`、`BoundedStack<double, 1>` 等；元素类型需要可默认构造、可拷贝构造并可赋值。
