A: I have a type here with exactly two tricks: `<` and `==`.

B: That won't carry you far. Sorting reads `<`, but you'll also want to ask "greater", "not greater", "not equal".

A: So I hired it a master called `Comparable<Derived>`. CRTP — the apprentice passes itself in as the template argument.

B: And what does the master teach?

A: Four more: `!=`, `>`, `<=`, `>=`. All of them derived from the apprentice's two tricks; the master writes no second copy of the comparison logic.

B: And how does `Entry` decide who comes first?

A: Higher `score` first; on a tie, smaller `id` first; equal `score`, `id` and `name` together, and only then are the objects equal.

B: All six tricks had better say the same thing. Don't let the mouth say "greater" while the hand says "not greater".

A: Naturally. Six operators in agreement, and sorting and `std::max_element` can be trusted with the type.

**In plain words**: complete the two types below. `Comparable<Derived>` supplies `!=`, `>`, `<=`, `>=` as `friend` functions that call only `Derived`'s `==` and `<`; `Entry` implements `operator<` and `operator==` itself. **Do not write `main`.**

```cpp
template <typename Derived>
struct Comparable {
    // Derived provides operator< and operator==
    // provide !=, >, <=, >= here
};

struct Entry : Comparable<Entry> {
    std::string name;
    int score = 0;
    int id = 0;

    Entry() = default;
    Entry(std::string n, int s, int i);

    bool operator<(const Entry& other) const;   // implement
    bool operator==(const Entry& other) const;  // implement
};
```

Requirements:

- `Comparable<Derived>` implements `!=`, `>`, `<=`, `>=` as `friend` functions that only use `Derived`'s `==` and `<`; do not duplicate the comparison logic.
- `Entry` ranking: larger `score` comes first; for equal `score`, smaller `id` comes first; objects are equal only when `score`, `id`, and `name` all match.
- Sorting and `std::max_element` rely only on `<` and must work directly.

`Entry` must genuinely derive from `Comparable<Entry>`, and all six operators must agree. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
Entry a{"A", 90, 1};
Entry b{"B", 95, 2};
b > a;    // true, supplied by the mixin
a <= b;   // true
```

### Constraints

- Standard library only.
- The six operators must agree with each other; do not hard-code return values to pass individual comparisons.
