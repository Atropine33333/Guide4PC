And the LORD spoke unto Moses, saying:

Make for the congregation a table of horns, and let eight horns hang there and no more. When a man comes to be enrolled, give him a number, beginning at one. When the table is full, the latecomer shall not hang a horn, neither shall the horns already hanging be moved. A horn that is empty shall not be hung.

And Moses said: what of the man who departs? And the LORD said: take down his horn by its number, and let those behind step forward in order, so that the first-comers remain first.

And the LORD said further: when the horns sound, sound them one by one in the order they were hung, the first hung sounding first. If one man brings the same horn to hang twice, it counts as two. While the horns sound, no man shall add or remove a horn.

**In plain words**: Complete `EventBus`. Keep a fixed array of `{id, event, handler, context}` and allow at most 8 subscriptions; `subscribe` returns a positive id or `-1`, `unsubscribe` removes by id and shifts later entries forward, and `publish` calls handlers in subscription order. Do not write `main`.

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

### Rules

- The bus holds at most 8 subscriptions at once; once full, `subscribe` returns -1 and leaves existing subscriptions untouched.
- Every subscription has a unique id; `unsubscribe` removes by id and returns `false` for an unknown id.
- `publish` calls handlers in subscription order, earliest first.
- A null `handler` must not be subscribed.
- The same handler may subscribe to the same event more than once, and each subscription counts separately.
- Do not modify the bus during `publish` (the tests do not do so).
- Do not write `main`.

### Hint

Keep an `Entry` array of `{id, event, handler, context}` and shift later entries left on removal to preserve order.
