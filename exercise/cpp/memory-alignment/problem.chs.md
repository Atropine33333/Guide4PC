欢迎来到对齐营地。

营地里的每条规则都是真的，只是常识在这里不落地。

规则一：界线只有 1、2、4、8 这些级数。`alignment` 为 2 的幂时，一切按界线办事。

规则二：`align_up` 把人送到不小于 `value` 的第一道界线；`padding_for` 报告还差几级台阶，数目恰为 `align_up(used, alignment) - used`。

规则三：`is_aligned` 只看门牌号能否被 `alignment` 整除；门牌号得先转成 `std::uintptr_t`。

规则四：界线为零时营地不设界线：`align_up` 原样放人，`padding_for` 报 0，`is_aligned` 一律答 `false`。

规则五：空地址站在零号门牌上；`alignment` 为正的 2 的幂时，`is_aligned(nullptr, alignment)` 答 `true`。

规则六：`alignment` 非 2 的幂时，`is_aligned` 答 `false`，另外两个只要不炸即合规矩。

规则七：有人跨到 `std::size_t` 尽头之外，营地不发回绕票，只给 `SIZE_MAX`。

规则八：营地不开正门。别写 `main`。

**人话**：实现三个函数：`align_up` 返回不小于 `value` 的最小对齐值，`padding_for` 返回补齐字节数，`is_aligned` 判断地址是否为 `alignment` 的整数倍。`alignment == 0` 时分别返回 `value`、`0`、`false`；`is_aligned(nullptr, 正的 2 的幂)` 返回 `true`；非 2 的幂时 `is_aligned` 返回 `false`，另外两个只需不崩溃；超出 `std::size_t` 上限返回 `SIZE_MAX`。

```cpp
#include <cstddef>

std::size_t align_up(std::size_t value, std::size_t alignment);
bool is_aligned(const void* pointer, std::size_t alignment);
std::size_t padding_for(std::size_t used, std::size_t alignment);
```

### 示例

| 调用 | 结果 |
| --- | --- |
| `align_up(5, 8)` | `8` |
| `align_up(8, 8)` | `8` |
| `padding_for(9, 8)` | `7` |
| `is_aligned(nullptr, 8)` | `true` |
| `is_aligned(buffer + 1, 16)` | `false`（`buffer` 按 16 对齐时） |

### 提示

2 的幂用 `alignment != 0 && (alignment & (alignment - 1)) == 0` 判断。`align_up` 先查 `value > SIZE_MAX - (alignment - 1)`，再算 `(value + alignment - 1) & ~(alignment - 1)`。
