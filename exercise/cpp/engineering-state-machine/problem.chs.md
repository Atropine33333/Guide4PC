实现订单状态机：

```cpp
enum class State { Created, Paid, Shipped, Delivered, Cancelled, Returned };
enum class Event { Pay, Ship, Deliver, Cancel, Return };

class Order {
public:
    Order();

    State state() const;
    bool apply(Event event);   // 事件合法则迁移并返回 true
    int rejected() const;      // 被拒绝的事件次数

    static const char* name(State state);
};
```

合法迁移只有这些：

| 当前状态 | 事件 | 新状态 |
| --- | --- | --- |
| Created | Pay | Paid |
| Created | Cancel | Cancelled |
| Paid | Ship | Shipped |
| Paid | Cancel | Cancelled |
| Shipped | Deliver | Delivered |
| Delivered | Return | Returned |

其余事件一律拒绝：返回 `false`，状态不变，`rejected()` 加一。`Cancelled`、`Returned` 是终态，任何事件都被拒绝。

`name(State)` 返回小写状态名：`"created"`、`"paid"`、`"shipped"`、`"delivered"`、`"cancelled"`、`"returned"`。

评测代码会直接包含你的实现。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
Order order;
order.apply(Event::Pay);     // true，状态变为 Paid
order.apply(Event::Deliver); // false，状态仍为 Paid，rejected() == 1
```

### 约束

- 只用一张迁移表或一段 `switch`，不要把状态判断散落成重复代码。
