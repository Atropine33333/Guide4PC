This task reads no stdin and writes no stdout. Implement a simple inventory ledger.

```cpp
#include <cstddef>
#include <string>

class Inventory {
public:
    Inventory();

    void add(const std::string& name, int quantity);
    bool remove(const std::string& name, int quantity);
    int quantity(const std::string& name) const;
    std::size_t distinct() const;
    long long total() const;
};
```

Rules:

- `add` accumulates onto any existing quantity; ignore calls with `quantity <= 0`.
- `remove` deducts and returns `true` when enough stock exists; erase the name when its quantity reaches exactly 0.
- Return `false` and change nothing when the name is absent, stock is insufficient, or `quantity <= 0`.
- `quantity` returns 0 for an unknown name.
- `distinct()` returns the number of names with positive quantity.
- `total()` returns the sum of all quantities as a 64-bit integer.
- Names are case-sensitive, may contain spaces, and may be empty.
- Do not write `main`, and do not change the public interface.
