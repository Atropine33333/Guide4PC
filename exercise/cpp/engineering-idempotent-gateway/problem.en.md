ORDINANCE ON IDEMPOTENT SUBMISSION

Article 1. This Ordinance is enacted to prevent duplicate postings and duplicate acceptance.

Article 2. Where an id has not appeared before, its first submission is recorded as accepted (`accepted`); the amount is kept, and the id is placed among the pending.

Article 3. Where the id exists and the amount is the same, answer duplicate (`duplicate`), and let all things remain as they were. Where the amount differs, answer conflict (`conflict`), and let all things remain as they were.

Article 4. Completion is open only to an id that exists and is not yet complete; record the result and return `true`. For an unknown or already completed id, return `false`. The completed state and its result shall never be cleared.

Article 5. A result is given only for a completed id; for all others, answer `std::nullopt`.

Article 6. The pending count counts only existing, incomplete ids; the completed count counts only completed ids. Both ids and amounts may be negative.

Article 7. Do not write `main`, and do not add, remove, or alter public members.

**In plain words**: Complete `Gateway` with `submit`, `complete`, `is_completed`, `result_of`, `pending`, and `completed`, following the states and return codes above. A repeated submission never disturbs an existing record, and a completed record never returns to pending.

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

### Rules

- The first `submit` for a new id returns `accepted`, records the amount, and leaves the id pending.
- An existing id with the same amount returns `duplicate` and changes no state.
- An existing id with a different amount returns `conflict` and changes no state.
- `complete` succeeds and records the result only for an existing, not-yet-completed id; it returns `false` for an unknown or already completed id.
- `result_of` returns the result only after completion, otherwise `std::nullopt`.
- Submitting an already completed id with the same amount still returns `duplicate`; a different amount still returns `conflict`. Completion state and the result are never cleared.
- `pending()` counts existing but incomplete ids; `completed()` counts completed ids.
- Do not write `main`, and do not change the public interface.
