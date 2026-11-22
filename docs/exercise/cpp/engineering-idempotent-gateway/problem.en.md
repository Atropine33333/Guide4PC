This task reads no stdin and writes no stdout. Implement an idempotent submission gateway.

```cpp
#include <cstddef>
#include <optional>

enum class SubmitStatus { accepted, duplicate, conflict };

class Gateway {
public:
    Gateway();

    SubmitStatus submit(int id, int amount);
    bool complete(int id, int result);
    bool is_completed(int id) const;
    std::optional<int> result_of(int id) const;
    std::size_t pending() const;
    std::size_t completed() const;
};
```

Rules:

- The first `submit` for a new id returns `accepted`, records the amount, and leaves the id pending.
- An existing id with the same amount returns `duplicate` and changes no state.
- An existing id with a different amount returns `conflict` and changes no state.
- `complete` succeeds and records the result only for an existing, not-yet-completed id; it returns `false` for an unknown or already completed id.
- `result_of` returns the result only after completion, otherwise `std::nullopt`.
- Submitting an already completed id with the same amount still returns `duplicate`; a different amount still returns `conflict`. Completion state and the result must never be cleared.
- `pending()` counts existing but incomplete ids; `completed()` counts completed ids.
- Both ids and amounts may be negative.
- Do not write `main`, and do not change the public interface.
