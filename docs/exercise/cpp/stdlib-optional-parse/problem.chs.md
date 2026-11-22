《整数解析法典》

第一条（适用范围）本法典规定如何将一段文本解析为 `int`。解析失败时不得抛出异常，应当返回 `std::nullopt`。

第二条（合法写法）合法文本由可选的一个 `-` 号与至少一位十进制数字组成，此外不得有任何字符。

第三条（禁止事项）`+` 号、空格、小数点、字母均不合法；空串不合法；单独的 `-` 也不合法。

第四条（前导零）允许前导零，`"000123"` 解析为 123；`"-0"` 合法，其值为 0。

第五条（范围）解析结果必须落在 `-2147483648` 到 `2147483647` 之间。超出范围时返回 `std::nullopt`，不得依靠溢出回绕。

第六条（`parse_or`）`parse_or` 解析成功时返回解析结果，失败时返回调用者给出的 `fallback`。

第七条（附则）实现下列两个函数，不得编写 `main`。本法典未规定之处，以 `std::optional<int>` 的语义为准。

```cpp
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text);
int parse_or(const std::string& text, int fallback);
```

**人话**：实现 `parse_int` 与 `parse_or`。`parse_int` 只在文本完全符合“可选负号加至少一位数字”且结果落在 `int` 范围内时返回整数，否则返回 `std::nullopt`，全程不抛异常；`parse_or` 解析成功时返回结果，失败时返回 `fallback`。逐字符检查比 `std::stoi` 更合适，后者会抛异常；可以先用 `long long` 累加数字，最后判断是否越界。

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
