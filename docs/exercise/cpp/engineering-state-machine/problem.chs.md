话说这一日，一张订单落在柜上，众人围着看它如何走完这一生。

列位看官，这订单只有六个去处：`Created`、`Paid`、`Shipped`、`Delivered`、`Cancelled`、`Returned`；走路只认五种事件：`Pay`、`Ship`、`Deliver`、`Cancel`、`Return`。哪条路通、哪条路堵，全在下头这张表里。

| 当前状态 | 事件 | 新状态 |
| --- | --- | --- |
| Created | Pay | Paid |
| Created | Cancel | Cancelled |
| Paid | Ship | Shipped |
| Paid | Cancel | Cancelled |
| Shipped | Deliver | Delivered |
| Delivered | Return | Returned |

表上没有的路，一律走不通：`apply` 返回 `false`，状态原地不动，`rejected()` 记上一笔。`Cancelled` 与 `Returned` 是两条断头路，到了那儿，什么事件都过不去。

`name(State)` 报出小写名号：`"created"`、`"paid"`、`"shipped"`、`"delivered"`、`"cancelled"`、`"returned"`。

**人话**：补完 `Order`。用一张迁移表或一个 `switch` 实现 `apply`：合法事件迁移并返回 `true`，其余返回 `false` 且让 `rejected()` 加一。`name` 把状态转成小写字符串。不要写 `main`。

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

### 规则或约定

- 初始状态为 `Created`，`rejected()` 为 0。
- 只有上表六条迁移合法；其余事件返回 `false`，状态不变，`rejected()` 加一。
- `Cancelled`、`Returned` 是终态，任何事件都被拒绝。
- 不要写 `main`。

### 示例

```cpp
Order order;
order.apply(Event::Pay);     // true，状态变为 Paid
order.apply(Event::Deliver); // false，状态仍为 Paid，rejected() == 1
```

### 约束

- 只用一张迁移表或一段 `switch`，不要把状态判断散落成重复代码。
