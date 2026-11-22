这一题不读 stdin，也不写 stdout。实现一个不会抛异常的整数解析函数，测试在隐藏的 harness 里。

```cpp
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text);
int parse_or(const std::string& text, int fallback);
```

规则：

- 合法写法：可选的一个 `-` 号，后面至少一位数字，除此之外不能有任何字符。
- 不允许 `+` 号、空格、小数点、字母；不允许空串，单独的 `-` 也不合法。
- 允许前导零（如 `"000123"`），`"-0"` 合法且等于 0。
- 结果必须落在 `int` 范围内（`-2147483648` 到 `2147483647`）；超出范围返回 `std::nullopt`，不要靠溢出回绕。
- `parse_or` 解析成功时返回解析结果，否则返回 `fallback`。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `parse_int("42")` | `42` |
| `parse_int("-0")` | `0` |
| `parse_int("4x")` | `std::nullopt` |
| `parse_int("2147483648")` | `std::nullopt` |
| `parse_or("abc", -1)` | `-1` |

### 提示

逐字符检查比 `std::stoi` 更合适：`std::stoi` 会抛异常，而本题不允许异常。可以先用 `long long` 累加数字，最后再判断是否越界。
