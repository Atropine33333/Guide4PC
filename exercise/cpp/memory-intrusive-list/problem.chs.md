这一题不读 stdin。实现一个**不拥有**结点的双向链表，结点由调用方分配。

```cpp
#include <cstddef>

struct Node {
    int value;
    Node* prev;
    Node* next;
};

class IntrusiveList {
public:
    IntrusiveList();
    ~IntrusiveList();
    void push_back(Node* node);
    void unlink(Node* node);
    Node* head() const;
    Node* tail() const;
    std::size_t size() const;
    bool empty() const;
};
```

约定：

- 链表只维护结点之间的 `prev` / `next` 链接，**不**分配也不释放结点，析构函数不能 `delete` 任何结点。
- `push_back` 把结点接到表尾；调用方保证 `node` 非空且当前不属于任何链表。
- `unlink` 把结点从链表摘除，并把它的 `prev` 和 `next` 置为 `nullptr`；如果结点不在本链表中（或为 `nullptr`），什么也不做。
- `head` / `tail` 返回首尾结点，空表返回 `nullptr`。
- `empty` 与 `size` 描述当前结点数。
- 不要写 `main`。

harness 会把结点放在一个已经预留好容量的 `std::vector<Node>` 中，结点的地址在测试期间保持稳定。
