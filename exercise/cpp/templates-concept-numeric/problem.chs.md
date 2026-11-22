这一题不读 stdin，也不写 stdout。用一个 C++20 concept 限制模板参数，并实现平均值函数，测试在隐藏的 harness 里。

```cpp
#include <concepts>
#include <type_traits>
#include <vector>

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template <Numeric T>
T average_of(const std::vector<T>& values, T fallback);
```

规则：

- `Numeric` 接受所有算术类型，但排除 `bool`；`Numeric<int>`、`Numeric<double>`、`Numeric<unsigned>` 为真，`Numeric<bool>`、`Numeric<std::string>` 为假。
- `average_of` 在序列为空时返回 `fallback`。
- 非空时返回所有元素的平均值，整个计算都用 `T` 类型：整数除法向零截断，例如 `{1, 2}` 的平均值是 `1`，`{-1, -2}` 是 `-1`。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `average_of<int>({1, 2}, 0)` | `1` |
| `average_of<int>({}, 42)` | `42` |
| `average_of<double>({1.5, 2.5}, 0.0)` | `2.0` |
| `average_of<unsigned>({1u, 2u}, 0u)` | `1u` |

### 提示

用 `std::is_arithmetic_v` 判断算术类型，再用 `std::is_same_v<T, bool>` 排除 `bool`。求和变量和元素个数都用 `T` 参与运算；`static_cast<T>(values.size())` 把长度转成 `T`。concept 可以直接写在 `template` 的参数列表里：`template <Numeric T>`。
