这一题不读 stdin。实现三个处理单个字节的函数，隐藏的 harness 会调用它们。

```cpp
#include <string>

enum class ByteKind { Control, Whitespace, Digit, Printable, High };

ByteKind classify_byte(unsigned char byte);
int to_lower(int ch);
std::string hex_byte(unsigned char byte);
```

`classify_byte` 的分类规则（按顺序判断）：

| 类别 | 字节值 |
| --- | --- |
| `Whitespace` | `9, 10, 11, 12, 13, 32` |
| `Control` | `0`–`8`、`14`–`31`、`127` |
| `Digit` | `'0'`–`'9'` |
| `High` | `128`–`255` |
| `Printable` | 其余（`33`–`126` 中除去数字的部分） |

其余要求：

- `to_lower`：`'A'`–`'Z'` 返回对应的小写字母，其他整数（包括负数）原样返回。
- `hex_byte`：返回恰好两个大写十六进制数字，例如 `0` → `"00"`、`255` → `"FF"`、`15` → `"0F"`。注意 `char` 在有的平台上是有符号的，不要把 `0xFF` 扩展成 8 位十六进制。
- 不要写 `main`。

### 提示

形参用 `unsigned char` 可以保证进入函数时值在 `0`–`255`；用 `static_cast<unsigned char>(c)` 后再取高四位和低四位。分类时先判断空白和数字这些较窄的区间。
