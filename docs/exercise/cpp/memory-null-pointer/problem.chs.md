这一题不读 stdin，也不写 stdout。实现三个接收“指针 + 长度”的函数，它们必须在空指针或非正长度时安全返回，测试在隐藏的 harness 里。

```cpp
long long sum_or_zero(const int* data, int count);
int max_or(const int* data, int count, int fallback);
int count_even(const int* data, int count);
```

规则：

- `data == nullptr` 或 `count <= 0` 时，三个函数分别返回 `0`、`fallback`、`0`，且绝对不能访问 `data`。
- `sum_or_zero` 返回所有元素之和，结果是 `long long`。
- `max_or` 返回最大元素；空输入返回 `fallback`。
- `count_even` 返回偶数个数；负偶数也算偶数（`-4` 是偶数，`-3` 不是）。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `sum_or_zero(nullptr, 5)` | `0` |
| `max_or(nullptr, 5, -7)` | `-7` |
| `sum_or_zero(values, 3)`，`values = {2, -4, 7}` | `5` |
| `count_even(values, 3)`，`values = {2, -4, 7}` | `2` |

### 提示

把守卫写成 `if (data == nullptr || count <= 0) return ...;`，再进入循环。即使调用方传入 `(nullptr, 0)`，也不要把空指针送进循环体或 `std::max_element`。
