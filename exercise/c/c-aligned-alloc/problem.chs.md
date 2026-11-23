《对齐的格子》

占卜师铺开一张桌布，横八格，竖八格，一共六十四格。牌要摆得正：每张牌的起边都得压在格线上，偏一格就不作数。

问卜的人报几件法器，占卜师就从布上裁一块够放的地方，裁出来的起边一定落在格线上。裁完在布角记一笔，收摊时照着这一笔把布原样还回去。

要是有人一件也不要，占卜师摇摇头，手里什么也不拿。

**人话**：实现 `alloc_aligned64(n)`，申请至少 `n` 字节，返回的地址是 64 的整数倍；`free_aligned64` 负责把这块地还回去。`n` 为 0 时返回 `NULL`。

**接口**：

```c
#include <stddef.h>
#include <stdint.h>

void *alloc_aligned64(size_t n);
void free_aligned64(void *p);
```

- `n > 0`：返回非 `NULL` 指针，`(uintptr_t)返回值 % 64 == 0`，从该地址起 `n` 字节可读可写；申请失败返回 `NULL`。
- `n == 0`：返回 `NULL`。
- `free_aligned64` 只接受 `alloc_aligned64` 返回的指针；`free_aligned64(NULL)` 什么也不做。
- 两次申请的块互不重叠。
- 返回的地址可以与 `malloc` 给的原始地址不同，归还时由 `free_aligned64` 找回最初那块。

### 示例

```c
#include <string.h>

void *p = alloc_aligned64(100);   /* p 是 64 的整数倍 */
memset(p, 0, 100);
free_aligned64(p);
```

### 约定

- 只实现这两个函数，不要写 `main`。
- 只用标准库，自己用 `malloc` 完成对齐；不要依赖运行库里的对齐分配函数。
- 对齐要求固定为 64。
- `malloc` 返回的地址碰巧对齐不算数，换一台机器、换一次运行都要成立。
- 归还之后不再使用该指针，每个指针只归还一次。
