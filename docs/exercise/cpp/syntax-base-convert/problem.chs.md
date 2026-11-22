这一题不读 stdin，也不写 stdout。实现两个在任意进制（2 到 36）之间转换整数的函数，测试在隐藏的 harness 里。

```cpp
#include <string>

std::string to_base(long long value, int base);
bool parse_base(const std::string& text, int base, long long& out);
```

规则：

- 进制范围是 `2 ≤ base ≤ 36`；越界时 `to_base` 返回空串，`parse_base` 返回 `false`。
- `to_base` 把 `value` 写成该进制：大于 9 的数字依次用大写 `A`–`Z` 表示，`0` 写作 `"0"`，负数在前面加一个 `-`。
- `to_base` 必须能处理 `LLONG_MIN`（`-9223372036854775808`），不能直接对它取相反数。
- `parse_base` 接受可选的 `+` 或 `-` 号，后面至少一位数字；字母大小写都接受。
- `text` 中出现非法字符、没有数字、或数值超出 `long long` 范围时返回 `false`，并且不修改 `out`。
- `parse_base` 成功时把结果写入 `out` 并返回 `true`。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `to_base(255, 16)` | `"FF"` |
| `to_base(-5, 2)` | `"-101"` |
| `to_base(0, 36)` | `"0"` |
| `parse_base("101", 2, out)` | `true`，`out == 5` |
| `parse_base("-ff", 16, out)` | `true`，`out == -255` |
| `parse_base("12", 2, out)` | `false` |

### 提示

先把负数转成 `unsigned long long` 的绝对值再逐位取余，最后统一加负号；解析时用 `unsigned long long` 累加，并在乘之前检查是否会超过 `LLONG_MAX`（负数时的上限是 `1ULL << 63`）。
