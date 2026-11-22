Implement a key-value store that can undo its most recent write.

Implement `VersionedStore`:

- `VersionedStore()`: constructs an empty store.
- `void put(int key, int value)`: insert or overwrite a key and record the write so it becomes the most recent undoable operation.
- `bool get(int key, int& out) const`: if the key exists, write its value to `out` and return `true`; otherwise return `false`.
- `bool undo()`: undo the most recent `put`. If it overwrote a value, restore the old one; if it inserted a key, remove it; return `true`. Return `false` when there is nothing to undo.
- `int size() const`: return the number of keys.
- `void clear()`: remove every key **and discard the entire undo history**. Afterwards `undo()` returns `false`.

Operations may arrive in any interleaving: each `undo()` reverses exactly the most recent `put`.
