四个颜色通道被压进一个 32 位无符号整数，要实现打包与拆包。

```cpp
#include <cstdint>

std::uint32_t pack_rgba(int r, int g, int b, int a);
void unpack_rgba(std::uint32_t packed, int& r, int& g, int& b, int& a);
std::uint32_t swap_rb(std::uint32_t packed);
```

布局（从高位到低位）：`a` 占第 24–31 位，`r` 占 16–23 位，`g` 占 8–15 位，`b` 占 0–7 位。也就是：

```
a << 24 | r << 16 | g << 8 | b
```

要求：

- `pack_rgba` 先把每个通道钳制到 `0`–`255`（小于 0 当作 0，大于 255 当作 255），再按上面的布局打包。
- `unpack_rgba` 从 `packed` 中取出四个通道，写入四个引用参数；每个值都在 `0`–`255` 之间。
- `swap_rb` 交换红色和蓝色两个字节，绿色和透明通道保持不变。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `pack_rgba(1, 2, 3, 4)` | `0x04010203` |
| `pack_rgba(255, 0, 128, 16)` | `0x10FF0080` |
| `pack_rgba(-5, 300, 0, 999)` | `0xFF00FF00` |
| `swap_rb(0x11223344)` | `0x11443322` |

### 提示

用 `std::uint32_t` 做移位，不要用有符号 `int` 左移可能溢出的写法；拆包时逐字节与 `0xFF` 相与。
