一只柜子有 16 个格子，编号 `0` 到 `15`。来一个整数，先算 `(unsigned)key % 16` 落到对应格子；格子被占了就往后挪一格，挪到底再回到 `0`。取值、删除也照这条路走。删掉的格子要留个「已删」的记号，好让排在它后面的元素仍然找得到。

**接口**：

```c
#include <stddef.h>

#define HASH_CAP 16

typedef struct {
    int keys[HASH_CAP];
    unsigned char state[HASH_CAP]; /* 0 空，1 占用，2 已删 */
    size_t count;
} HashSet;

void hash_init(HashSet *s);
int hash_set(HashSet *s, int key);
int hash_get(const HashSet *s, int key);
int hash_remove(HashSet *s, int key);
size_t hash_size(const HashSet *s);
```

- `hash_init` 把所有格子清空，计数归零。
- `hash_set` 从 `(unsigned)key % HASH_CAP` 起线性探测：键已存在就返回 `0` 且不重复占位；遇到空位或已删位就放进去并返回 `0`；16 个格子都试过仍无位置返回 `-1`。
- `hash_get` 找到返回 `1`，碰到空位或走完一圈返回 `0`。
- `hash_remove` 找到就把该格标为已删并返回 `0`；找不到返回 `-1`。删除后 `hash_size` 减一。
- `hash_size` 返回当前元素个数。负数键也按下标公式处理，`(unsigned)` 转换即可。

### 示例

依次放入 `1`、`17`、`-15`：三个键都从 1 号格开始，分别落在 1、2、3 号格。此时 `hash_get(17)` 为 `1`，`hash_size` 为 `3`。删掉 `17` 后，`hash_get(1)` 与 `hash_get(-15)` 仍为 `1`，`hash_get(17)` 为 `0`；再放入 `49`，它会占用刚才空出来的位置。

### 约定

- 只实现这些函数，不要写 `main`。
- 表长固定为 `HASH_CAP`，不要动态扩容。
- 槽位公式固定为 `(unsigned)key % HASH_CAP`，从该槽位开始逐个线性探测。
- 结构体保持题面给出的字段，函数签名不变。
