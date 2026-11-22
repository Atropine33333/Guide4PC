(An order lands on the counter, and the crowd gathers to watch it live out its life.)

Hear this, honoured listeners: the order has six resting places — `Created`, `Paid`, `Shipped`, `Delivered`, `Cancelled`, `Returned` — and it walks on five events only: `Pay`, `Ship`, `Deliver`, `Cancel`, `Return`. Which road is open and which is barred is written in the table below.

| Current state | Event | New state |
| --- | --- | --- |
| Created | Pay | Paid |
| Created | Cancel | Cancelled |
| Paid | Ship | Shipped |
| Paid | Cancel | Cancelled |
| Shipped | Deliver | Delivered |
| Delivered | Return | Returned |

Any road not in that table is barred: `apply` returns `false`, the state stays put, and `rejected()` takes down a mark. `Cancelled` and `Returned` are dead ends; once there, no event passes.

`name(State)` announces the lower-case name: `"created"`, `"paid"`, `"shipped"`, `"delivered"`, `"cancelled"`, `"returned"`.

**In plain words**: Complete `Order`. Implement `apply` with one transition table or one `switch`: a legal event changes state and returns `true`; anything else returns `false` and increments `rejected()`. `name` turns a state into its lower-case string. The judge includes your implementation; do not write `main`.

```cpp
enum class State { Created, Paid, Shipped, Delivered, Cancelled, Returned };
enum class Event { Pay, Ship, Deliver, Cancel, Return };

class Order {
public:
    Order();

    State state() const;
    bool apply(Event event);   // true when the event is legal and the state changes
    int rejected() const;      // number of rejected events

    static const char* name(State state);
};
```

### Rules

- The initial state is `Created` and `rejected()` is 0.
- Only the six transitions in the table are legal; everything else returns `false`, keeps the state, and increments `rejected()`.
- `Cancelled` and `Returned` are terminal, so any event is rejected there.
- Do not write `main`.

### Example

```cpp
Order order;
order.apply(Event::Pay);     // true, state becomes Paid
order.apply(Event::Deliver); // false, state stays Paid, rejected() == 1
```

### Constraints

- Use one transition table or one `switch`; do not scatter duplicated state checks.
