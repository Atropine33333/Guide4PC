话说这一日，演武场上摆开一队人马，唤作 `IntrusiveList`。列位看官，这队伍有个古怪脾气：只认人，不养人。

怎么讲？结点 `Node` 是调用方自备的，链表只管把 `prev`、`next` 两根搭膊一系，让诸位前后认得。至于吃住工钱，一概不问；到了散场，也不许把人家 `delete` 了去。

开场先点卯：`push_back` 把新人接到队尾，来者须是活人，且身上没系过别的绳子。`unlink` 把某人从队里摘出，顺手把他的 `prev`、`next` 抹成 `nullptr`；若此人本不在本队，或者压根没来，就当无事发生。

`head`、`tail` 报队首队尾，空队报 `nullptr`；`empty`、`size` 报人数。散场时分，本队只散不解，不动刀兵。

有人问：这结点搁在何处，可会挪窝？看官放心，harness 早把结点请进一间预留了容量的 `std::vector<Node>`，地址稳如泰山，只等列位系绳。

**人话**：实现 `IntrusiveList`：只维护结点间的 `prev` / `next`，不分配也不释放结点，析构时不能 `delete` 任何结点。`push_back` 接在表尾，调用方保证结点非空且当前不属于任何链表；`unlink` 摘除结点并把它的 `prev`、`next` 置为 `nullptr`，结点不在本表或为 `nullptr` 时什么也不做。`head` / `tail` 空表返回 `nullptr`，`size` / `empty` 描述结点数。本题不读 stdin；不写 `main`。

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
