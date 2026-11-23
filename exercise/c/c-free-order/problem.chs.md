（街角旧货摊，喇叭循环播放。）

收摊了！本摊今日只办一件事：把白天挂上去的招牌，一件一件摘下来。

招牌是这么挂的：新来的挂最前头，后到的压着先到的。

收摊也讲个次序：最后挂上的先摘，摘一件先看清下一件在哪儿，再摘下一件。八点整收完，一件不剩。

**人话**：用 `node_alloc` 借节点、把头插进链表；`list_free_all` 从表头开始，先把下一节的地址记下来，再 `node_free` 当前节点，做到每节恰好归还一次。

**接口**：

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *node_alloc(void);   /* 题目提供：借一节节点，借不到返回 NULL */
void node_free(Node *p);  /* 题目提供：归还一节节点 */

Node *list_push_front(Node *head, int value);
size_t list_length(const Node *head);
size_t list_to_array(const Node *head, int *out, size_t cap);
void list_free_all(Node *head);
```

- `list_push_front` 借一节节点，填入 `value`，接到表头前面并返回新的表头；借不到节点时原样返回 `head`。
- `list_length` 返回节点个数，空表返回 `0`。
- `list_to_array` 从表头开始最多写 `cap` 个值到 `out`，返回实际写入的个数；`head` 为 `NULL` 时返回 `0`。
- `list_free_all` 从表头开始逐节归还；`head` 为 `NULL` 时什么也不做。

### 示例

```c
Node *head = NULL;
head = list_push_front(head, 1);   /* 1 */
head = list_push_front(head, 2);   /* 2 -> 1 */
head = list_push_front(head, 3);   /* 3 -> 2 -> 1 */
int out[3];
list_to_array(head, out, 3);       /* out 为 3, 2, 1 */
list_free_all(head);               /* 先 3，再 2，最后 1 */
```

### 约定

- 只需要实现四个链表函数，不要写 `main`。
- 节点内存只通过 `node_alloc` 与 `node_free` 取得和归还，不要直接调用 `malloc`、`free`。
- 归还之前先把下一节的地址存好；已经归还的节点不再访问。
- 一次收摊把链表上原有的每一节都还掉，每节只还一次。
