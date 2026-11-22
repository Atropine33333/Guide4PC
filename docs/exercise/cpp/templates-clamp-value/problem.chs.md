这一题不读 stdin，也不写 stdout。函数模板的定义会被隐藏的 harness 直接包含，请把它写成一个头文件式的实现。

实现：

```cpp
template <typename T>
T clamp_value(const T& value, const T& low, const T& high);
```

返回：

- 若 `value < low`，返回 `low`；
- 若 `high < value`，返回 `high`；
- 否则返回 `value`。

调用方保证 `low <= high`。`T` 至少会被实例化为 `int`、`double` 和 `char`。不要写 `main`。

要求：

- 只使用 `<` 比较，不要要求 `T` 支持算术运算；
- 当 `low == high` 时，区间内只剩一个值，结果必须是这个值；
- 返回值类型是 `T`（按值返回）。
