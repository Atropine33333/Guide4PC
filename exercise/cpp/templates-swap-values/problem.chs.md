这一题不读 stdin，也不写 stdout。实现两个函数模板，测试在隐藏的 harness 里。

```cpp
template <typename T>
void swap_values(T& first, T& second);

template <typename T>
T min_value(T first, T second);
```

规则：

- `swap_values` 交换两个同类型对象的值；允许两个引用绑定同一个对象，此时什么也不改变。
- `min_value` 返回两个同类型值中较小的那个；相等时返回哪一个都可以。
- 两个函数都必须对 `int`、`double`、`char` 和 `std::string` 等类型工作，只能使用 `<` 比较。
- 不要使用 `std::swap`，自己用临时变量实现交换。
- 不要写 `main`。

### 提示

模板参数 `T` 会从实参推导出来；两个参数必须是同一类型。字符串的 `<` 是字典序比较。
