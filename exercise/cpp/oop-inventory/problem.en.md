The kingdom of heaven is like a householder who handed the storehouse keys to a servant and said: "Whatever goods come in, write them down by number. Whatever is taken out, first see whether the ledger can bear it. If it can, deduct it; if the last grain is gone, strike the name from the book. If it cannot — or the number is not seemly — touch nothing, and answer only 'it cannot be done'."

The servant asked: "And if the same name comes in twice?"

The householder said: "Add the two in one place; no need for a second line. Names on this ledger are reckoned letter by letter, upper case apart from lower; names with spaces and even empty names are each reckoned on their own. At month's end count the names and sum the quantities; make the sum wide, for only a 64-bit number will hold it."

**In plain words**: Implement the inventory ledger `Inventory` over a string-to-integer map: `add` accumulates onto any existing quantity (ignore `quantity <= 0`); `remove` deducts and returns `true` when the stock suffices, erasing the name when it reaches exactly 0, and otherwise returns `false` with the stock unchanged; `quantity(name)` reads a quantity (0 for an unknown name); `distinct()` counts names with a positive quantity; `total()` sums every quantity into a 64-bit integer.

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

### Rules

- `add` accumulates onto any existing quantity; ignore calls with `quantity <= 0`.
- `remove` deducts and returns `true` when enough stock exists; erase the name when its quantity reaches exactly 0.
- Return `false` and change nothing when the name is absent, stock is insufficient, or `quantity <= 0`.
- `quantity` returns 0 for an unknown name.
- `distinct()` returns the number of names with positive quantity.
- `total()` returns the sum of all quantities as a 64-bit integer.
- Names are case-sensitive, may contain spaces, and may be empty.
- Do not write `main`, and do not change the public interface.
