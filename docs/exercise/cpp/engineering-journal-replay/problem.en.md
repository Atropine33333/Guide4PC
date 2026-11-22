This exercise reads no stdin. Replay a simple transaction log into the final state of a key-value store.

```cpp
#include <string>
#include <utility>
#include <vector>

struct JournalEntry {
    std::string op;   // "put" / "begin" / "commit" / "rollback"
    std::string key;
    int value;
};

std::vector<std::pair<std::string, int>> replay(
    const std::vector<JournalEntry>& entries);
```

Rules:

- With **no active transaction**, `put` writes to the store immediately; inside an active transaction it is buffered (a later write to the same key overwrites the earlier one).
- `begin` starts a transaction; if one is already active, ignore this `begin` (only flat transactions are supported).
- `commit` applies the buffered writes to the store in order, clears the buffer, and ends the transaction; with no active transaction it is ignored.
- `rollback` discards the buffer and ends the transaction; with no active transaction it is ignored.
- An unknown `op` is ignored.
- If the log ends with an active transaction, all buffered writes are discarded (an implicit rollback).
- Return the final key-value pairs sorted by key in lexicographic order (`std::string`'s `<`).
- Do not write `main`.

### Example

Log: `put a 1`, `begin`, `put a 5`, `put b 3`, `commit`. The result is `[("a", 5), ("b", 3)]`.

If the last entry were `rollback` instead of `commit`, the result would be `[("a", 1)]`.

### Hint

Use two `std::map<std::string, int>` objects, one for the committed store and one for the transaction buffer, plus a `bool active`. Converting the committed map to a vector already yields lexicographic order.
