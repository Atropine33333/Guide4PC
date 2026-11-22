This exercise reads no stdin and writes no stdout. Write three overloads of one function name so that callers can distinguish lvalues, `const` lvalues, and rvalues, tested by a hidden harness.

```cpp
#include <string>

std::string describe(int& value);         // returns "lvalue"
std::string describe(const int& value);   // returns "const-lvalue"
std::string describe(int&& value);        // returns "rvalue"
```

Rules:

- The three overloads must return exactly `lvalue`, `const-lvalue`, and `rvalue`.
- Do not modify the argument (not even in the `int&` version).
- Do not write `main`.

### Hint

Overload resolution prefers the best reference match: a non-const lvalue selects `int&`, a const lvalue can only select `const int&`, and a temporary or `std::move(x)` selects `int&&`. With a single by-value overload, every call lands on that one function.
