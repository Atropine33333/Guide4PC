这一题不读 stdin，也不写 stdout。实现一个可变参数函数模板，测试在隐藏的 harness 里。

```cpp
template <typename... Args>
long long sum_all(Args... args);
```

规则：

- 把所有实参按 64 位有符号整数相加并返回。
- 不传参数时返回 0。
- 实参可以是 `char`、`short`、`int`、`long`、`long long` 等整型，也可能混合出现。
- `char` 按它的数值参与相加（例如 `'a'` 是 97），不要当作文本。
- 结果可能为负；不要用 `int` 作为累加类型。
- 不要写 `main`。

### 提示

C++17 的折叠表达式最方便：`return (0LL + ... + static_cast<long long>(args));`。空参数包时这个折叠的结果是 0。
