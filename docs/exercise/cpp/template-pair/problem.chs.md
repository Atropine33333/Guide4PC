这一题不读 stdin，也不写 stdout。实现一个泛型数对类模板，测试在隐藏的 harness 里。

实现类模板 `Pair<T>`：

- `Pair()`：两个元素值初始化。
- `Pair(const T& first, const T& second)`：按给定值构造。
- `T& first()` / `const T& first() const`：访问第一个元素，两个重载都要有。
- `T& second()` / `const T& second() const`：访问第二个元素，两个重载都要有。
- `Pair<T> swapped() const`：返回一个新数对，两个元素交换位置，原数对不变。
- `bool operator==(const Pair<T>& other) const`：两个元素都相等时返回 `true`。

harness 会用 `int`、`std::string` 和 `Pair<int>` 作为 `T` 实例化这个模板；`const` 对象也必须能读取元素。
