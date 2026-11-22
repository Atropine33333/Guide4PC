This exercise reads no stdin and writes no stdout. Implement a fixed-capacity event bus; a hidden harness runs the tests.

Implement:

```cpp
using EventHandler = void (*)(void* context, int payload);

class EventBus {
public:
    EventBus();

    // Return a positive id on success; return -1 when full or the handler is null.
    int subscribe(int event, EventHandler handler, void* context);

    // Remove the subscription; return whether it existed.
    bool unsubscribe(int id);

    // Call every handler subscribed to event, in subscription order.
    void publish(int event, int payload);

    int count(int event) const;  // current subscriptions for this event
    int total() const;           // current total subscriptions
    void clear();                // remove all subscriptions
};
```

Requirements:

- The bus holds at most 8 subscriptions at once; once full, `subscribe` returns -1 and leaves existing subscriptions untouched;
- Every subscription has a unique id; `unsubscribe` removes by id and returns `false` for an unknown id;
- `publish` calls handlers in subscription order, earliest first;
- A null `handler` must not be subscribed;
- The same handler may subscribe to the same event more than once, and each subscription counts separately;
- Do not modify the bus during `publish` (the tests do not do so);
- Do not write `main`.

Hint: keep an `Entry` array of `{id, event, handler, context}` and shift later entries left on removal to preserve order.
