Implement functions that lay out a "name + amount" row as a fixed-width table line.

```cpp
#include <ostream>
#include <string>

std::string format_row(const std::string& name, long long amount);
void print_row(std::ostream& out, const std::string& name, long long amount);
```

Rules:

- `format_row` returns one line of text without a newline.
- The name is left-aligned in 12 characters: pad with spaces on the right when shorter, and keep it unchanged (never truncate) when longer.
- A single `|` follows the name immediately.
- The amount is right-aligned in 10 characters: pad with spaces on the left when shorter, and keep it unchanged when longer.
- `print_row` writes the result of `format_row` plus `'\n'` to `out`; do not use `std::cout`.
- Do not write `main`.

### Example

- `format_row("ann", 5)` returns `ann         |         5`
- `format_row("abcdefghijkl", 1234567890)` returns `abcdefghijkl|1234567890`
- `format_row("bob", -42)` returns `bob         |       -42`

### Hint

Use `std::setw(12) << std::left` for the name and `std::setw(10) << std::right` for the amount. Width is only a minimum, so longer content is not truncated. `std::setfill(' ')` makes sure the padding is spaces.
