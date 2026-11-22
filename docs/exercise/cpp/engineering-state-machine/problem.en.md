Implement an order state machine:

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

The only legal transitions are:

| Current state | Event | New state |
| --- | --- | --- |
| Created | Pay | Paid |
| Created | Cancel | Cancelled |
| Paid | Ship | Shipped |
| Paid | Cancel | Cancelled |
| Shipped | Deliver | Delivered |
| Delivered | Return | Returned |

Every other event is rejected: return `false`, keep the state, and increment `rejected()`. `Cancelled` and `Returned` are terminal, so any event is rejected there.

`name(State)` returns the lowercase state name: `"created"`, `"paid"`, `"shipped"`, `"delivered"`, `"cancelled"`, `"returned"`.

The judge includes your implementation. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
Order order;
order.apply(Event::Pay);     // true, state becomes Paid
order.apply(Event::Deliver); // false, state stays Paid, rejected() == 1
```

### Constraints

- Use one transition table or one `switch`; do not scatter duplicated state checks.
