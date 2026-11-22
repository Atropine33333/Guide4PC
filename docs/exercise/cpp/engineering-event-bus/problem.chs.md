这一题不读 stdin，也不写 stdout。实现一个固定容量的事件总线，测试在隐藏的 harness 里。

实现：

```cpp
using EventHandler = void (*)(void* context, int payload);

class EventBus {
public:
    EventBus();

    // 成功时返回正整数编号；已达上限或 handler 为空时返回 -1。
    int subscribe(int event, EventHandler handler, void* context);

    // 找到并删除订阅，返回是否删除成功。
    bool unsubscribe(int id);

    // 按订阅顺序调用所有订阅了 event 的 handler。
    void publish(int event, int payload);

    int count(int event) const;  // 该事件当前的订阅数
    int total() const;           // 当前总订阅数
    void clear();                // 删除所有订阅
};
```

要求：

- 总线最多同时保存 8 个订阅；达到上限后 `subscribe` 返回 -1，且不改变已有订阅；
- 每个订阅有唯一编号，`unsubscribe` 按编号删除；编号不存在时返回 `false`；
- `publish` 按订阅的先后顺序调用，先订阅的先被调用；
- 空 `handler` 不能订阅；
- 同一个 handler 可以多次订阅同一个事件，每次算一个独立订阅；
- 发布过程中不要修改总线（测试不会这样做）；
- 不要写 `main`。

提示：用一个 `Entry` 数组保存 `{id, event, handler, context}`，删除时把后面的元素前移即可保持顺序。
