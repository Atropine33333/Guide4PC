Implement three functions that handle a `std::variant`.

```cpp
#include <string>
#include <variant>

using Value = std::variant<int, double, std::string>;

std::string kind_of(const Value& value);

double as_double(const Value& value);

std::string describe(const Value& value);
```

Rules:

- `kind_of` returns `"int"`, `"double"`, or `"string"` according to the active alternative.
- `as_double`:
  - for `int`, convert the integer to the corresponding `double`;
  - for `double`, return it unchanged;
  - for `std::string`, return `0.0`.
- `describe` returns exactly these formats:
  - `int`: `int(42)`, `int(-7)`;
  - `double`: fixed notation with exactly two fractional digits, such as `double(3.50)`, `double(-0.25)`, `double(100.00)`;
  - `std::string`: `string(hello)`, and `string()` for an empty string.
- Use `std::visit` or `std::holds_alternative`; do not rely on undefined behavior.
- Do not write `main`, and do not change the alias or the signatures.
