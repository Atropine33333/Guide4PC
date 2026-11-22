Write a CRTP comparison mixin so a type that defines only `<` and `==` automatically gets the other four comparison operators. Complete:

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

The judge checks that `Entry` really derives from `Comparable<Entry>` and verifies that all six operators agree. **Do not write `main`.**

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
