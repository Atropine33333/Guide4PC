这一题不读 stdin，也不写 stdout。实现对指针和长度做对齐运算的三个函数，测试在隐藏的 harness 里。

```cpp
#include <cstddef>

std::size_t align_up(std::size_t value, std::size_t alignment);
bool is_aligned(const void* pointer, std::size_t alignment);
std::size_t padding_for(std::size_t used, std::size_t alignment);
```

规则：

- `alignment` 是 2 的幂（1、2、4、8……）时按对齐语义计算。
- `align_up(value, alignment)` 返回不小于 `value` 的最小对齐值。
- `padding_for(used, alignment)` 返回从 `used` 补齐到对齐需要加上的字节数，即 `align_up(used, alignment) - used`。
- `is_aligned(pointer, alignment)` 判断指针地址是否是 `alignment` 的整数倍。
- 特殊情况：
  - `alignment == 0` 时，`align_up` 原样返回 `value`，`padding_for` 返回 0，`is_aligned` 返回 `false`。
  - `is_aligned(nullptr, alignment)` 在 `alignment` 是正数且为 2 的幂时返回 `true`。
  - `alignment` 不是 2 的幂时，`is_aligned` 返回 `false`；`align_up` 和 `padding_for` 只保证不崩溃（可以按位掩码公式计算）。
  - `align_up` 的结果超过 `std::size_t` 上限时返回 `SIZE_MAX`，不要回绕成小数。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `align_up(5, 8)` | `8` |
| `align_up(8, 8)` | `8` |
| `padding_for(9, 8)` | `7` |
| `is_aligned(nullptr, 8)` | `true` |
| `is_aligned(buffer + 1, 16)` | `false`（`buffer` 按 16 对齐时） |

### 提示

判断 2 的幂：`alignment != 0 && (alignment & (alignment - 1)) == 0`。指针地址转成 `std::uintptr_t` 再取模。`align_up` 先判断 `value > SIZE_MAX - (alignment - 1)`，再算 `(value + alignment - 1) & ~(alignment - 1)`。
