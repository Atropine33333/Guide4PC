这一题不读 stdin，也不写 stdout。用 `shared_ptr` 和 `weak_ptr` 搭一条双向可回看的链，确保不会形成所有权环，测试在隐藏的 harness 里。

```cpp
#include <memory>

struct Node {
    int value = 0;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> previous;
};

std::shared_ptr<Node> build_chain(int length);
std::size_t chain_length(const std::shared_ptr<Node>& head);
bool links_consistent(const std::shared_ptr<Node>& head);
```

规则：

- `build_chain(length)` 创建一条长度恰好为 `length` 的链，节点的 `value` 依次为 `0, 1, ..., length - 1`。
- 每个节点的 `next` 指向后一个节点，最后一个节点的 `next` 为空。
- 每个节点的 `previous` 指向前一个节点，第一个节点的 `previous` 为空；`previous` 必须是弱引用，不能增加所有权。
- `length <= 0` 时返回空指针。
- `chain_length(head)` 沿 `next` 数节点个数；空指针返回 0。
- `links_consistent(head)` 检查双向链接是否一致：每个 `next` 非空的节点，其后继的 `previous.lock()` 必须正好指回它；头节点的 `previous` 必须已经失效。空指针视为一致。
- 不要写 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `build_chain(0)` | 空指针 |
| `chain_length(build_chain(5))` | `5` |
| `links_consistent(build_chain(5))` | `true` |

### 提示

构造时用 `node->previous = current;` 把 `shared_ptr` 赋给 `weak_ptr`。链尾的 `previous` 是弱引用，所以整条链只由头部的 `shared_ptr` 拥有；头指针释放后，链尾的 `previous.lock()` 会返回空。`links_consistent` 比较地址时用 `.get()`。
