数值类型管理法典

第一条 本法所称“数值类型”，指算术类型，`bool` 除外。`int`、`double`、`unsigned` 在其列；`bool`、`std::string` 不在此列。

第二条 凡接受数值类型之模板，其参数以 `Numeric` 约束，不得以运行期检查替代。

第三条 平均值之计算，自始至终在本类型 `T` 内完成。元素之和记于 `T`，元素个数亦转成 `T`。

第四条 序列为空者，返回调用者所给之 `fallback`。

第五条 非空者，返回全部元素之平均值。整数除法向零截断：`{1, 2}` 之平均为 `1`，`{-1, -2}` 之平均为 `-1`。

第六条 本法自模板实例化之时施行。

**人话**：定义 concept `Numeric`（算术类型，排除 `bool`），再实现 `average_of`：空序列返回 `fallback`，非空时整个计算都用 `T`，整数除法向零截断。

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
