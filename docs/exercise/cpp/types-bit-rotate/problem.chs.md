实现 32 位无符号整数的循环移位和位反转。

```cpp
#include <cstdint>

std::uint32_t rotate_left(std::uint32_t value, int count);
std::uint32_t rotate_right(std::uint32_t value, int count);
std::uint32_t reverse_bits(std::uint32_t value);
```

要求：

- `rotate_left` 把高位移出的位补到低位；`rotate_right` 方向相反。
- `count` 可以是任意整数：先对 32 取模，负数按相反方向旋转。例如左移 33 位等价于左移 1 位，左移 `-1` 位等价于右移 1 位。
- 取模后为 0 时返回原值；不能出现按 32 位或负数移位的未定义行为。
- `reverse_bits` 把第 0 位与第 31 位对调、第 1 位与第 30 位对调，依此类推。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `rotate_left(0x80000001u, 1)` | `0x00000003u` |
| `rotate_left(0x12345678u, 8)` | `0x34567812u` |
| `rotate_right(0x12345678u, 8)` | `0x78123456u` |
| `rotate_right(1u, 1)` | `0x80000000u` |
| `reverse_bits(0x12345678u)` | `0x1E6A2C48u` |
| `reverse_bits(0x0000FFFFu)` | `0xFFFF0000u` |

### 提示

先把 `count` 规范到 `[0, 32)`：`int n = count % 32; if (n < 0) n += 32;`。当 `n == 0` 时单独返回原值，避免写 `value >> 32`。
