《出埃及记·链》

耶和华晓谕摩西说：

“你要为百姓造一条链，每节向前，只许向后望一眼。向后望的，算作看见，不算作拥有。”

摩西问：“若有人向后望而伸手抓住呢？”

耶和华说：“不可。向后伸手，链便成环，环上的都不肯放手，末后一节也必不熄灭。你要使后望之绳为弱绳：望得见，拿不走。”

“链有几节由 `length` 定，节上刻数自 `0` 起，依次加一。头一节向后望，望的是空；末一节向前望，望的也是空。”

“数节的人顺向前的绳走，走一节数一节，到空处便止。查链的人要看每节向后望时是否望见前一节，头一节向后望时必须已经看不见。”

“出营的时候，不可为自己造 `main`。”

**人话**：实现三个函数。`build_chain(length)` 造一条长度恰为 `length` 的双向链，节点 `value` 依次为 `0` 到 `length - 1`；`next` 是 `shared_ptr`，`previous` 必须是 `weak_ptr`，不增加所有权；`length <= 0` 返回空指针。`chain_length(head)` 沿 `next` 数节点，空指针返回 0。`links_consistent(head)` 检查每条非空 `next` 的后继，其 `previous.lock()` 必须指回自己，头节点的 `previous` 必须已失效；空指针视为一致。不写 `main`。

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

### 示例

| 调用 | 结果 |
| --- | --- |
| `build_chain(0)` | 空指针 |
| `chain_length(build_chain(5))` | `5` |
| `links_consistent(build_chain(5))` | `true` |

### 提示

造链时用 `node->previous = current;` 把 `shared_ptr` 赋给 `weak_ptr`。末节的 `previous` 是弱引用，整条链只由头部的 `shared_ptr` 拥有；头指针释放之后，链尾的 `previous.lock()` 返回空。比较地址时用 `.get()`。
