这一题不读 stdin，也不写 stdout。实现两个接收数组引用的函数模板，测试在隐藏的 harness 里。

```cpp
template <typename T, int N>
double average(const T (&values)[N]);

template <typename T, int N>
T max_value(const T (&values)[N]);
```

规则：

- 两个模板都从数组引用推导 `T` 和 `N`，`N >= 1`。
- `average` 返回所有元素的算术平均值，类型是 `double`；即使 `T` 是整数，也不能用整数除法截断。
- `max_value` 返回最大的元素，返回类型是 `T`；用 `<` 比较，相等时返回任意一个。
- `int`、`double`、`long long`、`char` 数组必须同时支持 `average` 和 `max_value`；`std::string` 数组只需支持 `max_value`。
- 不要写 `main`。

### 提示

遍历数组时把元素累加到 `double` 变量中，再除以 `N`。`char` 的和不要存回 `char`，否则会溢出。`std::string` 数组只测试 `max_value`，不会实例化 `average`。
