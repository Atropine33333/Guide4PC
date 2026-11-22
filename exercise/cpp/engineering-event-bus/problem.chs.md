耶和华晓谕摩西说：

你要为会众造一座号角台，号角只挂八支。人来登记，登记过的，给他一个编号，从一数起。台子挂满了，后来的人不许挂，已有的号角也不许挪动。号角若是空的，不挂。

摩西问：人走了怎么办？耶和华说：按编号取下，空出的位置由后面的人依次补上，先来的人仍旧在前。

耶和华又说：吹号的时候，按挂上去的先后，一支一支地吹，先挂的先响。同一个人拿同一支号来挂两回，算两支。吹号的时候，谁也不许增删号角。

**人话**：补完 `EventBus`。用定长数组存 `{id, event, handler, context}`，最多 8 条订阅；`subscribe` 返回正整数编号或 `-1`，`unsubscribe` 按编号删除，删除后把后面的元素前移，`publish` 按订阅顺序回调。不要写 `main`。

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

### 规则或约定

- 总线最多同时保存 8 个订阅；达到上限后 `subscribe` 返回 -1，且不改变已有订阅。
- 每个订阅有唯一编号，`unsubscribe` 按编号删除；编号不存在时返回 `false`。
- `publish` 按订阅的先后顺序调用，先订阅的先被调用。
- 空 `handler` 不能订阅。
- 同一个 handler 可以多次订阅同一个事件，每次算一个独立订阅。
- 发布过程中不要修改总线（也不必支持这种情况）。
- 不要写 `main`。

### 提示

用一个 `Entry` 数组保存 `{id, event, handler, context}`，删除时把后面的元素前移即可保持顺序。
