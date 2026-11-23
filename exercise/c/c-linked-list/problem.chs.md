一条单链表从 `head` 出发，每个节点带一个 `int`，节点之间用指针串起来，最后一个节点的 `next` 是 `NULL`。你来管这条链：从前面塞、从前面取、报长度、判空，最后整条拆掉。

**接口**：

```c
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} IntList;

void list_init(IntList *l);
int list_push_front(IntList *l, int value);
int list_pop_front(IntList *l, int *out);
size_t list_size(const IntList *l);
int list_is_empty(const IntList *l);
void list_free(IntList *l);
```

- `list_init` 把 `head` 置为 `NULL`，`size` 置为 `0`。
- `list_push_front` 用 `malloc` 造一个新节点挂到最前面；成功返回 `0`，分配失败返回 `-1`，链表保持原样。
- `list_pop_front` 摘下最前面的节点，把它的值写进 `*out`，`free` 掉这个节点，返回 `0`；空链表返回 `-1`，`*out` 不动。
- `list_size` 返回节点个数；`list_is_empty` 空表返回 `1`，否则返回 `0`。
- `list_free` 释放全部节点，并把 `head` 置回 `NULL`、`size` 置回 `0`，之后这条链还能接着用。

### 示例

依次 `list_push_front` 压入 `1`、`2`、`3`，链上从前往后是 `3 2 1`；弹一次得到 `3`，再弹一次得到 `2`，此时 `list_size` 为 `1`。调用 `list_free` 之后 `list_size` 回到 `0`，`list_is_empty` 返回 `1`。

### 约定

- 只实现这些函数，不要写 `main`。
- 节点由 `malloc` 分配，长度不受固定上限约束；空链表上的操作不能崩溃。
- 结构体保持题面给出的字段，函数签名不变。
