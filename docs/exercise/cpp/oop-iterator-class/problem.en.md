This exercise reads no stdin. Implement an integer-range iterator usable with `range-for`.

```cpp
#include <iterator>

class IntRange {
public:
    IntRange(int start, int stop, int step = 1);

    class iterator {
    public:
        int operator*() const;
        iterator& operator++();
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };

    iterator begin() const;
    iterator end() const;
};
```

Rules:

- The range starts at `start` and adds `step` each time, producing values strictly less than `stop`; when `step` is negative it produces values strictly greater than `stop`.
- For an empty range, `begin()` equals `end()` and a `range-for` body never runs.
- The iterator only needs the four operations `range-for` uses: `*it`, `++it`, `it != end`, `it == end`.
- Iterators are copied by value; copying one must not affect another.
- When `step` does not divide `stop - start`, stop after passing `stop`; do not loop forever because the value can never equal `stop`.
- Do not write `main`.

### Example

`IntRange(1, 10, 3)` produces `1, 4, 7`; `IntRange(5, 0, -2)` produces `5, 3, 1`.

### Hint

An iterator can store the current value, the step, and `stop`, plus an internal `done` flag; `end()` returns an iterator whose flag is set. Then `!=` becomes false once the value passes the bound, even when the step does not divide the span.
