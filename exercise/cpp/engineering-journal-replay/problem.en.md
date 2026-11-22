And the LORD spoke unto Moses, saying:

Walk again every station of the journey, according to this journal. Where the journal says `put`, carve the name and the number on the stones outside the tent; if the people were then encamped — that is, a transaction was open — write it in the tent first, and a later carving over the same name blots out the earlier.

Where it says `begin`, pitch the tent; if a tent is already pitched, pay no heed to this `begin`, for one tent only is allowed.

Where it says `commit`, carry the entries from the tent to the stones in order, then strike the tent. Where it says `rollback`, wipe every entry in the tent, then strike the tent. If no tent is pitched, both words are passed over.

A word you do not know, pass over.

If the journey ends with the tent still pitched, none of its entries shall count.

And Moses asked: in what order shall the names on the stones be read? And the LORD said: by the lexicographic order of the names, from smallest to greatest.

**In plain words**: Implement `replay` to return the key-value pairs after replaying the log, sorted by key in ascending `std::string` lexicographic order. Two `std::map`s (committed store and transaction buffer) plus a `bool active` are enough; on `commit`, merge the buffer in order. Do not write `main`.

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

### Rules

- With no active transaction, `put` writes to the store immediately; inside an active transaction it is buffered (a later write to the same key overwrites the earlier one).
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
